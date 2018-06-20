#ifndef _QRPARAM_H
#define  _QRPARAM_H

#include "global.h"

class qrparam {
    public:    
     qrparam();
    ~qrparam();    
    qrparam(QRMode qrMode, ECLevel ecLevel, int len);
    
    QRMode get_QRMode() { return m_mode; };
    ECLevel get_ECLevel() { return m_level; };
    int get_QRCodeSize() { return m_qrcode_size; };
    int get_QRDataSize() { return m_qrdata_size; };
    int get_NumCharPerQRSymbol() { return m_num_char_per_sym; };
    int get_QRSymbolSize() { return m_size_per_sym; };
    int get_QRVersion() { return m_version; };
    int get_InputLen() {return m_input_len; };

    private:
        void init(QRMode qrMode, ECLevel ecLevel, int len);
    //    void set_QRMode(char *url);
        QRMode m_mode;        
        int m_qrcode_size;
        int m_qrdata_size;
        int m_num_char_per_sym;// = symbol_size = NUMERIC_SYM_SIZE;
        int m_size_per_sym;// = size_per_sym = CHAR_SIZE_PER_NUMERIC_SYM;
        
        ECLevel m_level;
        int m_input_len;
        int m_version;
 //       char *m_url;
};

#endif


