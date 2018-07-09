#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "iffmpeg.h"

iffmpeg::iffmpeg() {
    init();
}

iffmpeg::~iffmpeg() {
    avcodec_free_context(&m_enc_ctx);
    if (m_out_image) delete [] m_out_image;
    m_out_image = NULL;
    m_out_size = 0;
}

void iffmpeg::init() {


//    ffmpeg = new find_encoder_by_name[1];
//    *ffmpeg = &find_encoder;
    //const char codec_name[] = "jpg";

    if ((m_avcodec =avcodec_find_encoder(AV_CODEC_ID_PNG)) == NULL) {
 //       fprintf(stderr, "Codec '%s' not found\n", codec_name);
        exit(1);
    }

    if ((m_enc_ctx= avcodec_alloc_context3(m_avcodec)) == NULL) {
        //fprintf(stderr, "Could not allocate video codec context\n");
        exit(1);
    }    
    
    m_out_image = new unsigned char [DEFAULT_BUFFER_SIZE];
    m_image_buffer_size = 2048;
    m_out_size = 0;

    
}

void  iffmpeg::setup(int width, int height, int pitch) {
//    m_ffmpeg->setup(image);//.getQRImageWidth(), image.getQRImageHeight(), image.getQRImagePitch());
    m_image_pitch = pitch;// image.getQRImagePitch();
    m_enc_ctx->width = width;//image.getQRImageWidth();
    m_enc_ctx->height = height;//image.getQRImageHeight();
    m_enc_ctx->pix_fmt = AV_PIX_FMT_GRAY8;    
        // put sample parameters 
   m_enc_ctx->bit_rate = 400000;
    // resolution must be a multiple of two 
    m_enc_ctx->time_base = (AVRational){1, 25};
    m_enc_ctx->framerate = (AVRational){25, 1};
    
    int  ret = avcodec_open2(m_enc_ctx, m_avcodec, NULL);
    if (ret < 0) {
        // fprintf(stderr, "Could not open codec: %s\n", av_err2str(ret));
        exit(1);
    }
//    m_enc_ctx->time_base = (AVRational){1, 25};
}

AVFrame * iffmpeg::getAVFRame(unsigned char *qrimage) {

    AVFrame *frame;
    int ret = 0;

    if ((frame = av_frame_alloc()) == NULL) {
        std::cout << "Could not allocate video frame\n";
        return NULL;
   // exit(1);
    }
    frame->format = m_enc_ctx->pix_fmt;
    frame->width  = m_enc_ctx->width;
    frame->height = m_enc_ctx->height;

    if ((ret = av_frame_get_buffer(frame, 0)) < 0) {
        std::cout << "Could not allocate the video frame data\n";
        return NULL;
    }
    
    ret = av_frame_make_writable(frame);
    if (ret < 0) {
        av_frame_free(&frame);
        return NULL;
    }

    // prepare a dummy image 
    // Y
    unsigned char *image_ptr;
    for (int y = 0; y < m_enc_ctx->height; y++) {
        image_ptr = qrimage + y *m_image_pitch;
        //for (int x = 0; x < m_enc_ctx->width; x++) {
        memcpy(&frame->data[0][y * frame->linesize[0]], image_ptr, m_enc_ctx->width);
        //frame->data[0][y * frame->linesize[0] + x] = *(image_ptr+x);//128;//x;// + y;// + i * 3;
        //}
    }
    return frame;
}
void iffmpeg::encode(AVFrame *frame,  AVPacket *pkt) {

    int ret = 0;

    if ((ret = avcodec_send_frame(m_enc_ctx, frame)) < 0) {
        std::cout << "Error sending a frame for encoding\n";
        return;
    }
    while (ret >= 0) {
        ret = avcodec_receive_packet(m_enc_ctx, pkt);
        printf("ret %d \n", ret);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
            return;
        } else if (ret < 0) {
            std::cout << "Error during encoding\n";
            return;
        }
        printf("Write packet %" PRId64 " (size=%5d)\n", pkt->pts, pkt->size);
        if (pkt->size + m_out_size > m_image_buffer_size) {
            int new_size = (pkt->size < DEFAULT_BUFFER_SIZE) ? DEFAULT_BUFFER_SIZE: ((pkt->size + 15) >> 4) << 4;
            unsigned char *tempBuffer = new unsigned char [m_image_buffer_size+new_size];
            memset(tempBuffer, 0, m_image_buffer_size+new_size);
            memcpy(tempBuffer, m_out_image, m_image_buffer_size);
            m_image_buffer_size += new_size;
            delete m_out_image;
            m_out_image = tempBuffer;
        }
        memcpy(m_out_image+m_out_size, pkt->data, pkt->size);
 //       outSize += pkt->size;
        m_out_size += pkt->size;
        av_packet_unref(pkt);
    }
}

void iffmpeg::encode(unsigned char * imageData) {

    AVPacket *pkt;

    if ((pkt = av_packet_alloc()) == NULL)  {
        return;
    }
    AVFrame *frame = getAVFRame(imageData);
    if (frame) encode(frame, pkt);
    encode(NULL, pkt);
    av_frame_free(&frame);
    av_packet_free(&pkt);
//    av_frame_free(&frame);
}
/*
void iffmpeg::finish_encode() {

}
*/
void iffmpeg::save() {

    FILE *f = NULL ;
//    printf("m_out_size %d \n", m_out_size);
    if (!m_out_image) return;
    if ((f =fopen("test.png", "wb")) == NULL) {
        printf("File could not open\n");
        return;
       // exit(1);
    }
    fwrite(m_out_image, 1, m_out_size, f);
    if (f) fclose(f);
}


