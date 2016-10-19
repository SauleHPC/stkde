#include <cstdio>
#include <cmath>
#include <iostream>
#include "types.hpp"
#include "io.hpp"
#include <memory>
#include "density.hpp"

std::shared_ptr<util::Compact3D<values>> stkde_voxelbased(const bounding_box& bb,
				 const instance& inst,
				 const parameters& pa) {

  int voxx = (bb.xh - bb.xl) / pa.xres;
  int voxy = (bb.yh - bb.yl) / pa.yres;
  int voxt = (bb.th - bb.tl) / pa.tres;

  int n = inst.obsx.size();
 
  coordinate px, py, pt;
  coordinate ox, oy, ot;
 
  long int evals = 0;
  
  util::timestamp t1;

  std::shared_ptr<util::Compact3D<values>> p = std::make_shared<util::Compact3D<values>>(voxx, voxy, voxt);
  util::Compact3D<values>& co = *p;
  co.zero();

  for(int i = 0; i <= voxx; i++) {
    for(int j = 0; j <= voxy; j++) {
      for(int k = 0; k <= voxt; k++) {
	// contruct the point
	px = bb.xl + i * pa.xres;
	py = bb.yl + j * pa.yres;
	pt = bb.tl + k * pa.tres;

	// for every voxel do every observation point
	for(int o = 0; o < n; o++) {
	  ox = inst.obsx[o];
	  oy = inst.obsy[o];
	  ot = inst.obst[o];	  
	  if(abs(pt - ot) <= pa.tbw) {
	    if(sqrt((px - ox) * (px - ox) + (py - oy) * (py - oy)) <= pa.xbw) {
	      co(i, j, k) += densityF(px, py, pt, ox, oy, ot, n, pa.xbw, pa.tbw);
	      evals++;
	    }
	  }
	}
      }
    }
  }
  
  util::timestamp t2;
  std::cout<<"compute: " << t2 - t1 << std::endl;
  std::cout<<"evals: "<< evals << std::endl;
  return p;
}

