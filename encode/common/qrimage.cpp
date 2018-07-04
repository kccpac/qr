#include <iostream>
#include <memory.h>
#include "qrconst.h"
#include "qrimage.h"

 qrimage::qrimage(int version) {
        init(version);
}
         
qrimage::~qrimage() {
    if (m_qrBaseMemory) delete []m_qrBaseMemory;
    m_qrBaseMemory = NULL;
    
//   delete []m_pdp_location;
//   delete []m_vinfo_location;
/*    if (m_image) {
        for (int i=0; i<m_image_dim; i++) {
            delete  m_image[i];
            m_image[i] = NULL;
        }
        delete m_image;
        m_image = NULL;
    }
    delete mAlignPatternsPosition;
    mAlignPatternsPosition = NULL;*/
    
}

unsigned char * qrimage::generate() {
    return (unsigned char * ) NULL;
}   
inline int align16 (int value) {
    return ((value + 0xF) >> 4) << 4;
}
void qrimage::init(int version) {

    int total_qrBaseMemory_size = 0;
//    int num_align_pattern = 0;
    version = 2;
    std::cout << "version = " << version << "\n";
    if (version < 1 || version > 40) return;
    
    m_pixel_per_module = 1;
    m_version = version;
    m_image_dim = QR_SYMBOL_BASE_DIMENSION + m_version * VERSION_INCR_FACTOR;
    m_image_pitch = align16(m_image_dim);
    
    for (int i=0; i<32; i++) {
        m_BitsWordToInt[i] = 1<<i;
    }    
    
    m_num_align_pattern = AlignPatternsPosition[m_version][0];
    
    m_item_pointer.image_location = 0; // QR image 
    total_qrBaseMemory_size +=  m_image_pitch*m_image_pitch*sizeof(unsigned char);
    m_item_pointer.map_location = total_qrBaseMemory_size;
    total_qrBaseMemory_size +=  align16(m_image_pitch*m_image_pitch*sizeof(int));   
    m_item_pointer.PDPObject_location = total_qrBaseMemory_size;// Position Detect Pattern
    total_qrBaseMemory_size += align16(OUTER_POS_DETECT_PATTERN_SIZE*OUTER_POS_DETECT_PATTERN_SIZE*sizeof(unsigned char));
    m_item_pointer.APObject_location = total_qrBaseMemory_size;  // Alignment Pattern   
    total_qrBaseMemory_size += align16(OUTER_ALIGN_PATTERN_SIZE * OUTER_ALIGN_PATTERN_SIZE*sizeof(unsigned char));
    m_item_pointer.APPosition_location = total_qrBaseMemory_size;
    total_qrBaseMemory_size += align16(m_num_align_pattern * sizeof (struct patternInfo));//struct pos));
    m_item_pointer.pdp_location = total_qrBaseMemory_size;
    total_qrBaseMemory_size += align16(NUM_POS_DETECT_PATTERN * sizeof (struct patternInfo));   
    m_item_pointer.vinfo_location = total_qrBaseMemory_size;
    total_qrBaseMemory_size += align16(NUM_VINFO * sizeof (struct patternInfo));
    m_item_pointer.formatInfo_location = total_qrBaseMemory_size;
    total_qrBaseMemory_size += align16(NUM_FORMAT_INFO * sizeof (struct patternInfo));
    m_item_pointer.versionInfo_location = total_qrBaseMemory_size;
    total_qrBaseMemory_size += align16(BCH_VINFO_SIZE  * sizeof (unsigned char));
    
    printf("m_image_dim %d m_image_pitch %d \n",  m_image_dim,  m_image_pitch);
    printf("map_location %d PDPObject_location %d \n",  m_item_pointer.map_location,  m_item_pointer.PDPObject_location);
    printf("APObject_location %d APPosition_location %d \n",  m_item_pointer.APObject_location,  m_item_pointer.APPosition_location);
    printf("versionInfo_location %d pdp_location %d \n",   m_item_pointer.versionInfo_location,  m_item_pointer.pdp_location);
    printf("vinfo_location %d total_qrBaseMemory_size %d  \n", m_item_pointer.vinfo_location, total_qrBaseMemory_size);

    m_qrBaseMemory = new unsigned char [total_qrBaseMemory_size];
    memset(m_qrBaseMemory, 0, total_qrBaseMemory_size*sizeof(unsigned char));
    memset(m_qrBaseMemory+m_item_pointer.image_location, 255, m_image_pitch*m_image_pitch*sizeof(unsigned char));
    memset(m_qrBaseMemory+m_item_pointer.map_location, UNINITIALIZED, m_image_pitch*m_image_pitch*sizeof(int));
    
 //   int countPattern = 0;

    std::cout << "num_align_pattern = " << m_num_align_pattern << "\n";    

    m_pdp = (patternInfo *)  (m_qrBaseMemory + m_item_pointer.pdp_location);
    
    m_pdp[0].setStartpos({0, 0}) ;
    m_pdp[0].setWidth(OUTER_POS_DETECT_PATTERN_SIZE);  m_pdp[0].setHeight(OUTER_POS_DETECT_PATTERN_SIZE); // top left corner
   //  { m_height = height; } = new patternInfo(, OUTER_POS_DETECT_PATTERN_SIZE, OUTER_POS_DETECT_PATTERN_SIZE) ; 
    m_pdp[1].setStartpos({m_image_dim - OUTER_POS_DETECT_PATTERN_SIZE, 0});
    m_pdp[1].setWidth(OUTER_POS_DETECT_PATTERN_SIZE);  m_pdp[1].setHeight(OUTER_POS_DETECT_PATTERN_SIZE);
    //new patternInfo({m_image_dim - OUTER_POS_DETECT_PATTERN_SIZE, 0}, OUTER_POS_DETECT_PATTERN_SIZE, OUTER_POS_DETECT_PATTERN_SIZE) ;  // top right corner
    m_pdp[2].setStartpos({0, m_image_dim - OUTER_POS_DETECT_PATTERN_SIZE}) ; 
    m_pdp[2].setWidth(OUTER_POS_DETECT_PATTERN_SIZE);  m_pdp[2].setHeight(OUTER_POS_DETECT_PATTERN_SIZE);
    
    // new patternInfo({0, m_image_dim - OUTER_POS_DETECT_PATTERN_SIZE}, OUTER_POS_DETECT_PATTERN_SIZE, OUTER_POS_DETECT_PATTERN_SIZE) ;  // bottom left corner
    
    m_vinfo = (patternInfo *)  (m_qrBaseMemory + m_item_pointer.vinfo_location);
    
    m_vinfo[0].setStartpos({m_image_dim - OUTER_POS_DETECT_PATTERN_SIZE - 1 - BCH_VINFO_SIZE/BCH_VERT_VINFO_ROW, 0});
    m_vinfo[0].setWidth(BCH_VINFO_SIZE/BCH_VERT_VINFO_ROW); m_vinfo[0].setHeight(BCH_VERT_VINFO_ROW);
    m_vinfo[1].setStartpos({0, m_image_dim - OUTER_POS_DETECT_PATTERN_SIZE - 1 - BCH_HORZ_VINFO_ROW});
    m_vinfo[1].setWidth(BCH_VINFO_SIZE/BCH_HORZ_VINFO_ROW); m_vinfo[1].setHeight(BCH_HORZ_VINFO_ROW);
    
    m_finfo = (patternInfo *) (m_qrBaseMemory + m_item_pointer.formatInfo_location);
    
    int strip_size = (FORMAT_INFO_SIZE+1)/2;
    m_finfo[0].setStartpos({BASE_FINFO_LOCATION, 0});
    m_finfo[0].setWidth(HORZ_FINFO_STRIP); m_finfo[0].setHeight(strip_size);
    m_finfo[1].setStartpos({0, BASE_FINFO_LOCATION});
    m_finfo[1].setWidth(strip_size); m_finfo[1].setHeight(HORZ_FINFO_STRIP);
    m_finfo[2].setStartpos({BASE_FINFO_LOCATION,  m_image_dim -BASE_FINFO_LOCATION});
    m_finfo[2].setWidth(HORZ_FINFO_STRIP); m_finfo[2].setHeight(strip_size);
    m_finfo[3].setStartpos({m_image_pitch -BASE_FINFO_LOCATION, m_image_dim});
    m_finfo[3].setWidth(strip_size); m_finfo[3].setHeight(HORZ_FINFO_STRIP);

    init_pattern_object(m_item_pointer.PDPObject_location,
                OUTER_POS_DETECT_PATTERN_SIZE,
                WHITE_POS_DETECT_PATTERN_SIZE,
                INNER_POS_DETECT_PATTERN_SIZE);
    init_pattern_object(m_item_pointer.APObject_location,
                OUTER_ALIGN_PATTERN_SIZE,
                WHITE_ALIGN_PATTERN_SIZE,
                INNER_ALIGN_PATTERN_SIZE);
    if (m_num_align_pattern > 0) init_app_object();
    init_map_object();
    
    create_empty_image();
    
    printf("print empty image starts \n");
/*    
    unsigned char *image_ptr =  (unsigned char *) m_qrBaseMemory +m_item_pointer.image_location;
    for (int i=0; i<m_image_dim/2; i++) {
        for (int j=0; j<m_image_dim/2; j++)
            printf("%d ", *(image_ptr + i*m_image_pitch + j));
        printf("\n");
    } 
    printf("print empty image ends \n"); */
}  

