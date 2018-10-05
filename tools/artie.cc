    
#include "artie.h"

#include <cassert>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <cstring>

#ifdef _WIN32
#include <random>
#else
#include<sys/random.h>
#endif


#include "polyartie.h"
#include "dbscan.h"
#include "queue.h"

using namespace Turk;

#ifdef _WIN32
boost::random::rand48 artie_gen;
#endif


const point Turk::ARTIE::m_neighbors[m_nNeighbs] = {point(-1,0), point(1,0), point(0,-1), point(0,1)};

const point Turk::ARTIE::m_segment[m_segsize] = {
  point(-1,1), point(0,1), point(1,1)
  ,point(-1,0), point(1,0)
  ,point(-1,-1), point(0,-1), point(1,-1) };

color Turk::gen_random_color() {
  
  char col[3];
#ifdef _WIN32
  unsigned tmp = artie_gen();
  memcpy(col, &tmp, 3);
#else
  // generate a random character from /dev/urandom
  getrandom(col,3,0);
#endif


  return color(col[0],col[1],col[2]);
}


void ARTIE::load_map(const unsigned * map, const unsigned width, const unsigned height) { 

  m_width = width;
  m_height = height;
  m_mapsize = width*height;

  m_walkable.clear();
  m_walkable.resize(m_mapsize);

  for ( unsigned i=0; i != m_mapsize; i++ ) 
    m_walkable[i] = (bool)map[i];


}

void ARTIE::analyze_map() { 
  clean_map();
  obstacle_flood_fill();
  build_distance_map();
  //mat();
  //water_level_decomposition();
  find_critical_points();
  triangulate();
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
  dump_image(output,&m_dmap[0],m_width,m_height);
  
  // dump obstacle clusters
  sprintf(output,"%s_obstacle.ppm",base_name);
  std::cout << "Dumping obstacle cluster map: " << output << std::endl;
  dump_categorical_map(output,&m_obstacles[0],m_width,m_height);

  // dump water level map
  /*sprintf(output,"%s_waterlevel.ppm",base_name);
  std::cout << "Dumping water level map: " << output << std::endl;
  dump_categorical_map(output,&m_wlmap[0],m_width,m_height);
  */

  // dump gate points
  /*sprintf(output,"%s_gatepoint.ppm",base_name);
  std::cout << "Dumping gatepoint map: " << output << std::endl;
  dump_image(output,m_gatepointmap,m_width,m_height);
  */

  // dump walkable areas flood fill map
  sprintf(output,"%s_walkable_areas.ppm",base_name);
  std::cout << "Dumping walkable areas map: " << output << std::endl;
  dump_categorical_map(output,&m_walkable_areas[0],m_width,m_height);

  // dump the MAT map
  /*sprintf(output,"%s_mat.ppm",base_name);
  std::cout << "Dumping MAT map: " << output << std::endl;
  dump_categorical_map(output,&m_mat[0],m_width,m_height);
  */

  // dump the critical points
  sprintf(output,"%s_critical_points.csv",base_name);
  dump_points(output,&m_critical_points[0],m_critical_points.size());

  // dump minima
  sprintf(output,"%s_critical_minima.csv",base_name);
  dump_points<double>(output,m_critical_mins);

  // dump clusters
  sprintf(output,"%s_clusters.csv",base_name);
  dump_csv<double>(output,&m_critical_clus[0],2,m_critical_clus.size()/2);

  // dump labels
  sprintf(output,"%s_cluster_labels.csv",base_name);
  dump_csv<int>(output,&m_clu_labels[0],1,m_clu_labels.size());

  // dump regions
  sprintf(output,"%s_map_regions.ppm",base_name);
  dump_categorical_map(output,&m_region_map[0],m_width,m_height);
 
  // dump map graph
  sprintf(output,"%s_map_graph.csv",base_name);
  dump_csv<unsigned>(output,&m_map_graph[0],m_n_nuclei,m_n_nuclei); 

}

