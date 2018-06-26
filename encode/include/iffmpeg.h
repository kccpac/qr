#ifndef _IFFMPEG_H
#define _IFFMPEG_H

extern "C" {
#include "../../external/ffmpeg/include/libavcodec/avcodec.h"

#include "../../external/ffmpeg/include/libavutil/opt.h"
#include  "../../external/ffmpeg/include/libavutil/imgutils.h"
}

//typedef  void (*find_encoder_by_name)( AVCodecContext *ctx);

    
class iffmpeg {
    public:
        iffmpeg();
        ~iffmpeg();
    //    void encode(AVFrame *frame, AVPacket *pkt,  FILE *outfile);
        void setup(int width, int height);
        void encode(unsigned char *qrimage);
        
    private:
        void init();
        
        AVCodecContext *m_enc_ctx;
        AVCodec *m_avcodec;
 //       find_encoder_by_name *ffmpeg;

};

#endif
