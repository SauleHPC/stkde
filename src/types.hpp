#ifndef TYPES_HPP_
#define TYPES_HPP_

typedef double coordinate;
typedef float values;

struct bounding_box {
  coordinate xl, xh;
  coordinate yl, yh;
  coordinate tl, th;
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
