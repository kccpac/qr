#include <iostream>

#include "stdio.h"
//#include "global.h"

#include "qrconst.h"
#include "global.h"
#include "inputanalyzer.h"
#include "ssconverter.h"
#include "string.h"
#include "iffmpeg.h"
#include "qrimage.h"
#include "qrencode.h"

qrencode::qrencode() {
    m_ffmpeg = new iffmpeg[1];
}

qrencode::~qrencode() {
    delete m_qrparam;
    delete [] m_ffmpeg;
}

qrencode::qrencode(char *url, QRMode qrMode, ECLevel ecLevel) : qrencode() {

    int len = strlen(url);
    m_qrparam = new qrparam(qrMode, ecLevel, len);

 //   erdata_capacity[qrMode][NUM_ECLEVEL][NUM_VERSION] 
}

qrsymbol  qrencode::charsToSymbol(QRMode qrMode, char *url) {
    int numCharPerSymbol = m_qrparam->get_NumCharPerQRSymbol();
    int wholeSymbolSize = m_qrparam->get_InputLen()/numCharPerSymbol;
    bool lastSymbol = (m_qrparam->get_InputLen()%numCharPerSymbol == 0) ? 0: 1;
//    int qrSymbolSize = urlLen/numCharPerSymbol + (urlLen%numCharPerSymbol == 0) ? 0: 1;
    char tmpChars[numCharPerSymbol+1];// = new char [numCharPerSymbol];
    qrsymbol qrs;
    qrs.codewords = new int [wholeSymbolSize+lastSymbol];
    qrs.len = wholeSymbolSize+lastSymbol;
//    int *qrSymbols = new int [wholeSymbolSize+lastSymbol];
    int *intptr = qrs.codewords;
    char *ptr = url;
 
    int i=0;
 
    std::cout <<  "m_qrparam->get_InputLen() = " << m_qrparam->get_InputLen() <<"\n";
    std::cout << "numCharPerSymbol =  "<<numCharPerSymbol << "\n";
    std::cout << "wholeSymbolSize = " <<wholeSymbolSize << "\n";
    std::cout << "lastSymbol = " << m_qrparam->get_InputLen()%numCharPerSymbol << "(" << lastSymbol << ")\n";

    memset(tmpChars, 0, numCharPerSymbol+1);
    switch (qrMode) {
        case QRMODE_NUMERIC: // subset of ALPHANUMERIC
            for (i=0; i<wholeSymbolSize; i++) {
                memcpy(tmpChars, ptr, numCharPerSymbol);
                *intptr = atoi(tmpChars);
                ptr+=numCharPerSymbol; intptr++;
            }  
            if (lastSymbol) {
                memset(tmpChars, 0, numCharPerSymbol);
                memcpy(tmpChars, ptr, m_qrparam->get_InputLen()%numCharPerSymbol);
                *intptr = atoi(tmpChars);
            }                
            break;
        case QRMODE_ALPHANUMERIC: // subset of JS_EIGHT
           for (i=0; i<wholeSymbolSize; i++) {
                memcpy(tmpChars, ptr, numCharPerSymbol);
  //              printf( "tmpChars[0] = %d (%c)  CHAR_TO_ALPHANUMERIC_SYM_LIST[tmpChars[0]] %d \n " , tmpChars[0], tmpChars[0] ,CHAR_TO_ALPHANUMERIC_SYM_LIST[tmpChars[0]]);
  //              printf( "tmpChars[1] = %d (%c)  CHAR_TO_ALPHANUMERIC_SYM_LIST[tmpChars[1]] %d \n " , (int)tmpChars[1],  (int)tmpChars[1] ,CHAR_TO_ALPHANUMERIC_SYM_LIST[(int)tmpChars[1]]);
                
                *intptr = CHAR_TO_ALPHANUMERIC_SYM_LIST[(int)tmpChars[0]]*ALPHANUMERIC_CHAR_SIZE+
                               CHAR_TO_ALPHANUMERIC_SYM_LIST[(int)tmpChars[1]];
                ptr+=numCharPerSymbol; intptr++;
            }  
            if (lastSymbol) {
                memset(tmpChars, 0, numCharPerSymbol);
                memcpy(tmpChars, ptr, m_qrparam->get_InputLen()%numCharPerSymbol);
                *intptr = atoi(tmpChars);
            }  
            break;
        case QRMODE_JS_EIGHT:
            for (i=0; i<wholeSymbolSize; i++) {
                memcpy(tmpChars, ptr, numCharPerSymbol);
                *intptr = (int) *tmpChars;
                intptr++; ptr++;
            }
            break;
        case QRMODE_KANJI:
    //        m_qrdata_size = -1;
            break;
        default:
            break;
    }
    
   for (i=0; i<wholeSymbolSize+lastSymbol; i++) {    
//      printf("qrSymbol[%d] = %d (%c) \n", i ,qrSymbols[i] , qrSymbols[i]);
        printf("qrSymbol[%d] = %d (%c) \n", i ,qrs.codewords[i] , qrs.codewords[i] );
   }
 //   delete []tmpChars;
    return qrs;//qrSymbols;
}