void qrimage::init_app_object() {

    struct pos bpos = {(OUTER_ALIGN_PATTERN_SIZE-INNER_ALIGN_PATTERN_SIZE)/2, 
                                 (OUTER_ALIGN_PATTERN_SIZE-INNER_ALIGN_PATTERN_SIZE)/2};
    int *positions = AlignPatternsPosition[m_version];
    int countPattern = 0;
    int i, j;
        
        
//    mAlignPatternsPosition =  (struct pos *) (m_qrBaseMemory + m_item_pointer.APPosition_location) ;
     mAlignPatterns = (patternInfo *) (m_qrBaseMemory + m_item_pointer.APPosition_location) ;
    
    for (i=MAX_APP_COMBINATION; i>=1; i--)  {
         if (positions[i] != -1) break;
    }
    int num_ap_dim = i;
//    patternInfo *ptr;
    for (i=1; i<num_ap_dim + 1; i++) {
        if (positions[i]== -1) continue;
        for (j=1; j<num_ap_dim + 1; j++) {
            if (positions[j]==-1 || (i == 1 && j == 1) || (i==1 && j == num_ap_dim) ||  (i==num_ap_dim && j == 1 ) ) continue;
            //ptr = &mAlignPatterns [countPattern]; 
            mAlignPatterns[countPattern].setStartpos({positions[i] - bpos.x, positions[j] - bpos.y});
            mAlignPatterns[countPattern].setWidth(OUTER_ALIGN_PATTERN_SIZE); mAlignPatterns [countPattern].setHeight(OUTER_ALIGN_PATTERN_SIZE);
           // mAlignPatternsPosition[countPattern].x =positions[i] - bpos.x;
            //mAlignPatternsPosition[countPattern].y =positions[j] - bpos.y;
 //           std::cout << "pos (" <<i << ", " << j << ") = (" << positions[i] << "," << positions[j] << ") \n" ;   
            countPattern++;     
        }
    }
    std::cout  << "total # APP pattern = " << countPattern << "\n";
}

