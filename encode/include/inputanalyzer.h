#ifndef _INPUTANALYZER_H
#define  _INPUTANALYZER_H

#include "global.h"
#include <iostream>
#include <list>

class inputanalyzer {
    public:
        inputanalyzer();
        ~inputanalyzer();
        QRMode analyze(char *url);

    private:
        void init();
        bool isCharacterOnList(std::list<int> clist, int urlLength);
 //       QRMode m_qrmode;
   // int len = 0;
    // numeric: {0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39}
    // 0,1,2,3,4,5,6,7,8,9,A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y,Z,{SP},$,%,*,+,-,.,/,:
    // alphaNumeric: {0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
    //                         0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F, 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A
    //                         0x20, 0x24, 0x25, 0x2D, 0x2A, 0x2B, 0x2E, 0x2F, 0x3A} 
    
   //     char *m_url;
        int m_counter[MAX_NUM_CHARACTER]; // input character counter        
        std::list<int> m_qrCharList[NUM_QRMODE];
 //       int m_url_len;
};

#endif
