#ifndef ARTIE_H
#define ARTIE_H

///////////////////////////////////////////////////////
// Turk Terrain Analysis - TTA
// C S Cowden    December 18, 2017
// ARTIE: http://terminator.wikia.com/wiki/ARTIE
///////////////////////////////////////////////////////



#include <map>
#include <vector>
#include <cmath>
#include <fstream>

#ifdef _WIN32
#include <boost/random/linear_congruential.hpp>
#endif

#include <boost/serialization/vector.hpp>
#include <boost/serialization/map.hpp>




namespace Turk {

struct color {

  char red;
  char green;
  char blue;

  color(char r, char g, char b): red(r), green(g), blue(b) { }
  color() { }

};

color gen_random_color();


/**
* point - 
* helper struct to contain x,y coordinates and 
* perform necessary vector algebra.
*/
struct point {

  point(): x(-1),y(-1) { }
  point(const int px, const int py): x(px),y(py) { }

  point operator+(const point &pt) const { return point(x+pt.x,y+pt.y); }
  point operator-(const point &pt) const { return point(x-pt.x,y-pt.y); }
  
  /**
  * dot product
  */
  int operator*(const point &pt) const { return x*pt.x+y*pt.y; }

  /**
  * scale the point
  */
  point operator*(unsigned s) const { return point(s*x,s*y); }


  /**
  * Return the Euclidean norm of the vector.
  */
  int length() const { return sqrt(x*x+y*y); }

  /**
  * Return the L1 norm (sum of components) of the vector.
  */
  int l1length() const { return abs(x)+abs(y); }
  
  int x;
  int y;

private:
  friend class boost::serialization::access;
  template<class S>
  void serialize( S & ar, const unsigned version) {
    ar & x;
    ar & y;
  }

};


// =====================================================
// Region

struct region {

  /**
  * default constructor
  */
  region():index_(UINT_MAX) { }

  /**
  * construct a region with the necessary information.
  * index
  * position (x,y)
  * area
  * depth
  * articulation
  * choke
  * neighbors
  */
  region(unsigned idx
    ,unsigned x, unsigned y
    ,unsigned area
    ,unsigned depth
    ,bool artic
    ,bool choke
    ,const std::vector<unsigned> & neighbs ):
    index_(idx),x_(x),y_(y),area_(area)
    ,depth_(depth),articulation_(artic),choke_(choke)
    ,neighbors_(neighbs) { }

  /**
  * copy constructor
  */
  region( const region & reg):
    index_(reg.index_), x_(reg.x_), y_(reg.y_)
    ,area_(reg.area_), depth_(reg.depth_)
    ,articulation_(reg.articulation_), choke_(reg.choke_)
    ,neighbors_(reg.neighbors_) { }

  // ------ accessor methods ----

  /**
   * return the ARTIE index of this region.
  */
  unsigned index() const { return index_; }

  /**
  * return the position of the centroid
  */
  point position() const { return point(x_,y_); }

  /**
  * return the area of the region
  */
  unsigned area() const { return area_; }

  /**
  * return the depth of the centroid
  */
  unsigned depth() const { return depth_; }

  /**
  * return an indicator if this region is an articulation point.
  */
  bool is_articulation() const { return articulation_; }

  /**
  * return an indicator if this is choke point.
  */
  bool is_choke() const { return choke_; }

  /**
  * return a list of neighbors.
  */
  const std::vector<unsigned> & neighbors() const { return neighbors_; }


  // ------- members --------

  unsigned index_;
  unsigned x_, y_;
  unsigned area_;
  unsigned depth_;
  bool articulation_;
  bool choke_;
  std::vector<unsigned> neighbors_;


private:

  friend class boost::serialization::access;
  template<class S>
  void serialize(S & ar, const unsigned version ) {

    ar & index_;
    ar & x_;
    ar & y_;
    ar & area_;
    ar & depth_;
    ar & articulation_;
    ar & choke_;
    ar & neighbors_;

  }

};


//========================================================
// ARTIE
//

class ARTIE {
public:

  /**
  * Default constructor
  * Initialize various maps to NULL
  */
  ARTIE():m_obstacles(NULL),m_walkable(NULL),m_dmap(NULL),m_wlmap(NULL),m_gatepoints(NULL) {}


  /**
  * Destructor
  */
  ~ARTIE() {
  }

  /**
  * load a new map.
  * Copy the data provided in map to an internal storage.
  */
  virtual void load_map(const unsigned * map, const unsigned width, const unsigned height);

  /**
  * analyze the map data.
  * * build the distance map
  * * detect chokepoints
  */
  virtual void analyze_map();

  /**
  * Dump relevent data and images using the base file name provides.
  */
  virtual void dump_data(const char * base_name);

  /**
  * dump the map to an image (PPM) file.
  */
  virtual void dump_image(const char * name, const std::vector<bool> & map, const unsigned width, const unsigned height);
  virtual void dump_image(const char * name, const unsigned *map, const unsigned width, const unsigned height);