void qrimage::init_map_object() {

    int *map_ptr = (int *) (m_qrBaseMemory+m_item_pointer.map_location);
//    unsigned char *image_ptr = m_qrBaseMemory + m_item_pointer.image_location;
    int i, j, k;
    int pattern_size;
//    unsigned char *sub_ptr = NULL;
    int *sub_map_ptr = NULL;
    struct pos p ;

// Format Info
    int size = (FORMAT_INFO_SIZE+1)/2;
//    memset(map_ptr +HORZ_FINFO_LOCATION*m_image_pitch, 0x0020, /*FORMAT_INFO,*/ size*sizeof(int));
//    memset(map_ptr +(HORZ_FINFO_LOCATION+1)*m_image_pitch-size, FORMAT_INFO, size*sizeof(int));
    for (j=0; j<size; j++) {
        //memset(map_ptr +j*m_image_pitch+VERT_FINFO_LOCATION, 0x20, /*FORMAT_INFO,*/ sizeof(int));
        #define HORZ_FINFO_LOCATION BASE_FINFO_LOCATION
        *(map_ptr + HORZ_FINFO_LOCATION*m_image_pitch + j) = 
        *(map_ptr +HORZ_FINFO_LOCATION*m_image_pitch+m_image_dim-j-1) =
        *(map_ptr +(m_image_dim-j+1)*m_image_pitch+VERT_FINFO_LOCATION) = 
        *(map_ptr +j*m_image_pitch+VERT_FINFO_LOCATION) = FORMAT_INFO;
 //        printf("## HORZ_FINFO_LOCATION %d %d %d \n", j, (HORZ_FINFO_LOCATION+1)*m_image_pitch-j-1, map_ptr[(HORZ_FINFO_LOCATION+1)*m_image_pitch-j-1] );
 //        printf("HORZ_FINFO_LOCATION*m_image_pitch + j %d \n", HORZ_FINFO_LOCATION*m_image_pitch + j);
      //  memset(map_ptr +(m_image_dim-j+1)*m_image_pitch+VERT_FINFO_LOCATION,  0x20, /*FORMAT_INFO,*/ sizeof(int));
    }
   /* 
     printf("print empty map start \n");
    printf("HORZ_FINFO_LOCATION %d %d m_image_dim %d\n", HORZ_FINFO_LOCATION, HORZ_FINFO_LOCATION*m_image_pitch, m_image_dim);
 //   image_ptr =  (unsigned char *) m_qrBaseMemory +m_item_pointer.image_location;
    for (i=0; i<m_image_dim; i++) {
        for (j=0; j<m_image_pitch; j++)
            printf("%d ",  *(map_ptr + i*m_image_pitch + j));
        printf("\n");
    } 
    printf("print empty image ends \n");
*/

  //  Timing pattern
  //  memset(map_ptr + VERT_TIMING_PATTERN_LOCATION*m_image_pitch, TIMING, m_image_pitch*sizeof(int));
    for (j=BASE_FINFO_LOCATION; j<m_image_dim-BASE_FINFO_LOCATION; j++) {
        *(map_ptr + VERT_TIMING_PATTERN_LOCATION*m_image_pitch +j) = 
        *(map_ptr +j*m_image_pitch + HORZ_TIMING_PATTERN_LOCATION) = TIMING; 
        //map_ptr + VERT_TIMING_PATTERN_LOCATION*m_image_pitch, TIMING, m_image_pitch*sizeof(int)
    //    memset(map_ptr +j*m_image_pitch, TIMING, sizeof(int));
    }
    /*
         printf("print empty map start \n");
 //   printf("HORZ_FINFO_LOCATION %d %d m_image_dim %d\n", HORZ_FINFO_LOCATION, HORZ_FINFO_LOCATION*m_image_pitch, m_image_dim);
 //   image_ptr =  (unsigned char *) m_qrBaseMemory +m_item_pointer.image_location;
    for (i=0; i<m_image_dim; i++) {
        for (j=0; j<m_image_pitch; j++)
            printf("%d ",  *(map_ptr + i*m_image_pitch + j));
        printf("\n");
    } 
    printf("print empty image ends \n");
*/
    
    pattern_size = AlignPatternsPosition[m_version][0];
    size = OUTER_ALIGN_PATTERN_SIZE * sizeof(int);
    for (i=0; i<pattern_size; i++) {
        p =  mAlignPatterns[i].getStartpos();//Position[i];
        sub_map_ptr = map_ptr+ p.y*m_image_pitch + p.x;
        for (int j=0; j<OUTER_POS_DETECT_PATTERN_SIZE; j++) {
            for (int k=0; k<OUTER_ALIGN_PATTERN_SIZE; k++) 
                *(sub_map_ptr +j*m_image_pitch+k) = ALIGNMENT;
        //sub_map_ptr +j*m_image_pitch
//           memset(sub_map_ptr +j*m_image_pitch, atoi("-4"),/* ALIGNMENT,*/ size);
        }
    }

    pattern_size = NUM_POS_DETECT_PATTERN;
    size = OUTER_POS_DETECT_PATTERN_SIZE * sizeof(int);
    for (i=0; i<pattern_size; i++) {
        p = m_pdp[i].getStartpos();
        sub_map_ptr = map_ptr + p.y*m_image_pitch + p.x;
        for (j=0; j<OUTER_POS_DETECT_PATTERN_SIZE; j++) {
            for (k=0; k<OUTER_POS_DETECT_PATTERN_SIZE; k++) 
                *(sub_map_ptr +j*m_image_pitch+k) = ALIGNMENT;
//            memset(sub_map_ptr +j*m_image_pitch, POSITION_DETECTION,  size);
        }
    }
    
    pattern_size = NUM_VINFO;
   // int rows[] = {BCH_HORZ_VINFO_ROW, BCH_VERT_VINFO_ROW};
  //  size = BCH_VINFO_SIZE * sizeof(int);
    for (i=0; i<pattern_size; i++) {
        p = m_vinfo[i].getStartpos();
        sub_map_ptr = map_ptr + p.y*m_image_pitch + p.x;
        for (j=0; j<m_vinfo[i].getHeight(); j++) {
            for (k=0; k<m_vinfo[i].getWidth(); k++) 
                *(sub_map_ptr +j*m_image_pitch+k) = VERSION_INFO;
      //      memset(sub_map_ptr +j*m_image_pitch, VERSION_INFO, m_vinfo[i].getWidth()*sizeof(int));
        }
    }
  
    printf("print empty map start \n");
 //   printf("HORZ_FINFO_LOCATION %d %d m_image_dim %d\n", HORZ_FINFO_LOCATION, HORZ_FINFO_LOCATION*m_image_pitch, m_image_dim);
 //   image_ptr =  (unsigned char *) m_qrBaseMemory +m_item_pointer.image_location;
    for (i=0; i<m_image_dim; i++) {
        for (j=0; j<m_image_dim; j++)
            printf("%d ",  *(map_ptr + i*m_image_pitch + j));
        printf("\n");
    } 
    printf("print empty image ends \n");


/*    
    pattern_size = NUM_VINFO;
//    int rows[] = {BCH_HORZ_VINFO_ROW, BCH_VERT_VINFO_ROW};
    for (i=0; i<pattern_size; i++) {
        p = m_vinfo_location[i];
        sub_ptr = image_ptr + p.y*m_image_pitch + p.x;
        for (j=0; j<rows[i]; j++) {
            memset(sub_ptr +j*m_image_pitch, QRPATTERN.POSITION_DETECTION, BCH_VINFO_SIZE/rows[i]);
        }
    }*/
}

