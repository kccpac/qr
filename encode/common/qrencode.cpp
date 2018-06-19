#include <iostream>
#include "global.h"
#include "inputanalyzer.h"
#include "qrencode.h"

qrencode::qrencode() {
}

qrencode::~qrencode() {
}

qrencode::qrencode(char *url, QRMode qrMode) : qrencode() {

}
    
int main(int argv, char **argc)
{
    qrencode *qrencoder = NULL;    
    char url[] = {"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"};//{"1234567890"};
  //  qrparam param(url, ECLEVEL_L);
      inputanalyzer *analyzer = new inputanalyzer();

    QRMode qrMode = analyzer->analyze(url);
    std::cout << "NUM_QRMODE= " <<qrMode << "\n";

    qrencoder = new qrencode(url, qrMode);
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
    delete analyzer;
    delete qrencoder;
    return 0;
}
