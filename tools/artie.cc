    
#include "artie.h"

#include <cassert>
#include <iostream>
#include <cmath>

#include<sys/random.h>

using namespace Turk;

const point Turk::ARTIE::m_neighbors[m_nNeighbs] = {point(-1,0), point(1,0), point(0,-1), point(0,1)};

color Turk::gen_random_color() {
  // generate a random character from /dev/urandom
  char col[3];
  getrandom(col,3,0);

  return color(col[0],col[1],col[2]);
}


void ARTIE::load_map(const unsigned * map, const unsigned width, const unsigned height) { 

  if ( m_walkable ) delete m_walkable;

  m_width = width;
  m_height = height;
  m_mapsize = width*height;

  m_walkable = new bool[m_mapsize];

  for ( unsigned i=0; i != m_mapsize; i++ ) 
    m_walkable[i] = (bool)map[i];


}

void ARTIE::analyze_map() { 
  clean_map();
  obstacle_flood_fill();
  //build_distance_map();
  refine_distance_map();
  water_level_decomposition();
}


void ARTIE::get_distance_map() { }

void ARTIE::get_chokes() { }


void ARTIE::dump_data(const char * base_name) { 

  char output[500];

  // dump walkable map
  sprintf(output,"%s_walkable.ppm",base_name);
  std::cout << "Dumping walkable map: " << output << std::endl;
  dump_image(output,m_walkable,m_width,m_height);

  // dump  distance map
  sprintf(output,"%s_distance.ppm",base_name);
  std::cout << "Dumping distance map: " << output << std::endl;
  dump_image(output,m_dmap,m_width,m_height);
  
  // dump obstacle clusters
  sprintf(output,"%s_obstacle.ppm",base_name);
  std::cout << "Dumping obstacle cluster map: " << output << std::endl;
  dump_categorical_map(output,m_obstacles,m_width,m_height);

  // dump water level map
  sprintf(output,"%s_waterlevel.ppm",base_name);
  std::cout << "Dumping water level map: " << output << std::endl;
  dump_categorical_map(output,m_wlmap,m_width,m_height);

  // dump gate points
  sprintf(output,"%s_gatepoint.ppm",base_name);
  std::cout << "Dumping gatepoint map: " << output << std::endl;
  dump_image(output,m_gatepointmap,m_width,m_height);

  // dump walkable areas flood fill map
  sprintf(output,"%s_walkable_areas.ppm",base_name);
  std::cout << "Dumping walkable areas map: " << output << std::endl;
  dump_categorical_map(output,&tmp[0],m_width,m_height);

}

void ARTIE::dump_image(const char * name, const bool * map, const unsigned width, const unsigned height ) { 

  FILE * file = fopen(name,"wb");
  fprintf(file,"P6\n%i %i 255\n",width,height);
  for ( unsigned i = 0; i != height; i++ ) {
    for ( unsigned j =0 ; j != width; j++ ) {
      if ( map[i*width+j] ) {
        fputc(255,file);
        fputc(255,file);
        fputc(255,file);
      } else {
        fputc(0,file);
        fputc(0,file);
        fputc(0,file);
      }
    }
  }

}

void ARTIE::dump_image(const char * name, const unsigned * map, const unsigned width, const unsigned height) { 

  FILE * file = fopen(name,"wb");
  fprintf(file,"P6\n%i %i 255\n",width,height);
  for ( unsigned i=0; i != height; i++ ) {
    for ( unsigned j=0; j != width; j++ ) {
      const unsigned value = map[i*width+j];
      fputc(value,file);
      fputc(value,file);
      fputc(value,file);
    }
  }

}