void qrimage::init_pattern_object(int pattern_location, int pitch, int inner_white_size, int inner_dark_size) {

    unsigned char *ptr = m_qrBaseMemory + pattern_location;
    int inner_white_delta = (pitch - inner_white_size) >> 1; 
    int inner_dark_delta = (pitch - inner_dark_size) >> 1; 

    unsigned char *white_pattern_ptr = ptr  + inner_white_delta*pitch + inner_white_delta;
    unsigned char *inner_dark_pattern_ptr = ptr + inner_dark_delta*pitch  + inner_dark_delta;
    int i;

    // draw the white pattern
    for (i=0; i<inner_white_size; i++) {
        memset(white_pattern_ptr + i*pitch, 255,  inner_white_size);
    }    
        
    // draw the inner dark pattern
    for (i=0; i<inner_dark_size; i++) {
        memset(inner_dark_pattern_ptr+i*pitch, 0, inner_dark_size);
    }
    
/*    printf("pattern_location %d\n", pattern_location);
    printf("pitch %d inner_white_delta %d inner_dark_delta %d \n", pitch, inner_white_delta, inner_dark_delta);
    for (i=0; i<pitch; i++) {
        for (int j=0; j<pitch; j++)
            printf("%d ", *(ptr+i*pitch+j));
        printf("\n");
    } */
}

