#include <iostream>
#include <cmath>
#include <fstream>
#include <assert.h>
#include "types.hpp"
#include "io.hpp"
#include <limits>

int main (int argc, char* argv[]) {

  std::string bbfile = argv[1];
  std::string obsfile = argv[2];
  
  bounding_box bb = load_bounding_box(bbfile);

  std::cerr<<"bounding: "
	   <<"["<<bb.xl<<";"<<bb.xh<<"]"<<"x"
    	   <<"["<<bb.yl<<";"<<bb.yh<<"]"<<"x"
    	   <<"["<<bb.tl<<";"<<bb.th<<"]"<<std::endl;

  std::vector<coordinate> obsx;
  std::vector<coordinate> obsy;
  std::vector<coordinate> obst;

  load_observations (obsfile, obsx, obsy, obst);

  //show first 10 observation
  std::cerr<<"first 10 observation"<<std::endl;
  std::cerr.precision(std::numeric_limits< coordinate > ::max_digits10);
  for (int i=0; i<std::min((size_t)10, (size_t)obsx.size()); ++i) {
    std::cerr<<obsx[i]<<" "
	     <<obsy[i]<<" "
	     <<obst[i]<<std::endl;
  }

    
  return 0;
}
