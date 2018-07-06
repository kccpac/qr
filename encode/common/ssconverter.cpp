#include <stdio.h>
#include "qrconst.h"
#include "global.h"
#include "ssconverter.h"

void swapLittle(short test) { 
    char *ptr = (char*)&test;
    printf("swapLittle %d %d \n", ptr[0], ptr[1]);
}
void swapBig(short test) {
    char *ptr = (char*)&test;
    printf("swapBig %d %d \n", ptr[0], ptr[1]);
}


ssconverter::ssconverter(){}

ssconverter::~ssconverter() {
    delete [] m_data;
    delete [] bsFcn;
}

ssconverter::ssconverter(qrparam *param/*int qrDataSize, int cwSizeInbits*/) {    
    m_bLittleEndian = isLittleEndian();
    m_param = param;
    //  m_byteswap_fn = new byteSwapFcn();
    bsFcn = new byteswap[1];
    *bsFcn = (m_bLittleEndian) ? &swapLittle: &swapBig;
    short int test = 0x1;
    (*bsFcn)(test);
    m_qrdataSize = m_param->get_QRDataSize();
    m_data = new unsigned char [m_qrdataSize];
}

bool ssconverter::isLittleEndian() {
    short int number = 0x1;
    char *numPtr = (char*)&number;
    return (numPtr[0] == 1);
}

int ssconverter::writeheader() {
    return 0;
}

void ssconverter::generatePlacementMap(qrimage base_image) {
}


int ssconverter::write(qrsymbol *qrSymbols) {
    int i=0;
    long int cwbuf =qrSymbols->codewords[0];
    int cwInBits = m_param->get_QRSymbolSize();
    int bufpos = cwInBits;
    int half_cwbuf = 2*sizeof(long int);
    int dataMask;// = (1<<(cwInBits+1))-1;
    int dataToSubmit;
    
    // symbol -> codeword (add on padding codeword) -> stream -> qr image bytes
    for (i=1; i<qrSymbols->len-1; i++) {       
        cwbuf = (cwbuf << cwInBits) + qrSymbols->codewords[i]; 
        bufpos += cwInBits;
        if (bufpos > half_cwbuf) {  
            dataMask = (1<<(bufpos - half_cwbuf))-1;
            dataToSubmit = cwbuf >>(bufpos - half_cwbuf);  
            cwbuf = cwbuf & dataMask;       
            printf("dataMask %x cwbuf (%d) %x dataToSubmit %x bufpos %d half_cwbuf %d \n", dataMask,  i, cwbuf, dataToSubmit, bufpos, half_cwbuf); 
            writedata(dataToSubmit, sizeof(int));
            bufpos -=half_cwbuf;
        } 
    }
     printf("cwbuf (%d) %x \n ", i, cwbuf); 
    if (m_qrdataSize - bufpos > 0) {
        // last symbol
        cwbuf = (cwbuf << (m_qrdataSize - bufpos)) + qrSymbols->codewords[qrSymbols->len-1]; 
    }   
 //   writedata(0, 0);
    return 0;

}



int ssconverter::writedata(int dataToSubmit, int size) {
 //(*bsFcn)(&test);
 // (*bsFcn)((short*)&dataToSubmit);
    return 0;
}