void qrimage::copy_pattern_objects(int pattern_location, patternInfo *pdp, int pattern_size) {

    unsigned char *image_ptr = m_qrBaseMemory + m_item_pointer.image_location;
    unsigned char *object_ptr = m_qrBaseMemory + pattern_location;
    unsigned char *sub_image_ptr = NULL;
    struct pos p;
    int width, height;
    for (int i=0; i<pattern_size; i++) {
        p = pdp[i].getStartpos();
        sub_image_ptr = image_ptr + p.y*m_image_pitch + p.x;
        width = pdp[i].getWidth();  height = pdp[i].getHeight(); 
        printf("pdp[%d].getWidth() = %d pdp[%d].getHeight() = %d \n", i, pdp[i].getWidth(), i, pdp[i].getHeight());
        for (int j=0; j<pdp[i].getHeight(); j++) {
            memcpy(sub_image_ptr +j*m_image_pitch, object_ptr + j*width, width);
        }
    }
}

void qrimage::create_empty_image() {

   // copy_pattern(m_item_pointer.APObject_location, mAlignPatternsPosition,  AlignPatternsPosition[m_version][0], OUTER_ALIGN_PATTERN_SIZE);
  //  copy_pattern(m_item_pointer.PDPObject_location, m_pdp,  NUM_POS_DETECT_PATTERN, OUTER_POS_DETECT_PATTERN_SIZE);
    int encoded_version_info = BCH_VERSION_INFO[m_version];
    unsigned char *obj;
    unsigned char *image_ptr;
    int *intptr;
    int *map_ptr;
    int i, j, k;
    
    copy_pattern_objects(m_item_pointer.APObject_location, mAlignPatterns,  AlignPatternsPosition[m_version][0]);
    
    printf("copy PDPObject start \n");
    copy_pattern_objects(m_item_pointer.PDPObject_location, m_pdp,  NUM_POS_DETECT_PATTERN);
    printf("copy PDPObject end \n");


    for (i=0; i<NUM_VINFO; i++) {
        struct pos p = m_vinfo[i].getStartpos();        
        int bitCount = 0;
        image_ptr = m_qrBaseMemory + m_item_pointer.image_location; 
        
        printf("version info pos (%d, %d) m_vinfo[i].getHeight() %d m_vinfo[i].getWidth() %d \n", p.x, p.y, m_vinfo[i].getHeight(), m_vinfo[i].getWidth());
        for (j=0; j<m_vinfo[i].getHeight(); j++) {
            obj = image_ptr + (p.y+j)*m_image_pitch + p.x;
            for (k=0; k<m_vinfo[i].getWidth(); k++) {
                obj[k] = (encoded_version_info & m_BitsWordToInt[bitCount]) >> bitCount;
                bitCount++;
            }
 //           obj += m_image_pitch;
        }
    }
    

    // test format info
    // Assume Error Correction Level M: 00
    // Mask Pattern Reference: 101
    // Data: 00101
    //BCH bits: 0011011100
    // Unmasked bit sequence: 001010011011100
    // Mask pattern for XOR operation: 101010000010010
    // Format Information module pattern: 100000011001110
    
    int encoded_format_info = 0x40CE;
    
    for (i=0; i<NUM_FORMAT_INFO; i++) {
        struct pos p = m_finfo[i].getStartpos();
        int bitCount = 0;
        intptr =  ((int *)(m_qrBaseMemory + m_item_pointer.map_location)) + p.y*m_image_pitch + p.x;;
        
        printf("pos (%d, %d) m_vinfo[%d].getHeight() = %d , m_vinfo[%d].getWidth() = %d \n", p.x, p.y, i, m_vinfo[i].getHeight(), i, m_vinfo[i].getWidth());
        for (j=0; j<m_vinfo[i].getHeight(); j++) {
           // obj += j*m_image_pitch; obj +=  j*m_image_pitch; 
           obj =m_qrBaseMemory + m_item_pointer.image_location + (p.y+j)*m_image_pitch + p.x;
            for (k=0; k<m_vinfo[i].getWidth(); k++) {
                if (intptr[k] == FORMAT_INFO) 
                { 
                    obj[k] = (encoded_format_info & m_BitsWordToInt[bitCount]) >> bitCount;
                }
                bitCount++;
            }
          //   obj += m_image_pitch;
        }
    }
    /*
    printf("===print empty image start \n");
    image_ptr =  (unsigned char *) m_qrBaseMemory +m_item_pointer.image_location;
    for (i=0; i<m_image_dim; i++) {
        for (j=0; j<m_image_dim; j++)
            printf("%d ", *(image_ptr + i*m_image_pitch + j));
        printf("\n");
    } 
    printf("print empty image ends \n");
    */

    

            
 //       unsigned char *vinfo = m_qrBaseMemory + VIObject_location[i];
  //  }
    // get a mask qr image to store the location of written pixels (this used to do bit test)
    // verify if the unwritten pixel = pre-computed qr symbol size
    
    // upper right
    
/*    int offset = (OUTER_ALIGN_PATTERN_SIZE - 1) >> 1;
    unsigned char *image_ptr = m_qrBaseMemory + m_item_pointer.image_location;
    unsigned char *align_pattern_ptr = m_qrBaseMemory +m_item_pointer.APObject_location;
    for (int i=0; i<num_align_pattern; i++) {
        struct pos p = mAlignPatternsPosition[i];
        p.x -= offset; p.y -= offset;
        unsigned char *pattern_ptr = image_ptr + p.y*m_image_pitch + p.x;
        for (int j=0; j<OUTER_ALIGN_PATTERN_SIZE; j++) {
            memcpy(pattern_ptr +j*m_image_pitch, align_pattern_ptr + j*OUTER_ALIGN_PATTERN_SIZE, OUTER_ALIGN_PATTERN_SIZE);
        }
    }
   */         
}
/*
void qrimage::init_PDPObject() {

    unsigned char *ptr = m_qrBaseMemory + m_item_pointer.PDPObject_location;
    int pitch = OUTER_POS_DETECT_PATTERN_SIZE;
    int inner_white_delta = (OUTER_POS_DETECT_PATTERN_SIZE - WHITE_POS_DETECT_PATTERN_SIZE) >> 1; // same to inner dark pattern
    int inner_dark_delta = (OUTER_POS_DETECT_PATTERN_SIZE - INNER_POS_DETECT_PATTERN_SIZE) >> 1; // same to inner dark pattern

    unsigned char *white_pattern_ptr = ptr + inner_white_delta + inner_white_delta*pitch;
    unsigned char *inner_dark_pattern_ptr = ptr + inner_dark_delta + inner_dark_delta*pitch;
    int i;

    // draw the inner middle white pattern
    for (i=0; i<OUTER_POS_DETECT_PATTERN_SIZE; i++) {
        memset(white_pattern_ptr + i*pitch, 255,  OUTER_POS_DETECT_PATTERN_SIZE);
    }
        
    // draw the inner middle white pattern
    for (i=0; i<INNER_POS_DETECT_PATTERN_SIZE; i++) {
        memset(inner_dark_pattern_ptr+i*pitch, 0, INNER_POS_DETECT_PATTERN_SIZE);
    }
//    #define INNER_POS_DETECT_PATTERN_SIZE 3 // in black pixel
//#define WHITE_POS_DETECT_PATTERN_SIZE 5 // in white pixel
//#define OUTER_POS_DETECT_PATTERN_SIZE 7
}

void qrimage::init_APObject() {

    unsigned char *ptr = m_qrBaseMemory + m_item_pointer.APObject_location;
    int pitch = OUTER_ALIGN_PATTERN_SIZE;
    int inner_white_delta = (OUTER_ALIGN_PATTERN_SIZE - WHITE_ALIGN_PATTERN_SIZE) >> 1; // same to inner dark pattern
    int inner_data_delta = (OUTER_ALIGN_PATTERN_SIZE - INNER_ALIGN_PATTERN_SIZE) >> 1; // same to inner dark pattern

    unsigned char *white_pattern_ptr = ptr + inner_white_delta + inner_white_delta*pitch;
    unsigned char *inner_dark_pattern_ptr = ptr + delta + delta*pitch;
    int i;

    // draw the inner middle white pattern
    for (i=0; i<OUTER_ALIGN_PATTERN_SIZE; i++) {
        memset(white_pattern_ptr + i*pitch, 255,  OUTER_ALIGN_PATTERN_SIZE);
    }
        
    // draw the inner middle white pattern
    for (i=0; i<INNER_ALIGN_PATTERN_SIZE; i++) {
        memset(inner_dark_pattern_ptr+i*ptch, 0, INNER_ALIGN_PATTERN_SIZE);
    }
//#define INNER_ALIGN_PATTERN_SIZE 1
//#define OUTER_ALIGN_PATTERN_SIZE 5
}
*/
 
void qrimage::addPattern(QRPATTERN pattern) {
}     
   

