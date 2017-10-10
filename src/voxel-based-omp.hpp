#ifndef VOXEL_BASED_OMP_HPP
#define VOXEL_BASED_OMP_HPP

#include <cstdio>
#include <cmath>
#include <iostream>
#include "types.hpp"
#include "io.hpp"
#include <memory>
#include "density.hpp"

namespace stkde {

std::shared_ptr<util::Compact3D<values>> stkde_voxelbased_omp(const bounding_box& bb,
				 const instance& inst,
				 const parameters& pa) {
  
  // std::cout << "Running the voxel based implementation." << std::endl;
  index voxx = std::lround(std::ceil((bb.xh - bb.xl) / pa.xres)) + 1;
  index voxy = std::lround(std::ceil((bb.yh - bb.yl) / pa.yres)) + 1;
  index voxt = std::lround(std::ceil((bb.th - bb.tl) / pa.tres)) + 1;

  long n = inst.obsx.size();
 
  long int evals = 0;

  std::shared_ptr<util::Compact3D<values>> p = std::make_shared<util::Compact3D<values>>(voxx, voxy, voxt);
  util::Compact3D<values>& co = *p;
  //co.zero(); //This is unnecessary since the correct value is directly written

#pragma omp parallel for collapse(3) schedule(dynamic,2048) reduction(+:evals)
  for(index i = 0; i < voxx; i++) {
    for(index j = 0; j < voxy; j++) {
      for(index k = 0; k < voxt; k++) {
	
	// contruct the point
	coordinate px = bb.xl + i * pa.xres;
	coordinate py = bb.yl + j * pa.yres;
	coordinate pt = bb.tl + k * pa.tres;

	values v = 0;
	
	// for every voxel do every observation point
	for(int o = 0; o < n; o++) {
	  coordinate ox = inst.obsx[o];
	  coordinate oy = inst.obsy[o];
	  coordinate ot = inst.obst[o];	  
	  if(std::abs(pt - ot) <= pa.tbw) {
	    if(std::sqrt((px - ox) * (px - ox) + (py - oy) * (py - oy)) <= pa.xbw) {
	      v += densityF(px, py, pt,
				      ox, oy, ot,
				      n, pa.xbw, pa.tbw);
	      evals++;
	    }
	  }
	}

	//write back to memory
	co(i, j, k) = v;
      }
    }
  }
  
  std::cout<<"evals: "<< evals << std::endl;
  return p;
}
}

#endif
