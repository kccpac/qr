#include <iostream>
#include "string.h"
#include "qrparam.h"
#include "inputanalyzer.h"

 qrparam::qrparam() {
    m_mode = QRMODE_UNDEFINED;     
    m_qrcode_size = m_qrdata_size = m_num_char_per_sym = m_size_per_sym = -1;
    ECLevel m_level = ECLEVEL_UNDEFINED;
 }
 
qrparam::~qrparam() {

}

qrparam::qrparam(QRMode qrMode, ECLevel ecLevel, int len):qrparam() {
    init(qrMode, ecLevel, len);
}

void qrparam::init(QRMode qrMode, ECLevel ecLevel, int len) {

    int symbol_size = -1; 
    int size_per_sym = -1;
    int estimated_qrdata_size = -1;
    int *dataCapacity;
    int version = 0;
    int R[] = {0, 4, 7};
    
    if (qrMode == QRMODE_UNDEFINED) {
        std::cout << " Invalid QR Mode \n";
        return;
    }
    
    if (qrMode == QRMODE_KANJI) {
        std::cout << " QR Mode " << qrMode << " is not implemented \n";
        return;
    }
    dataCapacity = erdata_capacity[qrMode][ecLevel];        
    for (int i=NUM_VERSION; i>0; i--) {
        if (len > dataCapacity[i]) {
            version = i;
            break;
        }
    }
    
    if (version == NUM_VERSION) {
        std::cout << " The url size of " << len << " is too large to be encoded as qr code \n";
        return;
    }
    
    m_input_len = len;
    m_version = version;
    m_qrcode_size  = dataCapacity[version];
    m_qrdata_size = 4 + CHAR_COUNT_INDICATOR_SIZE[version][qrMode];    
    switch (qrMode) {
        case QRMODE_NUMERIC: // subset of ALPHANUMERIC           
            // B = 4 + C + 10(D DIV 3) + R
            // R = 0 if (DMOD 3) = 0
            // R = 4 if (D MOD 3) = 1
            // R = 7 if (D MOD 3) = 2
              m_size_per_sym =  size_per_sym  = NUMERIC_SYM_SIZE; //
            m_num_char_per_sym  = symbol_size = CHAR_SIZE_PER_NUMERIC_SYM;;
            m_qrdata_size +=  size_per_sym * len/symbol_size + R[len%symbol_size];             
            break;
        case QRMODE_ALPHANUMERIC: // subset of JS_EIGHT
            // B = 4 + C + 11(D DIV 2) + 6(D MOD 2)
            // B = number of bits in bit stream
            // C = number of bits in Character Count Indicator ( from Table 3)
            // D = number of input data characters
             m_num_char_per_sym = symbol_size = CHAR_SIZE_PER_ALPHANUMERIC_SYM;
             m_size_per_sym  = size_per_sym = ALPHANUMERIC_SYM_SIZE;
            m_qrdata_size +=  size_per_sym * len/symbol_size + 6*(len%symbol_size);
            break;
        case QRMODE_JS_EIGHT:
            symbol_size = JS_EIGHT_SYM_SIZE;
            // B = 4 + C + 8D
            m_size_per_sym = size_per_sym = JS_EIGHT_SYM_SIZE;
            m_num_char_per_sym = 1;
            m_qrdata_size += size_per_sym * len; 
            break;
        case QRMODE_KANJI:
            m_qrdata_size = -1;
            break;
        default:
            m_qrdata_size = -1;
            break;    
    }
}

/*
void qrparam::set_QRMode(char *url) {

   // int ctcounter[MAX_NUM_CHARACTER];
   // int len = 0;
    // numeric: {0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39}
    // 0,1,2,3,4,5,6,7,8,9,A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y,Z,{SP},$,%,*,+,-,.,/,:
    // alphaNumeric: {0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
    //                         0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F, 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A
    //                         0x20, 0x24, 0x25, 0x2D, 0x2A, 0x2B, 0x2E, 0x2F, 0x3A} 
    int i = 0;
    
    if (url == NULL) return;
    
    inputanalyzer *analyzer = new inputanalyzer();

    QRMode qrMode = analyzer->analyze(url);
    std::cout << "NUM_QRMODE= " <<qrMode << "\n";
    delete analyzer;     
}
*/