  /**
  * dump a region map
  * assign a random color for each unique category
  */
  virtual void dump_categorical_map(const char * name, const unsigned *map, const unsigned width, const unsigned height);

  /**
  * dump a csv file 
  */
  virtual void dump_points(const char * name, const point * points, const unsigned len);
  template<class T> void dump_points(const char * name, const std::vector<std::pair<T,T> > & points);

  /**
  * dump a csv file
  */
  template<class T> void dump_csv(const char * name, const T * data, const unsigned n_features, const unsigned n_obs);


  // ---------------------------
  // accessor methods

  // --
  // map pixel to region
  /**
  * Return the region information given the walk tile (8x8 pixels)
  * location.
  */
  inline virtual region get_region(unsigned x, unsigned y) {
    
    // look up the region
    // construct the map index
    const unsigned index = composeIndex(x,y);
    const unsigned reg = m_region_map[index];
 
    // return the region
    return m_regions[reg];

  }

  /** 
  * Return the region information given the walk tile by BWAPI::WalkPosition
  */
  inline virtual region get_region(const point & pt) {
    
    // look up the region
    // construct the map index
    const unsigned index = composeIndex(pt);
    const unsigned reg = m_region_map[index];
 
    // return the region
    return m_regions[reg];
  }

  /**
  * Return a region by index.
  */
  inline virtual region operator[](const unsigned i) const {
    if ( m_regions.size() ) 
      return m_regions[i];
    else
      return region();
  }


  /**
  * Get the map width
  */
  inline virtual unsigned width() const { return m_width; }

  /**
  * Get the map height
  */
  inline virtual unsigned height() const { return m_height; }

  /**
  * Return the distance map
  */
  inline virtual const std::vector<unsigned> & get_distance_map() const { return m_dmap; }

  
  /**
  * Return a list of choke indicators for all nuclei.
  */
  inline virtual const std::vector<unsigned> & get_chokes() const { return m_choke_points; }


private:

  friend class boost::serialization::access;
  template<class S>
  void serialize(S & ar, const unsigned version ) {
    ar & m_width;
    ar & m_height;
    ar & m_mapsize;

    ar & m_obstacles;
    ar & m_nObstacles;

    ar & m_walkable;
    ar & m_nWalkable;

    ar & m_dmap;
    ar & m_nnobj;

    ar & m_rdmap;

    ar & m_wlmap;
    
    ar & m_gatepoints;
    ar & m_gatepointmap;
    ar & m_nGatePoints;

    ar & m_walkable_areas;
   
    ar & m_critical_points;
    ar & m_critical_mins;
    ar & m_critical_clus;
    ar & m_clu_labels;

    ar & m_n_nuclei;
    ar & m_region_map;
    ar & m_map_graph;
    ar & m_region_areas;

    ar & m_articulation_points;
    ar & m_choke_points;
  
    ar & m_regions;

  }


  /**
  * Build the dstance map from the walkable map
  */
  virtual void build_distance_map();

  /**
  * Apply the medial axis transformation
  */
  virtual void mat();


  /**
  * determine choke point candidates by water level decomposition.
  */
  virtual void water_level_decomposition();

  /**
  * flood fill obstacles (non-walkable areas)
  */
  virtual void obstacle_flood_fill();

  /**
  * clean walkable map by removing tiny isolated walkable areas
  */
  virtual void clean_map();

  /**
  * Like flood fill, but start the fill from a set of starting points.  This 
  * process is similar to crystal nucleation.  The algorithm works by starting
  * a fill queue for each nucleation point.
  * When two regions intersect the nuclei are linked.  In the absence of obstacles, 
  * this method results in a Delauney triangulation.
  */
  virtual void triangulate();

  /**
  * flood fill an area
  * recursively.
  */
  inline virtual void rfill_area(const unsigned index, const unsigned counter, unsigned * map, const unsigned * exclude) {
    if ( index >= m_mapsize || map[index] ) return;

    if ( exclude[index] ) return;

    map[index] = counter;

    const unsigned x = index % m_width;
    const unsigned y = index / m_width;

    if ( y > 0 ) rfill_area((y-1)*m_width + x, counter,map,exclude);
    if ( y + 1 < m_height ) rfill_area((y+1)*m_width + x, counter,map,exclude);
    if ( x > 0 ) rfill_area(y*m_width + x - 1, counter,map,exclude);
    if ( x + 1 < m_width ) rfill_area(y*m_width + x + 1, counter,map,exclude);

  }

  inline virtual void rfill_area(const unsigned index, const unsigned counter, unsigned * map, const std::vector<bool> & exclude) {
    if ( index >= m_mapsize || map[index] ) return;

    if ( exclude[index] ) return;

    map[index] = counter;

    const unsigned x = index % m_width;
    const unsigned y = index / m_width;

    if ( y > 0 ) rfill_area((y-1)*m_width + x, counter,map,exclude);
    if ( y + 1 < m_height ) rfill_area((y+1)*m_width + x, counter,map,exclude);
    if ( x > 0 ) rfill_area(y*m_width + x - 1, counter,map,exclude);
    if ( x + 1 < m_width ) rfill_area(y*m_width + x + 1, counter,map,exclude);

  }