void ARTIE::dump_categorical_map(const char * name, const unsigned * map, const unsigned width, const unsigned height ) {

  const unsigned ms = width*height;

  // determine the number of unique categories
  // assign a color for each category
  std::map<unsigned, color>  cmap;
  for ( unsigned i=0; i != ms; i++ ) {
    const unsigned cat = map[i];

    // if category is not in map, create a random color and add to map
    if ( cmap.find(cat) == cmap.end() ) {
      if ( cat != 0 )
        cmap[cat] = gen_random_color();
      else
        cmap[cat] = color(0,0,0);
    }
  }

  // dump color map
  FILE * file = fopen(name,"wb");
  fprintf(file,"P6\n%i %i 255\n",width,height);
  for ( unsigned i=0; i != ms; i++ ) {
    const color col = cmap[map[i]];
    fputc(col.red,file);
    fputc(col.green,file);
    fputc(col.blue,file);
  }
  

}

void ARTIE::build_distance_map() { 

  // build distance map
  if ( m_dmap ) delete m_dmap;
  m_dmap = new unsigned[m_mapsize];
  for (unsigned i = 0; i != m_mapsize; i++) m_dmap[i] = 0U;



  for (unsigned y = 0; y != m_height; y++) {
    for (unsigned x = 0; x != m_width; x++) {
      if (!m_walkable[y*m_width + x]) {
        m_dmap[y*m_width + x] = 0;
        continue;
      }

      int dist = 1;
      bool foundwall = false;
      while (!foundwall) {
        //foundwall = true;

        // top edge
        if (y >= dist) {
          for (int i = -dist; i <= dist; i++) {
            if ( x + i > m_width ) continue;
            unsigned index = (y - dist)*m_width + x + i;
            if (index >= m_mapsize) continue;

            if (!m_walkable[index]) {
              m_dmap[y*m_width + x] = dist;
              foundwall = true;
              break;
            }
          }
        }

        // bottom edge
        if (y + dist < m_width) {
          for (int i = -dist; i <= dist; i++) {
            if ( x + i > m_width ) continue;
            unsigned index = (y + dist)*m_width + x + i;
            if (index >= m_mapsize) continue;

            if (!m_walkable[index]) {
              m_dmap[y*m_width + x] = dist;
              foundwall = true;
              break;
            }
          }
        }

        // left edge
        if (x >= dist) {
          for (int i = -dist + 1; i < dist; i++) {
            if ( y + i > m_height ) continue;
            unsigned index = (y + i)*m_width + x - dist;
            if (index >= m_mapsize)continue;

            if (!m_walkable[index]) {
              m_dmap[y*m_width + x] = dist;
              foundwall = true;
              break;
            }
          }
        }

        //right edge
        if (x + dist < m_width) {
          for (int i = -dist + 1; i < dist; i++) {
            if ( y + i > m_height ) continue;
            unsigned index = (y + i)*m_width + x + dist;
            if (index >= m_mapsize) continue;
        
            if (!m_walkable[index]) {
              m_dmap[y*m_width + x] = dist;
              foundwall = true;
              break;
            }
          }
        }

        dist++;
      }
    }
  }


  // build the reverse map of distance to a set of tiles
  unsigned max_dist = 0U;
  for (unsigned i = 0; i != m_mapsize; i++) {
    assert((m_obstacles[i] > 0 && !m_walkable[i]) ^ (m_obstacles[i] == 0 && m_walkable[i]));
    if (m_obstacles[i] > 0) continue;
    if (m_dmap[i] > max_dist)
    max_dist = m_dmap[i];
		
    m_rdmap[m_dmap[i]].push_back(i);
  }

}

void ARTIE::refine_distance_map() {

  std::cout << "Refine Distance Map" << std::endl;

  // allocate memory for distance map
  if ( m_dmap ) delete m_dmap;
  m_dmap = new unsigned[m_mapsize];
  for ( unsigned i=0; i != m_mapsize; i++ ) m_dmap[i] = 0U;

  m_nnobj.resize(m_mapsize);


  //  cycle over each point
  for ( unsigned i=0; i != m_mapsize; i++ ) {
    // continue of this is an obstacle
    if ( m_obstacles[i] ) continue;

    // distance fill the area
    //dist_fill_area(i);
    dist_nearest_obstacle(i);
 
  }


  // build the reverse map of distance to a set of tiles
  unsigned max_dist = 0U;
  for (unsigned i = 0; i != m_mapsize; i++) {
    assert((m_obstacles[i] > 0 && !m_walkable[i]) ^ (m_obstacles[i] == 0 && m_walkable[i]));
    if (m_obstacles[i] > 0) continue;
    if (m_dmap[i] > max_dist)
    max_dist = m_dmap[i];
		
    m_rdmap[m_dmap[i]].push_back(i);
  }

}



