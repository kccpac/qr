#ifndef _IFFMPEG_H
#define _IFFMPEG_H

extern "C" {
#include "../../external/ffmpeg/include/libavcodec/avcodec.h"
#include "../../external/ffmpeg/include/libavutil/opt.h"
#include  "../../external/ffmpeg/include/libavutil/imgutils.h"

#define DEFAULT_BUFFER_SIZE 2048
}


//#include "qrimage.h"

//typedef  void (*find_encoder_by_name)( AVCodecContext *ctx);

    
class iffmpeg {
    public:
        iffmpeg();
        ~iffmpeg();
    //    void encode(AVFrame *frame, AVPacket *pkt,  FILE *outfile);
        void setup(int width, int height, int pitch);
        void start_encode(unsigned char *imageData);
        void finish_encode();
        void save();
    private:
        void init();
        void encode(AVFrame *frame);
        AVFrame * getAVFRame(unsigned char *qrimage) ;
        
        AVCodecContext *m_enc_ctx;
        AVCodec *m_avcodec;
        int m_image_pitch;
        unsigned char *m_out_image;
        int m_image_buffer_size;
        int m_out_size;
 //       find_encoder_by_name *ffmpeg;

};

#endif
