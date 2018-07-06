#ifndef _BITSTREAM_H
#define _BITSTREAM_H

//#include "stdio.h"
//#include "global.h"
#include "qrparam.h"
#include "qrimage.h"

#define CW0_PADDING 0xED // 0x11101100
#define CW1_PADDING 0x11  // 0x00010001

//struct byteSwapFcn {
    typedef  void (*byteswap)(short );
 //   byteswap fn;

//};
//m_byteswap_fn = (m_bLittleEndian) ? byteSwapFcn::swapLittle: byteSwapFcn::swapBig;
class ssconverter {
    public:
        ssconverter();
        ssconverter(qrparam *param);
        ~ssconverter();
        int read();
     //   int write();
        int writeheader();
     //   int writedata(qrsymbol *qrSymbols);
 //    unsigned char * ToStream();
     int write(qrsymbol *qrSymbols);
     int writedata(int dataToSubmit, int size);//, byteSwapFcn::byteswap *bsFcn);
     void generatePlacementMap(qrimage base_image);
        //      auto fn;
    private:
        bool isLittleEndian();
        
        void transpose();
        byteswap *bsFcn;

        
        unsigned char *m_data;
        qrparam *m_param;
        int m_cwSizeInbits;
        int m_qrdataSize;
        bool m_bLittleEndian;
        

};

#endif
