#ifndef libppm
#define libppm

/** @file 
* C S Cowden   12/18/2017
*
* Library to help read/write simple image files
* PPM bitmap format.
*/

#include <cstdio>

/**
* read a PPM file
* return the image and parameters through pointers.
*/
void readPPM(const char * filenm, unsigned * width, unsigned * height, unsigned ** map);



/**
* Types of pixmap formats
* | type | ascii | binary | colors |
* |------|-------|--------|--------|
* |bitmap| P1    | P4     | 0-1    |
* |greymap| P2   | P5     | 0-255  |
* |pixmap | P3   | P6     | 0-255  |
*/
enum format {
  P1,
  P2,
  P3,
  P4,
  P5,
  P6
};


/**
* read the header from an open
* PPM file, and return the width and height.
*/
void readHeader(FILE * file, unsigned * width, unsigned *height);

#endif