void ARTIE::dump_image(const char * name, const std::vector<bool> & map, const unsigned width, const unsigned height ) { 

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

void ARTIE::dump_points(const char *name, const point * points, const unsigned len) {

  std::ofstream out(name);
  out << "x,y" << std::endl;
  for ( unsigned i=0; i != len; i++ )
    out << points[i].x << "," << points[i].y << std::endl;

  out.close();

}


void ARTIE::build_distance_map() {

  std::cout << "Refine Distance Map" << std::endl;

  // allocate memory for distance map
  m_dmap.clear();
  m_dmap.resize(m_mapsize);
  for ( unsigned i=0; i != m_mapsize; i++ ) m_dmap[i] = 0U;

  m_nnobj.resize(m_mapsize);


  //  cycle over each point
  for ( unsigned i=0; i != m_mapsize; i++ ) {
    // continue of this is an obstacle
    if ( m_obstacles[i] ) continue;

    // distance fill the area
    dist_nearest_obstacle_l2(i);
 
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
  m_gatepoints.resize(m_mapsize);
  m_wlmap.resize(m_mapsize);


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
  m_gatepointmap.clear();
  m_gatepointmap.resize(m_mapsize);
  for ( unsigned i=0; i != m_mapsize; i++ ) 
    m_gatepointmap[i] = false;

  for ( unsigned i=0; i != m_nGatePoints; i++ ) 
    m_gatepointmap[m_gatepoints[i]] = true;

}


void ARTIE::obstacle_flood_fill() {

  // allocate the obstacle map
  //m_obstacles.clear(); // check
  m_obstacles.resize(m_mapsize);
  
  // fill with zeros
  for ( unsigned i = 0; i != m_mapsize; i++ )
    m_obstacles[i] = 0U;

  // flood fill the areas
  unsigned counter = 1;
  for ( unsigned i = 0; i != m_mapsize; i++ ) {
    if ( !m_walkable[i] && !m_obstacles[i] ) {
      rfill_area(i,counter,&m_obstacles[0],m_walkable);
      counter++;
    }
  }

  m_nObstacles = counter;


}

void ARTIE::clean_map() {

  // flood fill the walkable map
  m_walkable_areas.resize(m_mapsize);
  std::cout << "Created cleaner map: " << m_walkable_areas.size() << std::endl;

  std::vector<unsigned> dummy(m_mapsize);
  for ( unsigned i=0; i != m_mapsize; i++ )
    dummy[i] = !m_walkable[i];


  unsigned tcount = 0U;
  // remove small regions
  for ( unsigned i=0; i != m_mapsize; i++ ) {
    if ( m_walkable[i] && !m_walkable_areas[i] ) {
      tcount++;
      ff_fill_area(i,tcount,&m_walkable_areas[0],&dummy[0]);
    }
  }

  // check for empty m_walkable_areas
  /*for ( unsigned i=0; i != m_mapsize; i++ ) {
    assert((m_walkable_areas[i] > 0 && m_walkable[i]) ^ (m_walkable_areas[i] == 0 && m_obstacles[i] > 0 ));
  }*/
  
  // acquire size/area for each walkable region
  std::cout << "There are " << tcount << " walkable regions." << std::endl;
  unsigned * areas = new unsigned[tcount];
  for ( unsigned i=0; i != tcount; i++ ) {
    areas[i] = 0U;   
  }

  for ( unsigned i=0; i != m_mapsize; i++ ) {
    if ( m_walkable_areas[i] ) {
      areas[m_walkable_areas[i]-1]++;
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
  for ( unsigned i=0; i != m_mapsize; i++ ) {
    if ( m_walkable_areas[i] && areas[m_walkable_areas[i]-1] < 1000 ) m_walkable[i] = false;
  }

  // delete areas memory
  delete [] areas;

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




void ARTIE::dist_nearest_obstacle_l1(const unsigned index) {

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
        return;
      }

    } 

    dist++;
    
  }

}


void  ARTIE::mat() {
  
  // initialize the mat image
  m_mat.reserve(m_mapsize);

  // scan the depth map and flag minima
  for ( unsigned i=0; i != m_mapsize; i++ ) {
    // construct the point
    const point pt = decomposeIndex(i);

    // check depth of neighbors (up, down, left, right)
    const point left = pt + m_neighbors[0];
    const point right = pt + m_neighbors[1];
    const point up = pt + m_neighbors[2];
    const point down = pt + m_neighbors[3];
    const point up_right = up + m_neighbors[1];
    const point up_left = up + m_neighbors[0];
    const point down_right = down + m_neighbors[1];
    const point down_left = down + m_neighbors[0];

    if ( isValidPoint(left) && isValidPoint(right) 
      && m_dmap[i] >= std::max(m_dmap[composeIndex(left)],m_dmap[composeIndex(right)])
      && m_dmap[i] != std::min( m_dmap[composeIndex(left)],m_dmap[composeIndex(right)])
    ) {
      m_mat[i] = true;
    } else if ( isValidPoint(up) && isValidPoint(down)
      && m_dmap[i] >= std::max(m_dmap[composeIndex(up)],m_dmap[composeIndex(down)])
      && m_dmap[i] != std::min(m_dmap[composeIndex(up)],m_dmap[composeIndex(down)])
    ) {
      m_mat[i] = true;
    } else if ( isValidPoint(up_right) && isValidPoint(down_left)
      && m_dmap[i] >= std::max(m_dmap[composeIndex(up_right)],m_dmap[composeIndex(down_left)])
      && m_dmap[i] != std::min(m_dmap[composeIndex(up_right)],m_dmap[composeIndex(down_left)])
    ) {
      m_mat[i] = true;
    } else if ( isValidPoint(up_left) && isValidPoint(down_right)
      && m_dmap[i] >= std::max(m_dmap[composeIndex(up_left)],m_dmap[composeIndex(down_right)])
      && m_dmap[i] != std::min(m_dmap[composeIndex(up_left)],m_dmap[composeIndex(down_right)])
    ) {
      m_mat[i] = true;
    }

    
  }

  // filter the mat
  mat_filter();

  // prune the resulting image

}


void ARTIE::dist_nearest_obstacle_l2(const unsigned index) {

  const point origin = decomposeIndex(index);

  bool foundObstacle = false;

  unsigned dist = 1;

  while ( !foundObstacle ) {

    // start dist pixels away to the left
    // scan across and check top and bottom curves
    const unsigned range = 2*dist + 1;
    const point start = origin + m_neighbors[0]*dist;
    for ( unsigned i=0; i != range; i++ ) {
      const unsigned x = start.x+i;
      const unsigned delta = abs((int)(origin.x-x));
      const point upper(x,start.y-sqrt(dist*dist-delta*delta));
      const point lower(x,start.y+sqrt(dist*dist-delta*delta));

      if ( !isValidPoint(upper) || m_obstacles[composeIndex(upper)] ) {
	foundObstacle = true;
	//assert((upper-origin).length() == dist);
	m_dmap[index] = dist;
	m_nnobj[index] = composeIndex(upper);
      } else if ( !isValidPoint(lower) || m_obstacles[composeIndex(lower)] ) {
	foundObstacle = true;
	//assert((lower-origin).length() == dist);
	m_dmap[index] = dist;
	m_nnobj[index] = composeIndex(lower);
      }

      if ( foundObstacle ) {
	return;
      }
    }

    dist++;    
  }

}

void ARTIE::mat_filter() {

  // filter north-to-south
  for ( unsigned i=0; i != m_width; i++ ) {
    for ( unsigned j=0; j != m_height; j++ ) {
      const unsigned index = j*m_width+i;
      if ( m_mat[index] ) {
        // construct the segment
        unsigned segment[9] = {0};

        const point p = decomposeIndex(index);

        for ( unsigned k=0; k != m_segsize; k++ ) {
          const point nn = p + m_segment[k];
          if ( isValidPoint(nn) ) segment[(1+m_segment[k].y)*3+1+m_segment[k].x] = m_mat[composeIndex(nn)];
 
        }

        const bool safe = mat_is_safe(segment);
        const bool end = mat_is_end(segment);

        if ( safe && !end ) m_mat[index] = 0;

      }
    }
  }



  // filter east-to-west
  for ( unsigned j=0; j != m_height; j++ ) {
    for ( unsigned i=0; i != m_width; i++ ) {
      const unsigned index = j*m_width+i;
      if ( m_mat[index] ) {

        // construct the segment
        unsigned segment[9] = {0};

        const point p = decomposeIndex(index);

        for ( unsigned k=0; k != m_segsize; k++ ) {
          const point nn = p + m_segment[k];
          if ( isValidPoint(nn) ) segment[(1+m_segment[k].y)*3+1+m_segment[k].x] = m_mat[composeIndex(nn)];

        }

        const bool safe = mat_is_safe(segment);
        const bool end = mat_is_end(segment);

        if ( safe && !end ) m_mat[index] = 0; 

      }
    }
  }
  
  

}


bool ARTIE::mat_is_safe(const unsigned segment[] ) {

  if ( segment[0] > 0 && !segment[1] && !segment[3] ) {
    return false;
  } else if ( segment[2] && !segment[1] && !segment[5] ) {
    return false;
  } else if ( segment[8] && !segment[5] && !segment[7] ) {
    return false;
  } else if ( segment[6] && !segment[7] && !segment[3] ) {
    return false;
  } else if ( segment[3] && segment[5] && !segment[1] && !segment[7] ) {
    return false;
  } else if ( !segment[3] && !segment[5] && segment[1] && segment[7] ) {
    return false;
  }

  return true;

}


bool ARTIE::mat_is_end(const unsigned segment[] ) {

  unsigned sum = 0;

  for ( unsigned i=0; i != 3; i++ ) {
    for ( unsigned j=0; j != 3; j++ ) {
      if ( i == 1 && j == 1 ) continue;
      sum += segment[i*3+j];
    }
  }

  return sum >= 5;
}


void ARTIE::find_critical_points() {

  // scan window
  std::vector<double> window(25);
  std::vector<double> x(25),y(25 );

  // fill x,y values
  for ( int i=-2; i != 3; i++ ) {
    for ( int j=-2; j != 3; j++ ) {
      x[(i+2)*5+j+2] = j;
      y[(i+2)*5+j+2] = i;
    }
  }

  // collection critical point locations
  std::vector<point> critical_points;
 
  // scan the image (ignore the edges, at least for now)
  const unsigned scan_width = m_width-2;
  const unsigned scan_height = m_height-2;
  for ( int i=2; i != scan_height; i++ ) {
    for ( int j=2; j != scan_width; j++ ) {

      // check if the central point is walkable
      if ( !m_walkable[i*m_width+j] ) continue;

      // fill window
      for ( int k=-2; k != 3; k++ ) {
        for ( int l=-2; l != 3; l++ ) {
          window[(k+2)*5+l+2] = m_dmap[(i+k)*(int)m_width+j+l];
        }
      }

      // fit the polynomial 
      polyartie fitter(x,y,window);
      
      // add point to list
      if ( fitter.isMinima() ) {
        critical_points.push_back(point(j,i));

        const std::pair<double,double> & min = fitter.getMinima();
        m_critical_mins.push_back(min);

      }


    }
  }

  m_critical_points = critical_points;

  const unsigned n_pts = critical_points.size();
  double * data = new double[2*n_pts];
  for ( unsigned i=0; i != n_pts; i++ ) {
    const point pt = critical_points[i];
    data[i*2] = pt.x;
    data[i*2+1] = pt.y;
  } 

  // cluster critical points
  dbscan db;
  db.fit(data,2,n_pts);
  const std::vector<int> labels = db.getLabels();
  m_clu_labels = labels;
 
  

  // collect cluster points and find centroids
  const unsigned nclus = *std::max_element(labels.begin(),labels.end()) + 1;
  std::vector<std::vector<unsigned> > clu_map(nclus);
  std::vector<double> centers(2*nclus);
  for ( unsigned i=0; i != n_pts; i++ ) {
    const int lab = labels[i];
    if ( lab >= 0 ) { 
      clu_map[lab].push_back(i);
      centers[lab*2] += data[i*2];
      centers[lab*2+1] += data[i*2+1];
    }
  }

  for ( unsigned i=0; i != nclus; i++ ) {
    const double N = clu_map[i].size();
    centers[i*2] /= N;
    centers[i*2+1] /= N;
  }

  // copy to cluster storage
  m_critical_clus.push_back(0.);
  m_critical_clus.push_back(0.);
  for ( unsigned i=0; i != nclus; i++ ) {
    const point pt(centers[i*2],centers[i*2+1]);
    if ( isValidPoint(pt) && m_dmap[composeIndex(pt)] > 2 ) {
      m_critical_clus.push_back(pt.x);
      m_critical_clus.push_back(pt.y);
    }
  }

  delete [] data;

}


void ARTIE::triangulate() {

  // determine nucleation points
  m_n_nuclei = m_critical_clus.size()/2;

  // create queues for each nucleation point
  std::vector<cycle_queue> queues(m_n_nuclei);

  // create region labeled map
  m_region_map.resize(m_width*m_height); 
  for ( unsigned i=0; i != m_mapsize; i++ )
    m_region_map[i] = 0;
  
  // create graph matrix
  m_map_graph.resize(m_n_nuclei*m_n_nuclei);
  for ( unsigned i=0; i != m_n_nuclei*m_n_nuclei; i++ )
    m_map_graph[i] = 0;

  std::vector<point> center_points(m_n_nuclei);

  // initialize the queues
  unsigned ntasks = m_n_nuclei - 1;
  for ( unsigned i=1; i != m_n_nuclei; i++ ) {
    queues[i].push(composeIndex(m_critical_clus[2*i],m_critical_clus[2*i+1]));
    queues[i].init_cycle();

    center_points[i] = point(m_critical_clus[2*i],m_critical_clus[2*i+1]); 
  }

  unsigned round = 0;

  // while items in queues
  while ( ntasks ) {
    //if ( round++ > 10 ) break;

    // for each nucleus, cycle queue  
    for ( unsigned i=1; i != m_n_nuclei; i++ ) {
      int index = queues[i].cycle_pop();
      while ( index >= 0 ) {

        // label index with cluster label
        if ( m_region_map[index] == 0 ) {
          m_region_map[index] = i;
          // add neighbors to queue
          const point pt = decomposeIndex(index);
          const point & cpt = center_points[i];
          const int radius = (pt - cpt).length() + 1;

          const point npt = pt + point(0,1);
          if ( isValidPoint(npt) && m_walkable[composeIndex(npt)] && (npt - cpt).length() <= radius ) 
            queues[i].push(composeIndex(npt));

          const point nept = npt + point(1,0);
          if ( isValidPoint(nept) && m_walkable[composeIndex(nept)] && (nept - cpt).length() <= radius)
            queues[i].push(composeIndex(nept));

          const point ept = pt + point(1,0);
          if ( isValidPoint(ept) && m_walkable[composeIndex(ept)] && (ept - cpt).length() <= radius)
            queues[i].push(composeIndex(ept));

          const point sept = ept + point(0,-1);
          if ( isValidPoint(sept) && m_walkable[composeIndex(sept)] && (sept - cpt).length() <= radius)
            queues[i].push(composeIndex(sept));

          const point spt = pt + point(0,-1);
          if ( isValidPoint(spt) && m_walkable[composeIndex(spt)] && (spt - cpt).length() <= radius)
            queues[i].push(composeIndex(spt));

          const point swpt = spt + point(-1,0);
          if ( isValidPoint(swpt) && m_walkable[composeIndex(swpt)] && (swpt - cpt).length() <= radius)
            queues[i].push(composeIndex(swpt));

          const point wpt = pt + point(-1,0);
          if ( isValidPoint(wpt) && m_walkable[composeIndex(wpt)] && (wpt - cpt).length() <= radius)
            queues[i].push(composeIndex(wpt));

          const point nwpt = wpt + point(0,1);
          if ( isValidPoint(nwpt) && m_walkable[composeIndex(nwpt)] && (nwpt - cpt).length() <= radius)
            queues[i].push(composeIndex(nwpt));
         

        } else if ( m_region_map[index] != i ) {
          const unsigned lab = m_region_map[index];
          // add link to graph matrix
          m_map_graph[i*m_n_nuclei+lab] = 1;
          m_map_graph[lab*m_n_nuclei+i] = 1;
        }

        index = queues[i].cycle_pop();
      }
    } 
   
    ntasks = 0; 
    for ( unsigned i=1; i != m_n_nuclei; i++ ) {
      const int n = queues[i].ntask();
      ntasks += n > 0 ? n : 0;
    }

  }

}



