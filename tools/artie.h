#ifndef artie
#define artie

///////////////////////////////////////////////////////
// Turk Terrain Analysis - TTA
// C S Cowden    December 18, 2017
// ARTIE: http://terminator.wikia.com/wiki/ARTIE
///////////////////////////////////////////////////////



#include <map>
#include <vector>
#include <cmath>


namespace Turk {

struct color {

  char red;
  char green;
  char blue;

  color(char r, char g, char b): red(r), green(g), blue(b) { }
  color() { }

};

color gen_random_color();


struct point {

  point(): x(-1),y(-1) { }
  point(const int px, const int py): x(px),y(py) { }

  point operator+(const point &pt) const { return point(x+pt.x,y+pt.y); }
  point operator-(const point &pt) const { return point(x-pt.x,y-pt.y); }

  int length() const { return sqrt(x*x+y*y); }
  
  int x;
  int y;

};

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
    if ( m_obstacles ) delete m_obstacles;
    if ( m_walkable ) delete m_walkable;
    if ( m_dmap ) delete m_dmap;
    if ( m_wlmap ) delete m_wlmap;
    if ( m_gatepoints ) delete m_gatepoints;
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
  virtual void dump_image(const char * name, const bool *map, const unsigned width, const unsigned height);
  virtual void dump_image(const char * name, const unsigned *map, const unsigned width, const unsigned height);

  /**
  * dump a region map
  * assign a random color for each unique category
  */
  virtual void dump_categorical_map(const char * name, const unsigned *map, const unsigned width, const unsigned height);


  /**
  * Return the distance map
  */
  virtual void get_distance_map();
  
  /**
  * Return the chokepoints.
  */
  virtual void get_chokes();

private:


  /**
  * Build the dstance map from the walkable map
  */
  virtual void build_distance_map();

  virtual void refine_distance_map();


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

  inline virtual void rfill_area(const unsigned index, const unsigned counter, unsigned * map, const bool * exclude) {
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

  /**
  * check if the point is a valid point (i.e. in range of the map size.)
  */
  bool isValidPoint(const point & pt) { 
    bool test = composeIndex(pt) < m_mapsize && pt.x < m_width && pt.y < m_height;

    return test;
  }




  // private members
  // map parameters
  unsigned m_width;
  unsigned m_height;
  unsigned m_mapsize;

  // obstacle map
  // map each obstructed tile to obstacle cluster
  unsigned * m_obstacles;
  unsigned m_nObstacles;

  // walkable map
  // a boolean of whether a tile is traversable
  bool * m_walkable;
  unsigned m_nWalkable;

  // distance graph
  // lookup table of tile to tile distances
  std::vector<std::vector<unsigned> > m_dgraph;

  // distance map
  // distance to nearest obstacle
  unsigned * m_dmap;
  std::vector<unsigned> m_nnobj;
  
  // reverse distance map
  // map depth (distance) to set of indices in map tiles
  std::map<unsigned,std::vector<unsigned> > m_rdmap;

  // water level region map
  // region label at each tile of the map
  unsigned * m_wlmap;
  

  // gateway/choke points
  unsigned * m_gatepoints;
  bool * m_gatepointmap;
  unsigned m_nGatePoints;  

  std::vector<unsigned> tmp;
  
};


}
#endif
