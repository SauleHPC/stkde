#ifndef TYPES_HPP_
#define TYPES_HPP_

#include <vector>

typedef long index;
typedef double coordinate;
typedef float values;

struct bounding_box {
  coordinate xl, xh;
  coordinate yl, yh;
  coordinate tl, th;
};

struct instance {
  std::vector<coordinate> obsx;
  std::vector<coordinate> obsy;
  std::vector<coordinate> obst;
};

struct parameters {
  coordinate xres;
  coordinate yres;
  coordinate tres;

  coordinate xbw;
  coordinate ybw;
  coordinate tbw;  
};

#endif