  /**
  * flood fill area
  * implement a queue based "forrest fire" algorithm.
  */
  void ff_fill_area(const unsigned index, const unsigned counter, unsigned * map, const unsigned * exclude );

  /**
  * distance (like flood fill) fill the area accessible from a given
  * point.  Fill a map of distance to each accessible tile.
  * Set the index and distance to the nearest obstacle.
  */
  void dist_fill_area(const unsigned index);

  /**
  * Find the L1 distance to the nearest obstacle from a given point.
  */
  void dist_nearest_obstacle_l1(const unsigned index); 
  
  /**
  * Find the Euclidean distance to the nearest obstacle from a given point. (L2 distance).
  */
  void dist_nearest_obstacle_l2(const unsigned index);
 

  /**
  * Filter the mat image to remove safe non-end point pixels.
  * The result should approach a single width medial axis.
  */
  void mat_filter();

  /**
  * Return true/false if the MAT point is safe to remove.  
  * Safe points do not create disjoint MAT segments if removed.
  * The input is an 3x3 image segment of the current MAT.
  */
  bool mat_is_safe(const unsigned []);
  
  /**
  * Return true/false if the MAT point is an end point.
  * The input is a 3x3 image segment of the current MAT.
  */
  bool mat_is_end(const unsigned []); 


  /**
  * decompose index to x,y coordinates
  */
  point decomposeIndex(const unsigned index) { return point(index % m_width, index / m_width); }

  /**
  * re-compose index from x,y coordinates
  */
  unsigned composeIndex(const point & pt) { return pt.y*m_width+pt.x; }
  
  unsigned composeIndex(const unsigned x, const unsigned y) { return y*m_width+x;}

  /** 
  * list allowable directions
  */
  static const unsigned m_nNeighbs = 4U;
  static const point m_neighbors[m_nNeighbs];
  static const unsigned m_segsize = 8U;
  static const point m_segment[m_segsize];

  /**
  * check if the point is a valid point (i.e. in range of the map size.)
  */
  bool isValidPoint(const point & pt) { 
    bool test = composeIndex(pt) < m_mapsize && pt.x < m_width && pt.y < m_height;

    return test;
  }


  /**
  * Scan the distance map and identify critical points
  */
  void find_critical_points();


  /**
  * Execute the Tarjan algorithm to identify articulation points
  * in the region graph.
  */
  void tarjan();


  /**
  * Label choke points.
  */
  void label_chokes();

  /**
  * Construct a list of regions (struct).
  */
  void construct_region_list();


  // private members
  // map parameters
  unsigned m_width;
  unsigned m_height;
  unsigned m_mapsize;

  // obstacle map
  // map each obstructed tile to obstacle cluster
  std::vector<unsigned> m_obstacles;
  unsigned m_nObstacles;

  // walkable map
  // a boolean of whether a tile is traversable
  std::vector<bool> m_walkable;
  unsigned m_nWalkable;

  // distance map
  // distance to nearest obstacle
  std::vector<unsigned> m_dmap;
  std::vector<unsigned> m_nnobj;
  
  // reverse distance map
  // map depth (distance) to set of indices in map tiles
  std::map<unsigned,std::vector<unsigned> > m_rdmap;

  // image containing the medial axis transformation
  std::vector<unsigned> m_mat;

  // water level region map
  // region label at each tile of the map
  std::vector<unsigned> m_wlmap;
  

  // gateway/choke points
  std::vector<unsigned> m_gatepoints;
  std::vector<bool> m_gatepointmap;
  unsigned m_nGatePoints;  

  std::vector<unsigned> m_walkable_areas;

  // critical points
  std::vector<point> m_critical_points;
  std::vector<std::pair<double,double> > m_critical_mins;
  std::vector<double> m_critical_clus;
  std::vector<int> m_clu_labels;


  // map regions and connection graph
  unsigned m_n_nuclei;
  std::vector<unsigned> m_region_map;
  std::vector<unsigned> m_map_graph;
  std::vector<unsigned> m_region_areas; 
 
  // post-analysis labels of nodes 
  std::vector<unsigned> m_articulation_points;
  std::vector<unsigned> m_choke_points;

  // final list of regions
  std::vector<region> m_regions;
  
  
};

template<class T> void ARTIE::dump_points(const char *name, const std::vector<std::pair<T,T> > & points) {

  std::ofstream out(name);
  out << "x,y" << std::endl;
  const unsigned len = points.size();
  for ( unsigned i=0; i != len; i++ )
    out << points[i].first << "," << points[i].second << std::endl;

  out.close();

}

template<class T> void ARTIE::dump_csv(const char *name, const T * data, const unsigned n_features, const unsigned n_obs ) {

  std::ofstream out(name);
  for ( unsigned i=0; i != n_obs; i++ ) {
    for ( unsigned j=0; j != n_features; j++ ) {
      out << data[i*n_features+j];
      if ( j != n_features-1 ) out << ", ";
    }
    out << std::endl;
  }

  out.close();
}


}
#endif