void ARTIE::water_level_decomposition() { 

  unsigned cur_label = 1;

  m_nGatePoints = 0;
  m_gatepoints = new unsigned[m_mapsize];
  m_wlmap = new unsigned[m_mapsize];


  // reverse iterate over the reverse distance map
  std::map<unsigned, std::vector<unsigned> >::reverse_iterator rit = m_rdmap.rbegin();
  for (; rit != m_rdmap.rend(); ++rit) {

    const std::vector<unsigned> & indices = (*rit).second;
    const unsigned n_indices = indices.size();
    
    unsigned labels[8];
    unsigned n_neighbs = 0;

    for (unsigned i = 0; i != n_indices; i++) {
      const unsigned index = indices[i];
      // skip this index if it's an obstacle
      if (m_obstacles[index] > 0) continue;

      n_neighbs = 0;

      const unsigned x = index % m_width;
      const unsigned y = index / m_width;

      // collect neighbors
      for (int j = -1; j != 2; j++) {
        if (y + j >= m_height || y+j < 0) continue;
        for (int k = -1; k != 2; k++) {
	  if (x + k >= m_width || x+k < 0 ) continue;
	  if (j == 0 && k == 0) continue;

	  const unsigned neighb = index + j*m_width + k;
	  if (neighb >= m_mapsize) continue;

	  if (m_wlmap[neighb] > 0) {
	    labels[n_neighbs] = m_wlmap[neighb];
	    n_neighbs++;
	  }
        }
      }

      // check if the tile has label neighbors
      if (n_neighbs > 1) {
        // check if all labels are the same
        bool same_labels = true;
        const unsigned label = labels[0];
        for (unsigned j = 1; j != n_neighbs; j++)
	  same_labels &= label == labels[j];

	if (!same_labels) {
	  m_gatepoints[m_nGatePoints] = index;
 	  m_nGatePoints++;
	}
	else {
	  m_wlmap[index] = label;
	}
      }
      else if (n_neighbs == 1) {
	m_wlmap[index] = labels[0];
      }
      else {
	m_wlmap[index] = cur_label;
        cur_label++;
      }
			
    }

  }

  // create gatepoint map
  m_gatepointmap = new bool[m_mapsize];
  for ( unsigned i=0; i != m_mapsize; i++ ) 
    m_gatepointmap[i] = false;

  for ( unsigned i=0; i != m_nGatePoints; i++ ) 
    m_gatepointmap[m_gatepoints[i]] = true;

}


void ARTIE::obstacle_flood_fill() {

  // allocate the obstacle map
  if ( m_obstacles ) {
    delete m_obstacles;
    m_nObstacles = 0U;
  }

  m_obstacles = new unsigned[m_mapsize];
  
  // fill with zeros
  for ( unsigned i = 0; i != m_mapsize; i++ )
    m_obstacles[i] = 0U;

  // flood fill the areas
  unsigned counter = 1;
  for ( unsigned i = 0; i != m_mapsize; i++ ) {
    if ( !m_walkable[i] && !m_obstacles[i] ) {
      rfill_area(i,counter,m_obstacles,m_walkable);
      counter++;
    }
  }

  m_nObstacles = counter;


}

