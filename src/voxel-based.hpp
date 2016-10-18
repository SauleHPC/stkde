#include <cstdio>
#include <iostream>
#include "types.hpp"
#include "io.hpp"


std::shared_ptr<util::Compact3D<values>> stkde_voxelbased(const bounding_box& bb,
				 const instance& inst,
				 const parameters& pa) {
  int xmax = (bb.xh - bb.xl) / pa.xres;
  int ymax = (bb.yh - bb.yl) / pa.yres;
  int zmax = (bb.th - bb.tl) / pa.tres;  
  for(int i = 0; i <= xmax; i++) {
    for(int j = 0; j <= ymax; j++) {
      for(int k = 0; k <= zmax; k++) {

      }
    } 
  }
  
}

