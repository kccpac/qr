#ifndef _QRIMAGE_H
#define _QRIMAGE_H

#include "qrconst.h"
#include "patternInfo.h"

enum QRPATTERN {
    UNKNOWN = -1,
    POSITION_DETECTION = -2,
    ALIGNMENT = -4,
    TIMING = -8,
    VERSION_INFO = -16,
    FORMAT_INFO = -32
};


/*struct patternInfo {
    pos start_pos;
    int width;
    int height;
}*/

struct qrpointer {
    int image_location; // QR image 
    int map_location; // QR codeward map
    int PDPObject_location; // Position Detect Pattern
    int APObject_location;  // Alignment Pattern    
    int APPosition_location; // location of Alignment Pattern 
    int versionInfo_location;
    int pdp_location;
    int vinfo_location;
    int formatInfo_location;
 //   int VIObject_location[NUM_VINFO];
 //   int FObject_location[NUM_FORMAT_INFO];
};

class qrimage {
    public:
        qrimage();
        qrimage(int version);
        ~qrimage();
        unsigned char * generate();
        int* getMapData() { return  (int*) (m_qrBaseMemory+m_item_pointer.map_location); }
        unsigned char * getImageData() { return  (unsigned char*) (m_qrBaseMemory+m_item_pointer.image_location); }
        int getQRImageSize(){return m_image_pitch*m_image_dim;} 
        int getQRImagePitch() {return m_image_pitch;}
        int getQRImageWidth() {return m_image_dim;} 
        int getQRImageHeight() {return m_image_dim;} 

    private:
        void init(int version);   
        void addPattern(QRPATTERN pattern);
        void init_app_object();
        void init_map_object();
        void init_pattern_object(int pattern_location, int pitch, int inner_white_size, int inner_dark_size);
        void copy_pattern_objects(int pattern_location, patternInfo *pdp, int pattern_size);
        void create_empty_image();
        
        unsigned char *m_qrBaseMemory;
       // struct pos *mAlignPatternsPosition;    // center location
        //struct pos *m_pdp_location; // start location
        patternInfo *mAlignPatterns;
        patternInfo *m_pdp;
        int m_BitsWordToInt[32];
 //       struct pos *m_vinfo_location;
        patternInfo *m_vinfo;
        patternInfo *m_finfo;
        struct qrpointer m_item_pointer; 
        int m_pixel_per_module;
        int m_num_align_pattern;
        int m_image_dim;       
        int m_image_pitch;
        int m_version;        
};

#endif
