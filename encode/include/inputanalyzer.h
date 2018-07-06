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

        int m_counter[MAX_NUM_CHARACTER]; // input character counter        
        std::list<int> m_qrCharList[NUM_QRMODE];
 //       int m_url_len;
};

#endif
