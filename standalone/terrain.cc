///////////////////////////////////////////
// C S Cowden  December 18, 2017
// Standalone program to experiment and test the
// Turk's terrain analysis class (ARTIE).
//////////////////////////////////////////


#include "artie.h"
#include "libppm.h"

#include <iostream>

int main(int argc, char ** argv ) {

  // parse command line arguments
  if ( argc != 4 ) {
    std::cerr << "Must provide an input image to analyze." << std::endl;
    std::cout << "terrain <map.ppm> <output_directory> <base_name>" << std::endl;
    return 0;
  }

  const char * mapfile = argv[1];
  const char * output_dir = argv[2];
  const char * base_name = argv[3];

  // read in binary image navigable map
  unsigned * map;
  unsigned height, width;
  readPPM(mapfile,&width,&height,&map);

  std::cout << "Map Info: " << width << " " << height << std::endl;
  
  unsigned max = 0;
  const unsigned mapsize = width*height;
  for ( unsigned i = 0; i != mapsize; i++ ) {
    if ( map[i] > max ){
      max = map[i];
    }
  }

  std::cout << " max from map: " << max << std::endl;

  //  create and instance of the terrain analyzer
  Turk::ARTIE ta;
  ta.load_map(map,width,height);

  // do some stuff with artie
  ta.analyze_map();

  // dump artie data
  char output_base[500];
  sprintf(output_base,"%s/%s",output_dir,base_name);
  ta.dump_data(output_base);


  return 0;
}
