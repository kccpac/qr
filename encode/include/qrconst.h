#ifndef _QRCONST_H
#define _QRCONST_H

//#include "global.h"

#define UNINITIALIZED -1
#define QR_SYMBOL_BASE_DIMENSION 21

#define QUIET_ZONE_SIZE 4 // nominal reflectance value shall be equal to that of the light modules.

#define QR_IMAGE_BASE_DIMENSION  (QR_SYMBOL_BASE_DIMENSION + 2 * QUIET_ZONE_SIZE)

#define VERSION_INCR_FACTOR 4

#define NUM_POS_DETECT_PATTERN  3

// 7.3.2
#define INNER_POS_DETECT_PATTERN_SIZE 3 // in black pixel
#define WHITE_POS_DETECT_PATTERN_SIZE 5 // in white pixel
#define OUTER_POS_DETECT_PATTERN_SIZE 7

#define POS_DETECT_PATTERN_SEPARATOR_SIZE 1//A one-module wide Separator

// 7.3.5
#define INNER_ALIGN_PATTERN_SIZE 1
#define WHITE_ALIGN_PATTERN_SIZE 3
#define OUTER_ALIGN_PATTERN_SIZE 5

#define MAX_APP_COMBINATION 7

// Timing pattern
//The horizontal and vertical Timing Patterns respectively consist of a one module wide row or column of alternating
//dark and light modules, commencing and ending with a dark module. The horizontal Timing Pattern runs across
//row 6 of the symbol between the separators for the upper Position Detection Patterns; the vertical Timing Pattern
//similarly runs down column 6 of the symbol between the separators for the left-hand Position Detection Patterns.
//They enable the symbol density and version to be determined and provide datum positions for determining module
//coordinates.

#define BASE_TIMING_PATTERN_LOCATION 6
#define VERT_TIMING_PATTERN_LOCATION BASE_TIMING_PATTERN_LOCATION
#define HORZ_TIMING_PATTERN_LOCATION BASE_TIMING_PATTERN_LOCATION

// Format Information 
// - a 15 bit sequence containing 5 data bits, with 10 error correction bits
// - generator polynomial G(x) = x10 + x8 + x5 + x4 + x2 + x + 1 : BCH (15, 5)
// - 2 bits error correction level + 3 bits masking pattern
// The 15 bit error corrected Format Information shall then be XORed with the Mask Pattern 101010000010010, in
// order to ensure that no combination of Error Correction Level and Mask Pattern will result in an all-zero data string.
// Model 1 symbols to be autodiscriminated from Model 2 symbols

#define FORMAT_INFO_SIZE 15
#define NUM_FORMAT_INFO  4
#define BASE_FINFO_STRIP_SIZE 1
#define BASE_FINFO_LOCATION (OUTER_POS_DETECT_PATTERN_SIZE+POS_DETECT_PATTERN_SEPARATOR_SIZE)
#define HORZ_FINFO_LOCATION BASE_FINFO_LOCATION
#define VERT_FINFO_LOCATION BASE_FINFO_LOCATION
#define VERT_FINFO_STRIP BASE_FINFO_STRIP_SIZE
#define HORZ_FINFO_STRIP BASE_FINFO_STRIP_SIZE

// VERSION_INFO

#define NUM_VINFO  2
#define BCH_VINFO_SIZE 18 
#define BCH_VERT_VINFO_ROW 6 
#define BCH_HORZ_VINFO_ROW 3

#define NUM_QRMODE QRMODE_MAX + 1
#define NUM_ECLEVEL ECLEVEL_MAX + 1
#define NUM_VERSION 40

#define NUMERIC_CHAR_SIZE 10
#define ALPHANUMERIC_CHAR_SIZE 45
#define MAX_NUM_CHARACTER 256

#define NUMERIC_SYM_SIZE 10 // 8.3.2
#define ALPHANUMERIC_SYM_SIZE 11 // 8.3.3
#define JS_EIGHT_SYM_SIZE 8 // 8.3.4

#define CHAR_SIZE_PER_NUMERIC_SYM 3
#define CHAR_SIZE_PER_ALPHANUMERIC_SYM 2
#define CHAR_SIZE_PER_JS_EIGHT_SYM 1

enum QRMode{
    QRMODE_UNDEFINED = -1,
    QRMODE_NUMERIC = 0, // subset of ALPHANUMERIC
    QRMODE_ALPHANUMERIC = 1, // subset of JS_EIGHT
    QRMODE_JS_EIGHT = 2,
    QRMODE_KANJI = 3,    // 2 byte character set
    QRMODE_MAX = 3
};

enum PLACEMENT_ORIENTATION {
    UP_VERTICAL = 0,
    DOWN_VERTICAL = 1,
    UP_TO_DOWN = 2,
    DOWN_TO_UP = 3
 //   UP_HORIZONTAL = 2,
 //   DOWN_HORIZONTAL = 3
};

enum PLACEMENT_SHAPE {
    REGULAR,
    IRREULGAR // BYTE BY BYTE COPY
};

enum PLACEMENT_TYPE { // UPDATE THE MASK
    TIMING_PATTERN = -8,
    POS_DETECT_PATTERN = -4,
    ALIGN_PATTERN =  -2,
    INFORMATION = -1,
    DATA = 0
};

enum ECLevel {
    ECLEVEL_UNDEFINED = -1,
    ECLEVEL_L = 0,
    ECLEVEL_M = 1,
    ECLEVEL_Q = 2 ,
    ECLEVEL_H = 3 ,
    ECLEVEL_MAX = 4
};

enum QRModeIndicator {
    ECI = 0x0111,
    NUMERIC = 0x0001,
    ALPHANUMERIC = 0x0010,
    EIGHT_BIT_BYTE  = 0x0100,
    KANJI  = 0x1000,
    STRUCTURED_APPEND  = 0x0011,
    FNC1  = 0x01011001, //(First position -  4bits, 2nd position)
    END_OF_MSG = 0x0000, // Terminator
};

typedef struct qrsymbol {
        int *codewords;
        int len;
} qrsymbol;

#endif
