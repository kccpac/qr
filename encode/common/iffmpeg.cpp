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
    

    
}

void  iffmpeg::setup(int width, int height) {

    m_enc_ctx->width = width;
    m_enc_ctx->height = height;
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

void iffmpeg::encode(unsigned char *qrimage) {//AVCodecContext *enc_ctx, AVFrame *frame) {//, AVPacket *pkt) {//, FILE *outfile) {
    
    AVFrame *frame;
    AVPacket *pkt;
    int ret = 0;
    
    if ((frame = av_frame_alloc()) == NULL) {
        std::cout << "Could not allocate video frame\n";
        return;
   // exit(1);
    }
    frame->format = m_enc_ctx->pix_fmt;
    frame->width  = m_enc_ctx->width;
    frame->height = m_enc_ctx->height;

    if ((ret = av_frame_get_buffer(frame, 0)) < 0) {
        std::cout << "Could not allocate the video frame data\n";
        return;
    }
    
    ret = av_frame_make_writable(frame);
    if (ret < 0)
        return;

    // prepare a dummy image 
    // Y
    for (int y = 0; y < m_enc_ctx->height; y++) {
        for (int x = 0; x < m_enc_ctx->width; x++) {
            frame->data[0][y * frame->linesize[0] + x] = 128;//x;// + y;// + i * 3;
        }
    }
    
    if ((pkt = av_packet_alloc()) == NULL)  {
        return;
    }
    
    if ((ret = avcodec_send_frame(m_enc_ctx, frame)) < 0) {
        std::cout << "Error sending a frame for encoding\n";
        return;
    }
    
    while (ret >= 0) {
        ret = avcodec_receive_packet(m_enc_ctx, pkt);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
            return;
        else if (ret < 0) {
            std::cout << "Error during encoding\n";
            return;
        }
        printf("Write packet %3"PRId64" (size=%5d)\n", pkt->pts, pkt->size);
        av_packet_unref(pkt);
    }
}

/*
int main(int argc, char **argv)
{
    const char *filename, *codec_name;
    const AVCodec *codec;
    AVCodecContext *c= NULL;
    int i, ret, x, y;
    FILE *f;
    AVFrame *frame;
  //  AVPacket *pkt;
 //   uint8_t endcode[] = { 0, 0, 1, 0xb7 };

    if (argc <= 2) {
        fprintf(stderr, "Usage: %s <output file> <codec name>\n", argv[0]);
        exit(0);
    }
    filename = argv[1];
    codec_name = argv[2];

    // find the mpeg1video encoder 
    
    codec = avcodec_find_encoder_by_name(codec_name);
    if (!codec) {
        fprintf(stderr, "Codec '%s' not found\n", codec_name);
        exit(1);
    }

    c = avcodec_alloc_context3(codec);
    if (!c) {
        fprintf(stderr, "Could not allocate video codec context\n");
        exit(1);
    }

    pkt = av_packet_alloc();
    if (!pkt)
        exit(1);

    // put sample parameters 
    c->bit_rate = 400000;
    // resolution must be a multiple of two 
    c->width = 352;
    c->height = 288;
    // frames per second 
//    c->time_base = (AVRational){1, 25};
//    c->framerate = (AVRational){25, 1};

//    c->gop_size = 10;
//    c->max_b_frames = 1;
    c->pix_fmt = AV_PIX_FMT_GRAY8;//AV_PIX_FMT_YUV420P;

 //  if (codec->id == AV_CODEC_ID_H264)
 //       av_opt_set(c->priv_data, "preset", "slow", 0);

    // open it 
    if (ret = avcodec_open2(c, codec, NULL)) < 0) {
        fprintf(stderr, "Could not open codec: %s\n", av_err2str(ret));
        exit(1);
    }

  //  f = fopen(filename, "wb");
  //  if (!f) {
  //      fprintf(stderr, "Could not open %s\n", filename);
  //      exit(1);
  //  }

    if ((frame = av_frame_alloc()) == NULL) {
        fprintf(stderr, "Could not allocate video frame\n");
        exit(1);
    }
    frame->format = c->pix_fmt;
    frame->width  = c->width;
    frame->height = c->height;

    ret = av_frame_get_buffer(frame, 32);
    if (ret < 0) {
        fprintf(stderr, "Could not allocate the video frame data\n");
        exit(1);
    }

    // encode 1 second of video 
    i = 0;
//    for (i = 0; i < 25; i++)
    // {
     //   fflush(stdout);

    // make sure the frame data is writable 
    if ((ret = av_frame_make_writable(frame)) < 0)
        exit(1);

    //
    //for (y = 0; y < c->height; y++) {
    //    for (x = 0; x < c->width; x++) {
   //         frame->data[0][y * frame->linesize[0] + x] = x + y + i * 3;
    //    }
  //  }

    // Cb and Cr 
//        for (y = 0; y < c->height/2; y++) {
//        for (x = 0; x < c->width/2; x++) {
//            frame->data[1][y * frame->linesize[1] + x] = 128 + y + i * 2;
//            frame->data[2][y * frame->linesize[2] + x] = 64 + x + i * 5;
//        }
//    }

    frame->pts = i;

    // encode the image
    encode(c, frame, pkt, f);
 //   }

    // flush the encoder 
    encode(c, NULL, pkt, f);

    // add sequence end code to have a real MPEG file 
//    fwrite(endcode, 1, sizeof(endcode), f);
//    fclose(f);

    avcodec_free_context(&c);
    return 0;
} */
