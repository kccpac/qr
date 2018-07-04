#ifndef _QRCONST_H
#define _QRCONST_H

#include "global.h"

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

//Version information bit stream for each version (from 7 to 40)
static int BCH_VERSION_INFO[] = { // 6 msg bits, 12 bit error bits
// No Version Information will result in an all-zero data string since only Versions 7 to 40 symbols contain the Version
// Information. Masking is not therefore applied to the Version Information.
// The Version Information areas are the 6x3 module block above the Timing Pattern and immediately to the left of
// the top right Position Detection Pattern Separator, and the 3x6 module block to the left of the Timing Pattern and
// immediately above the lower left Position Detection Pattern separator.
    0x07C94,
    0x085BC,
    0x09A99,
    0x0A4D3,
    0x0BBF6,
    0x0C762,
    0x0D847,
    0x0E60D,
    0x0F928,
    0x10B78,
    0x1145D,
    0x12A17,
    0x13532,
    0x149A6,
    0x15683,
    0x168C9,
    0x177EC,
    0x18EC4,
    0x191E1,
    0x1AFAB,
    0x1B08E,
    0x1CC1A,
    0x1D33F,
    0x1ED75,
    0x1F250,
    0x209D5,
    0x216F0,
    0x228BA,
    0x2379F,
    0x24B0B,
    0x2542E,
    0x26A64,
    0x27541,
    0x28C69
};

// Row/column coordinates of center module of Alignment Patterns
static int AlignPatternsPosition[NUM_VERSION+1][MAX_APP_COMBINATION + 1] = {
    {0, -1, -1, -1, -1, -1, -1, -1},
    {0, -1, -1, -1, -1, -1, -1, -1},
    {1, 6, 18, -1, -1, -1, -1, -1},
    {1, 6, 22, -1, -1, -1, -1, -1},
    {1, 6, 26, -1, -1, -1, -1, -1},
    {1, 6, 30, -1, -1, -1, -1, -1},
    {1, 6, 34, -1, -1, -1, -1, -1},
    {6, 6, 22, 38, -1, -1, -1, -1},
    {6, 6, 24, 42, -1, -1, -1, -1},
    {6, 6, 26, 46, -1, -1, -1, -1},
    {6, 6, 28, 50, -1, -1, -1, -1},
    {6, 6, 30, 54, -1, -1, -1, -1},
    {6, 6, 32, 58, -1, -1, -1, -1},
    {6, 6, 34, 62, -1, -1, -1, -1},
    {13, 6, 26, 46, 66, -1, -1, -1},
    {13, 6, 26, 48, 70, -1, -1, -1},
    {13, 6, 26, 50, 74, -1, -1, -1},
    {13, 6, 30, 54, 78, -1, -1, -1},
    {13, 6, 30, 56, 82, -1, -1, -1},
    {13, 6, 30, 58, 86, -1, -1, -1},
    {13, 6, 34, 62, 90, -1, -1, -1},
    {22, 6, 28, 50, 72, 94, -1, -1},
    {22, 6, 26, 50, 74, 98, -1, -1},
    {22, 6, 30, 54, 78, 102, -1, -1},
    {22, 6, 28, 54, 80, 106, -1, -1},
    {22, 6, 32, 58, 84, 110, -1, -1},
    {22, 6, 30, 58, 86, 114, -1, -1},
    {22, 6, 34, 62, 90, 118, -1, -1},
    {33, 6, 26, 50, 74, 98, 122, -1},
    {33, 6, 30, 54, 78, 102, 126, -1},
    {33, 6, 26, 52, 78, 104, 130, -1},
    {33, 6, 30, 56, 82, 108, 134, -1},
    {33, 6, 34, 60, 86, 112, 138, -1},
    {33, 6, 30, 58, 86, 114, 142, -1},
    {33, 6, 34, 62, 90, 118, 146, -1},
    {46, 6, 30, 54, 78, 102, 126, 150},
    {46, 6, 24, 50, 76, 102, 128, 154},
    {46, 6, 28, 54, 80, 106, 132, 158},
    {46, 6, 32, 58, 84, 110, 136, 162},
    {46, 6, 26, 54, 82, 110, 138, 166},
    {46, 6, 30, 58, 86, 114, 142, 170}
};

#endif