void ARTIE::clean_map() {

  // flood fill the walkable map
  //std::vector<unsigned> tmp(m_mapsize);
  tmp.resize(m_mapsize);
  std::cout << "Created cleaner map: " << tmp.size() << std::endl;

  std::vector<unsigned> dummy(m_mapsize);
  for ( unsigned i=0; i != m_mapsize; i++ )
    dummy[i] = !m_walkable[i];


  unsigned tcount = 0U;
  // remove small regions
  for ( unsigned i=0; i != m_mapsize; i++ ) {
    if ( m_walkable[i] && !tmp[i] ) {
      tcount++;
      ff_fill_area(i,tcount,&tmp[0],&dummy[0]);
    }
  }

  // check for empty tmps
  /*for ( unsigned i=0; i != m_mapsize; i++ ) {
    assert((tmp[i] > 0 && m_walkable[i]) ^ (tmp[i] == 0 && m_obstacles[i] > 0 ));
  }*/
  
  // acquire size/area for each walkable region
  std::cout << "There are " << tcount << " walkable regions." << std::endl;
  unsigned * areas = new unsigned[tcount];
  for ( unsigned i=0; i != tcount; i++ ) {
    areas[i] = 0U;   
  }

  for ( unsigned i=0; i != m_mapsize; i++ ) {
    if ( tmp[i] ) {
      areas[tmp[i]-1]++;
    }
  }

  unsigned area_sum = 0U;
  unsigned max_area = 0U;
  unsigned max_index = 0U;
  for ( unsigned i=0; i != tcount; i++ ) {
    area_sum += areas[i];
    if ( areas[i] > max_area ) {
      max_area = areas[i];
      max_index = i;
    }
  } 

  unsigned walkable_sum = 0U;
  for ( unsigned i=0; i != m_mapsize; i++ ) {
    if ( m_walkable[i] ) walkable_sum++;
  }

  std::cout << "Max Area: areas[" << max_index << "] = " << max_area << std::endl;
  std::cout << "Walkable Area: " << walkable_sum << " area sum: " << area_sum << std::endl;

  // clean up walkable map
  /*for ( unsigned i=0; i != m_mapsize; i++ ) {
    if ( tmp[i] && areas[tmp[i]-1] < 100 ) m_walkable[i] = false;
  }*/

}


void ARTIE::ff_fill_area(const unsigned index, const unsigned counter, unsigned * map, const unsigned * exclude ) {

  // leave if this has already been evaluated or
  // out of bounds.
  if ( index >= m_mapsize || map[index] ) return;
  if ( exclude[index] ) return;

  // create a queue
  std::vector<unsigned> queue(m_mapsize);
  unsigned loc = 0U;
  unsigned ql = 0U; // length of the current queue

  // add current index to the queue
  queue[0] = index;
  ql++;

  map[index] = counter;

  while ( ql > 0 ) {

    // take one out of the queue
    ql--;

    // take current node out of queue
    const unsigned cindex = queue[loc];

    // check neighbors
    const unsigned x = cindex % m_width;
    const unsigned y = cindex / m_width;

    const unsigned north = (y-1)*m_width+x;
    const unsigned south = (y+1)*m_width+x;
    const unsigned east = y*m_width+x+1;
    const unsigned west = y*m_width+x-1;

    // number of additional indices to the queue
    unsigned add = 0U;

    // if neighbor is viable, mark and add to queue
    // the neighbor is viable if it is within bounds and not already set

    if ( y > 0 && north < m_mapsize && !map[north] && !exclude[north] ) {
      map[north] = counter;
      add++;
      queue[loc+ql+add] = north;
    }

    if ( y+1 < m_height && south < m_mapsize && !map[south] && !exclude[south] ) {
      map[south] = counter;
      add++;
      queue[loc+ql+add] = south;
    }

    if ( x > 0 && west < m_mapsize && !map[west] && !exclude[west] ) {
      map[west] = counter;
      add++;
      queue[loc+ql+add] = west;
    }

    if ( x+1 < m_width && east < m_mapsize && !map[east] && !exclude[east] ) {
      map[east] = counter;
      add++;
      queue[loc+ql+add] = east;
    }


    // progress the location
    loc++;


    // extend the queue length
    ql += add;

  }

}


