#ifndef _QRENCODE_H
#define  _QRENCODE_H

#include <iostream>
#include "qrparam.h"
#include "iffmpeg.h"

class qrencode {
public:
    qrencode();
    ~qrencode();
    qrencode(char *url, QRMode qrMode, ECLevel ecLevel);
    qrsymbol charsToSymbol(QRMode qrMode, char *url);
    unsigned char * symbolToQRCodeword(qrsymbol symbols, QRMode qrMode);
    bool encode(qrsymbol symbol);
//    int * qrencode::charToSymbol(char *url);
    
private:
    qrparam *m_qrparam;
    iffmpeg *m_ffmpeg;
};

#endif