unsigned char * symbolToQRCodeword(qrsymbol symbols, QRMode qrMode) {
    return NULL;
}

bool qrencode::encode(qrsymbol symbols) {
    ssconverter bEncode(m_qrparam);
    printf("version: %d \n", m_qrparam->get_QRVersion());
    qrimage image(m_qrparam->get_QRVersion());
    printf("width  %d height %d \n",  image.getQRImageWidth(),  image.getQRImageHeight());
    m_ffmpeg->setup(image.getQRImageWidth(), image.getQRImageHeight(), image.getQRImagePitch());
    bEncode.write(&symbols);

    m_ffmpeg->encode(image.getImageData());
  //  m_ffmpeg->finish_encode();
    m_ffmpeg->save();
//    delete image;
    return true;
}
   
int main(int argv, char **argc)
{
    qrencode *qrencoder = NULL;    
    char url[] = {"AC-42"};//{"0123456789012345"};//{"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"};//{"1234567890"};
  //  qrparam param(url, ECLEVEL_L);
      inputanalyzer *analyzer = new inputanalyzer[1];

    QRMode qrMode = analyzer->analyze(url);
    std::cout << "NUM_QRMODE= " << qrMode << "\n";

 //   qrMode = QRMODE_JS_EIGHT;
    qrencoder = new qrencode(url, qrMode, ECLEVEL_L);
    
    qrsymbol symbols = qrencoder->charsToSymbol(qrMode, url);

    std::cout << " # symbol: " << symbols.len << "\n";
    qrencoder->encode(symbols);

//    delete qrSymbols;
/*     int i;
     
    for (i=0; i<ALPHANUMERIC_CHAR_SIZE; i++)
    {
        printf("%c(%x)\n ",  ALPHANUMERIC_CHAR_LIST[i], ALPHANUMERIC_CHAR_LIST[i]);
        }*/
  /*  
    int i;
    
    int xx[] = {
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0x24, 0xff, 0xff, 0xff, 0x25, 0x26, 0xff, 0xff,
0xff, 0xff, 0x28, 0x29, 0xff, 0x27, 0x2a, 0x2b, 0x0, 0x1, 0x2,
0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0x2c, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xa, 0xb, 0xc, 0xd, 0xe, 0xf, 
0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19,
0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff};

    std::cout << "xx = " << sizeof(xx)/sizeof(int) << "\n";*/
//    int tmp[256];
 //   memset(tmp, 0xFF, 256*sizeof(int));
//    int count = 0;
    
/*    for (i=0; i<ALPHANUMERIC_CHAR_SIZE; i++) {
        printf("ALPHANUMERIC_CHAR_LIST[%d] = (%c) %d \n", ALPHANUMERIC_CHAR_LIST[i], ALPHANUMERIC_CHAR_LIST[i], xx[(int)ALPHANUMERIC_CHAR_LIST[i]]);
    }
   */ 
   /*
 //   count = 0;
            for (i=0; i<256; i++) {
            tmp[i] = 0xFF;
            }
    do {
        std::cout <<  "To find : " << ALPHANUMERIC_CHAR_LIST[count] << " ";
        for (i=0; i<256; i++) {
 //           tmp[i] = 0xFF;
 //       for (count=0; count<ALPHANUMERIC_CHAR_SIZE; count++) {
            if (i==ALPHANUMERIC_CHAR_LIST[count]) {
                tmp[i] = count;
                count++;
            }
        }
    } while (count < ALPHANUMERIC_CHAR_SIZE);
    
     std::cout << "count: " << count << "\n";
    for (i=0; i<256; i++) {
    std::cout << std::hex << "0x" << tmp[i] << ", ";
    }
     std::cout << "\n";
   
  //  qrencoder.charToSymbol(url);
  */  
 /*   int i, j, k;
    std::cout << "NUM_QRMODE= " << NUM_QRMODE << " NUM_ECLEVEL=" <<NUM_ECLEVEL << " NUM_VERSION= " <<NUM_VERSION<< "\n";
     
    std::cout << "erdata_capacity = " << "\n";
    for (i=0; i<NUM_QRMODE; i++)
        for (j=0; j<NUM_ECLEVEL; j++)
            for (k=0; k<NUM_VERSION; k++) {
            std::cout << "i = " << i << " j = " << j << " k = " << k <<" = " << erdata_capacity[i][j][k] << "\n";
            }
    std::cout << "erdata_size = " << "\n";
    for (k=0; k<NUM_VERSION; k++)             
        for (j=0; j<NUM_ECLEVEL; j++)
            std::cout << "k = " << k << " j = " << j << " = " << erdata_size[k][j] << "\n";
    */        
    delete [] symbols.codewords;

    delete [] analyzer;
    if (qrencoder) delete qrencoder;

    return 0;
}
