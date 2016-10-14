#ifndef POINT_BASED_SYMOMP_HPP
#define POINT_BASED_SYMOMP_HPP

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

struct computation {
  //size of the space in voxels
  index voxX;
  index voxY;
  index voxT;

  //span of bandwidth in voxels
  index voxsbw;
  index voxtbw;

  //return
  std::shared_ptr<util::Compact3D<values>> p;

  //total obs and box
  index totalobs;
  bounding_box bb;

  //parameters
  parameters pa;
};


void init_stkde (computation& c) {
  util::Compact3D<values>& co = *(c.p);

  util::timestamp init_b;
  
  //fill in zeroes
  for (index i=0; i< co.getSizeX(); ++i)
    for (index j=0; j< co.getSizeY(); ++j)
      for (index k=0; k< co.getSizeZ(); ++k)
	co(i,j,k) = 0;

  util::timestamp init_e;
  
  std::cerr<<"Initialization time: "<<init_e-init_b<<std::endl;

}

long process_observation_boxed_sym (computation& c,
				    index voxXmin, index voxXmax,//bounding box [min;max[
				    index voxYmin, index voxYmax,
				    index voxTmin, index voxTmax,
				    const std::vector<coordinate>& obsx,//observations
				    const std::vector<coordinate>& obsy,
				    const std::vector<coordinate>& obst,
				    util::Compact2D<values>& bufferdisk,//buffer
				    std::vector<values>& bufferbar
				    ) {
  long eval = 0;

  util::Compact3D<values>& co = *(c.p);
  
//account for observations
  for (index ob=0; ob<obsx.size(); ++ob) {
    //observation
    coordinate ox = obsx[ob];
    coordinate oy = obsy[ob];
    coordinate ot = obst[ob];

    //voxel containing the observation
    int obsvx = (ox - c.bb.xl)/c.pa.xres;
    int obsvy = (oy - c.bb.yl)/c.pa.yres;
    int obsvt = (ot - c.bb.tl)/c.pa.tres;

    //std::cerr<<"obsv: "<<obsvx<<" "<<obsvy<<" "<<obsvt<<std::endl;


    for (index i = std::max(obsvx - c.voxsbw, (index)0); i< std::min(obsvx + c.voxsbw, c.voxX); ++i) {
      for (index j = std::max(obsvy - c.voxsbw, (index)0); j< std::min(obsvy + c.voxsbw, c.voxY); ++j) {
	coordinate vox_x = c.bb.xl + i*c.pa.xres;
	coordinate vox_y = c.bb.yl + j*c.pa.yres;

	if (std::sqrt((ox - vox_x)*(ox - vox_x) + (oy - vox_y)*(oy - vox_y)) <= c.pa.xbw ) {
	  bufferdisk[i][j]  = densityF_disk(ox, oy, ot,
					    vox_x, vox_y, -1,
					    c.totalobs, c.pa.xbw, c.pa.tbw);
	} else {
	  bufferdisk [i][j] = 0.;
	}
      }
    }

    for (index k = std::max(obsvt - c.voxtbw, (index)0); k< std::min(obsvt + c.voxtbw, c.voxT); ++k) {
      coordinate vox_t = c.bb.tl + k*c.pa.tres;
      
      if (std::abs(vox_t - ot) <= c.pa.tbw) { //is this test even necessary?
	
	bufferbar[k] = densityF_bar(ox, oy, ot,
				    -1, -1, vox_t,
				    c.totalobs, c.pa.xbw, c.pa.tbw);
      }
    }
    
    
    //BW around observation
    for (index i = std::max(obsvx - c.voxsbw, (index)0); i< std::min(obsvx + c.voxsbw, c.voxX); ++i) {
      for (index j = std::max(obsvy - c.voxsbw, (index)0); j< std::min(obsvy + c.voxsbw, c.voxY); ++j) {
	
	for (index k = std::max(obsvt - c.voxtbw, (index)0); k< std::min(obsvt + c.voxtbw, c.voxT); ++k) {
	  values val = bufferdisk[i][j]*bufferbar[k];
	  
	  //std::cerr<<vox_x<<" "<<vox_y<<" "<<vox_t<<" "<<val<<std::endl;
	  
	  co(i,j,k) += val;
	  //eval++;
	}
	
      }
    }
  }

  return eval;  
}


std::shared_ptr<util::Compact3D<values>> stkde_pointbased_symomp(const bounding_box& bb,
								 const instance& inst,
								 const parameters& pa) {
  computation c;
  
  c.voxX = std::lround(std::ceil((bb.xh-bb.xl)/pa.xres))+1;
  c.voxY = std::lround(std::ceil((bb.yh-bb.yl)/pa.yres))+1;
  c.voxT = std::lround(std::ceil((bb.th-bb.tl)/pa.tres))+1;

  c.voxsbw = std::lround(std::ceil(pa.xbw/pa.xres));
  c.voxtbw = std::lround(std::ceil(pa.tbw/pa.tres));

  c.totalobs = inst.obsx.size();
  c.bb = bb;
  c.pa = pa;
  
  std::cerr.precision(4);
    
  std::cerr<<"voxsize: "<<c.voxX<<"x"<<c.voxY<<"x"<<c.voxT<<" size:"<<c.voxX*c.voxY*c.voxT*sizeof(values)/1024./1024.<<"MB"<<std::endl;
  std::cerr<<"voxBW: "<<c.voxsbw<<" "<<c.voxtbw<<std::endl;
  
  c.p = std::make_shared<util::Compact3D<values>>(c.voxX, c.voxY, c.voxT);

  util::Compact3D<values>& co = *(c.p);

  util::Compact2D<values> disk (c.voxX, c.voxY); //naive version of symmetry uses a disk buffer of the map size
  std::vector<values> bar(c.voxT);
  
  init_stkde(c);
  
  long int eval = 0;

  
  eval +=  process_observation_boxed_sym (c, //comp
					  0, c.voxX, 0, c.voxY, 0, c.voxT, //box
					  inst.obsx, inst.obsy, inst.obst, //observations
					  disk, bar //workbuffer
					  );
  
  std::cerr<<"evaluations: "<<eval<<std::endl;
  
  return c.p;
}


#endif
