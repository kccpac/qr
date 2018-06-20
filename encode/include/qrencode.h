#include <iostream>
#include "qrparam.h"

class qrencode {
public:
    qrencode();
    ~qrencode();
    qrencode(char *url, QRMode qrMode, ECLevel ecLevel);
    int * charsToSymbol(QRMode qrMode, char *url);
//    int * qrencode::charToSymbol(char *url);
    
private:
    qrparam *m_qrparam;
};