void ARTIE::dist_fill_area(const unsigned index) {

  // preliminary checks on the condition of the index
  // and traversability of the tile

  const point origin = decomposeIndex(index);

  // start the queue
  std::vector<point> queue(m_mapsize);
  unsigned loc = 0;
  unsigned ql = 1;
  queue[0] = decomposeIndex(index);

  // checks to first obstacle encountered
  bool isFirstObstacle = true;

  // add a mask to keep the queue efficiently filled
  std::vector<bool> mask(m_mapsize);
  std::vector<int> qdist(m_mapsize);


  // keep going until the queue is empty
  while ( ql > 0 ) {
    // decrement the queue length
    ql--;

    unsigned add = 0;

    const point & cur_point = queue[loc];

    // cycle over potential neighbors
    for ( unsigned j=0; j != m_nNeighbs; j++ ) {
      point neighb = cur_point + m_neighbors[j];
      

      if ( isValidPoint(neighb) ) {
        // calculate the distance to the point
        unsigned dist = (neighb-origin).l1length();

        unsigned nindex = composeIndex(neighb);
  

        // if its the first obstacle record distnace and index
        if ( isFirstObstacle && m_obstacles[nindex] ) {
          isFirstObstacle = false;
          m_dmap[index] = dist;
          m_nnobj[index] = nindex;
          break;
        } else if ( !m_obstacles[nindex] && !mask[nindex] ) {
          // add point to the queue
          add++;
          queue[loc+ql+add] = neighb;
          qdist[loc+ql+add] = dist;
          mask[nindex] = true;
        }

      }

    } 

    
    // progress the location
    loc++;

    // extend the queue length
    ql += add;
  }

  

}




void ARTIE::dist_nearest_obstacle(const unsigned index) {

  // preliminary checks on the condition of the index
  
  const point origin = decomposeIndex(index);

  // 
  bool foundObstacle = false;

  unsigned dist = 1;

  while ( !foundObstacle ) {

    // top left diag
    point start1 = origin + m_neighbors[0]*dist;
    point start2 = origin + m_neighbors[1]*dist;
    point start3 = origin + m_neighbors[2]*dist;
    point start4 = origin + m_neighbors[3]*dist;

    for ( unsigned i=0; i != dist; i++ ) {

      const point pt1 = start1 + point(i,-i);
      const unsigned nindex1 = composeIndex(pt1);
      const point pt2 = start2 + point(-i,i);
      const unsigned nindex2 = composeIndex(pt2);
      const point pt3 = start3 + point(i,i);
      const unsigned nindex3 = composeIndex(pt3);
      const point pt4 = start4 + point(-i,-i);
      const unsigned nindex4 = composeIndex(pt4);


      if ( isValidPoint(pt1) && m_obstacles[nindex1] ) {
        foundObstacle = true;
        assert((pt1-origin).l1length() == dist);
        m_dmap[index] = dist;
        m_nnobj[index] = nindex1;
        //return;
      }

      if ( isValidPoint(pt2) && m_obstacles[nindex2] ) {
        foundObstacle = true;
        assert((pt2-origin).l1length() == dist);
        m_dmap[index] = dist;
        m_nnobj[index] = nindex2;
        //return;
      }

      if ( isValidPoint(pt3) && m_obstacles[nindex3] ) {
        foundObstacle = true;
        assert((pt3-origin).l1length() == dist);
        m_dmap[index] = dist;
        m_nnobj[index] = nindex3;
        //return;
      }

      if ( isValidPoint(pt4) && m_obstacles[nindex4] ) {
        foundObstacle = true;
        assert((pt4-origin).l1length() == dist);
        m_dmap[index] = dist;
        m_nnobj[index] = nindex4;
        //return;
      }

      if ( foundObstacle ) {
        const unsigned dd = dist;
        return;
      }

    } 

    dist++;
    
  }

}


