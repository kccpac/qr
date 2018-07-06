#include "string.h"
#include "qrconst.h"
#include "global.h"
#include "inputanalyzer.h"

inputanalyzer::inputanalyzer() {
    init();
}

inputanalyzer::~inputanalyzer() {
//std::list<int>::iterator itr;
  for (int i=0; i<NUM_QRMODE; i++)  {
      m_qrCharList[i].clear();

//          std::cout << "i= " << i << " " << m_qrCharList[i].size() << "\n"; 
     }
//      std::cout << "~inputanalyzer end  \n";
}

void inputanalyzer::init() {
   /*
    for (int i=0; i<MAX_NUM_CHARACTER; i++)
        JS_EIGHT_CHAR_LIST[i] = i;
  */
    m_qrCharList[QRMODE_NUMERIC].assign(NUMERIC_CHAR_LIST, NUMERIC_CHAR_LIST+NUMERIC_CHAR_SIZE);
    m_qrCharList[QRMODE_ALPHANUMERIC].assign(ALPHANUMERIC_CHAR_LIST, ALPHANUMERIC_CHAR_LIST+ALPHANUMERIC_CHAR_SIZE);
 //   m_qrCharList[QRMODE_JS_EIGHT].assign(JS_EIGHT_CHAR_LIST, JS_EIGHT_CHAR_LIST+MAX_NUM_CHARACTER);
 //   m_qrCharList[QRMODE_KANJI].clear();

}

bool inputanalyzer::isCharacterOnList(std::list<int> clist, int urlLength) {

    int sum = 0;
    std::list<int>::iterator itr;
    for (itr=clist.begin(); itr!=clist.end(); itr++) {
        sum += m_counter[*itr];
    }    
    return sum == urlLength;

}


QRMode inputanalyzer::analyze(char *url) {
    int i=0;
    int len = strlen(url);
    memset(m_counter, 0, MAX_NUM_CHARACTER*sizeof(int));
    
    for (i=0; i<len; i++) m_counter[(int)url[i]]++;
    
    for (i=QRMODE_NUMERIC; i<QRMODE_MAX; i++) {
         if (!m_qrCharList[i].empty() && isCharacterOnList(m_qrCharList[i], len) == true)
            break;
    }
    return (QRMode) i;
}
