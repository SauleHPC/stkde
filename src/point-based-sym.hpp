#ifndef POINT_BASED_SYM_HPP
#define POINT_BASED_SYM_HPP

#include <iostream>
#include <cmath>
#include <fstream>
#include <assert.h>
#include "types.hpp"
#include "io.hpp"
#include <limits>
#include "compact.hpp"
#include <memory>
#include "timestamp.hpp"
#include "density.hpp"

namespace stkde {

std::shared_ptr<util::Compact3D<stkde::values>> stkde_pointbased_sym(const stkde::bounding_box& bb,
								     const stkde::instance& inst,
								     const stkde::parameters& pa) {
  stkde::index voxX = std::lround(std::ceil((bb.xh-bb.xl)/pa.xres))+1;
  stkde::index voxY = std::lround(std::ceil((bb.yh-bb.yl)/pa.yres))+1;
  stkde::index voxT = std::lround(std::ceil((bb.th-bb.tl)/pa.tres))+1;

  stkde::index voxsbw = std::lround(std::ceil(pa.xbw/pa.xres));
  stkde::index voxtbw = std::lround(std::ceil(pa.tbw/pa.tres));

  std::cerr.precision(4);
    
  std::cerr<<"voxsize: "<<voxX<<"x"<<voxY<<"x"<<voxT<<" size:"<<voxX*voxY*voxT*sizeof(stkde::values)/1024./1024.<<"MB"<<std::endl;
  std::cerr<<"voxBW: "<<voxsbw<<" "<<voxtbw<<std::endl;
  
  std::shared_ptr<util::Compact3D<stkde::values>> p = std::make_shared<util::Compact3D<stkde::values>>(voxX, voxY, voxT);

  util::Compact3D<stkde::values>& co = *p;

  util::Compact2D<stkde::values> disk (voxX, voxY); //naive version of symmetry uses a disk buffer of the map size
  std::vector<stkde::values> bar(voxT);
  
  util::timestamp init_b;
  
  //fill in zeroes
  // for (index k=0; k< co.getSizeZ(); ++k)
  //   for (index j=0; j< co.getSizeY(); ++j)
  //     for (index i=0; i< co.getSizeX(); ++i)
  //	co(i,j,k) = 0;
  co.zero();
  
  util::timestamp init_e;

  std::cerr<<"Initialization time: "<<init_e-init_b<<std::endl;


  util::timestamp computebeg;
  long int eval = 0;
    
  //account for observations
  for (stkde::index ob=0; ob<inst.obsx.size(); ++ob) {
    //observation
    stkde::coordinate ox = inst.obsx[ob];
    stkde::coordinate oy = inst.obsy[ob];
    stkde::coordinate ot = inst.obst[ob];

    //voxel containing the observation
    int obsvx = (ox - bb.xl)/pa.xres;
    int obsvy = (oy - bb.yl)/pa.yres;
    int obsvt = (ot - bb.tl)/pa.tres;

    //std::cerr<<"obsv: "<<obsvx<<" "<<obsvy<<" "<<obsvt<<std::endl;


    for (stkde::index i = std::max(obsvx - voxsbw, (stkde::index)0); i< std::min(obsvx + voxsbw+1, voxX); ++i) {
      for (stkde::index j = std::max(obsvy - voxsbw, (stkde::index)0); j< std::min(obsvy + voxsbw+1, voxY); ++j) {
	stkde::coordinate vox_x = bb.xl + i*pa.xres;
	stkde::coordinate vox_y = bb.yl + j*pa.yres;

	if (std::sqrt((ox - vox_x)*(ox - vox_x) + (oy - vox_y)*(oy - vox_y)) <= pa.xbw ) {
	  disk[i][j]  = densityF_disk(ox, oy, ot,
				      vox_x, vox_y, -1,
				      inst.obsx.size(), pa.xbw, pa.tbw);
	} else {
	  disk [i][j] = 0.;
	}
      }
    }

    for (stkde::index k = std::max(obsvt - voxtbw, (stkde::index)0); k< std::min(obsvt + voxtbw+1, voxT); ++k) {
      stkde::coordinate vox_t = bb.tl + k*pa.tres;
      
      if (std::abs(vox_t - ot) <= pa.tbw) {
	
	bar[k] = densityF_bar(ox, oy, ot,
			      -1, -1, vox_t,
			      inst.obsx.size(), pa.xbw, pa.tbw);
      }
    }
    
    
    //BW around observation
    for (stkde::index i = std::max(obsvx - voxsbw, (stkde::index)0); i< std::min(obsvx + voxsbw+1, voxX); ++i) {
      for (stkde::index j = std::max(obsvy - voxsbw, (stkde::index)0); j< std::min(obsvy + voxsbw+1, voxY); ++j) {
	
	for (stkde::index k = std::max(obsvt - voxtbw, (stkde::index)0); k< std::min(obsvt + voxtbw+1, voxT); ++k) {
	  stkde::values val = disk[i][j]*bar[k];
	  
	  //std::cerr<<vox_x<<" "<<vox_y<<" "<<vox_t<<" "<<val<<std::endl;
	  
	  co(i,j,k) += val;
	  eval++;
	}
	
      }
    }
  }
  util::timestamp computeend;
  std::cerr<<"compute: "<<computeend-computebeg<<" seconds"<<std::endl;

  std::cerr<<"evaluations: "<<eval<<std::endl;
  
  return p;
}

}
#endif
