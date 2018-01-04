

#include "libppm.h"

#include <iostream>
#include <cstring>
#include <cassert>


void readHeader(FILE * file, unsigned * width, unsigned * height ) {

  // The first two characters in the header correspond to the format
  fseek(file,0,SEEK_SET);
  char buffer[500];
  unsigned loc = 0U;
  fread(buffer,1,2,file);
  
  // If the first two characters are not a valid format, throw an error
  format frmt;
  if ( strcmp(buffer,"P1") ) frmt = P1;
  else if ( !strcmp(buffer,"P2") ) frmt = P2;
  else if ( !strcmp(buffer,"P3") ) frmt = P3;
  else if ( !strcmp(buffer,"P4") ) frmt = P4;
  else if ( !strcmp(buffer,"P5") ) frmt = P5;
  else if ( !strcmp(buffer,"P6") ) frmt = P6;
  else {
    // raise some error
    assert(false);
  } 
  
  // read to the first new line
  char sb[1];
  fread(sb,1,1,file);
  while ( sb[0] != '\n' ) {
    fread(sb,1,1,file);
  }

  unsigned loc1 =  ftell(file);
  // while line does not start with a # (comment)
  bool hascomments = false;
  fread(sb,1,1,file);
  while ( sb[0] == '#' ) {
    hascomments = true;
    fread(sb,1,1,file);
    while ( sb[0] != '\n' ) {
      fread(sb,1,1,file);
    }
  }

  // fill string with next line
  loc = ftell(file);
  if ( !hascomments ) loc = loc1;
  fread(sb,1,1,file);
  while ( sb[0] != '\n' ) {
    fread(sb,1,1,file);
  }
  unsigned bend = ftell(file);
  fseek(file,loc,SEEK_SET);
  assert(bend-loc < 500);
  fread(buffer,1,bend-loc,file);

  unsigned pos = 0U;
  char cwidth[50];
  char cheight[50];
  // separate by spaces
  while ( buffer[pos] != ' ' ) {
    cwidth[pos] = buffer[pos];
    pos++;
  }
  // seek next non-whitespace in buffer
  while ( buffer[pos] == ' ' ) {
    pos++;
    assert(pos < bend-loc);
  }

  unsigned start = pos;
  while ( buffer[pos] != ' ' && buffer[pos] != '\n' ) {
    cheight[pos-start] = buffer[pos];
    pos++;
  }
  
  // extract the dimensions
  *width = atoi(cwidth);
  *height = atoi(cheight);
  
  // return 
}


void readPPM(const char *filenm, unsigned * width, unsigned * height, unsigned ** map) {

  // open the file to read
  FILE * file = fopen(filenm,"rb");

  char buffer[15];

  // read the header of the file (15bytes)
  /*fread(buffer,1,15,file);

  char cwidth[4] = {0};
  for ( unsigned i=0; i != 3; i++ ) 
    cwidth[i] = buffer[3+i];

  char cheight[4] = {0};
  for ( unsigned i=0; i != 3; i++ )
    cheight[i] = buffer[7+i];

   *width = atoi(cwidth);
   *height = atoi(cheight);*/

  readHeader(file,width,height);

  const unsigned w = *width;
  const unsigned h = *height;
  const unsigned size = w*h;

  const unsigned offset = ftell(file); // was 16

  // initialize the map data
  *map = new unsigned[size];
  for ( unsigned i =0 ; i != size; i++ ) 
    (*map)[i] = 0U;


  // determine file size (in bytes)
  // read all of the file into memory
  // and then parse into the map 
  fseek(file,0,SEEK_END);
  unsigned len = ftell(file);
  fseek(file,0,SEEK_SET);
  char * buf = new char[sizeof(char)*len];

  fread(buf,1,len,file);

  for ( unsigned i=0; i != size; i++ ) {
    (*map)[i] = (unsigned)buf[offset+3*i];
  }

  fclose(file);

  // clean up memory
  delete buf;

}
