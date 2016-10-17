#ifndef POINT_BASED_SYMOMP_OBSDECOMP_HPP
#define POINT_BASED_SYMOMP_OBSDECOMP_HPP

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
#include "omp.h"


std::shared_ptr<util::Compact3D<values>> stkde_pointbased_symomp_obsdecomp(const bounding_box& bb,
									   const instance& inst,
									   const parameters& pa,
									   index decompsizeX,
									   index decompsizeY,
									   index decompsizeT
) {
  computation c;
  
  c.voxX = std::lround(std::ceil((bb.xh-bb.xl)/pa.xres))+1;
  c.voxY = std::lround(std::ceil((bb.yh-bb.yl)/pa.yres))+1;
  c.voxT = std::lround(std::ceil((bb.th-bb.tl)/pa.tres))+1;

  c.voxsbw = std::lround(std::ceil(pa.xbw/pa.xres));
  c.voxtbw = std::lround(std::ceil(pa.tbw/pa.tres));

  c.totalobs = inst.obsx.size();
  c.bb = bb;
  c.pa = pa;

  {
    bool decompchanged = false;
    
    while (decompsizeX >= (bb.xh-bb.xl)/(2*pa.xbw) && decompsizeX>1) {
      decompsizeX --;
      decompchanged = true;
    }
    while (decompsizeY >= (bb.yh-bb.yl)/(2*pa.ybw) && decompsizeY>1) {
      decompsizeY --;
      decompchanged = true;
    }
    while (decompsizeT >= (bb.th-bb.tl)/(2*pa.tbw) && decompsizeT>1) {
      decompsizeT --;
      decompchanged = true;
    }
    if (decompchanged) {
      std::cerr<<"decomposition should allow blocks larger than bandwidth. Adjusting decomposition to "
	       <<decompsizeX<<"x"<<decompsizeY<<"x"<<decompsizeT
	       <<std::endl;
    }

  }
  
  std::cerr.precision(9);
    
  std::cerr<<"voxsize: "<<c.voxX<<"x"<<c.voxY<<"x"<<c.voxT<<" size:"<<c.voxX*c.voxY*c.voxT*sizeof(values)/1024./1024.<<"MB"<<std::endl;
  std::cerr<<"voxBW: "<<c.voxsbw<<" "<<c.voxtbw<<std::endl;
  
  c.p = std::make_shared<util::Compact3D<values>>(c.voxX, c.voxY, c.voxT);

  util::Compact3D<values>& co = *(c.p);


  init_stkde(c);
  
  long int eval = 0;


  util::timestamp decbeg;
  //decomposition
  
  util::Compact3D<index> load (decompsizeX, decompsizeY, decompsizeT);

  for (int dx = 0; dx<decompsizeX; ++dx) {
    for (int dy = 0; dy<decompsizeY; ++dy) {
      for (int dt = 0; dt<decompsizeT; ++dt) {
	load(dx,dy,dt) = 0;
      }
    }
  }
  
    
  util::Compact3D<std::vector<coordinate>> decompX (decompsizeX, decompsizeY, decompsizeT);
  util::Compact3D<std::vector<coordinate>> decompY (decompsizeX, decompsizeY, decompsizeT);
  util::Compact3D<std::vector<coordinate>> decompT (decompsizeX, decompsizeY, decompsizeT);

  //
  //  long inter = 0;
  for (int i=0; i< inst.obsx.size(); ++i) {
    auto ox = inst.obsx[i];
    auto oy = inst.obsy[i];
    auto ot = inst.obst[i];


    index dx = (ox-bb.xl)/(bb.xh-bb.xl)*decompsizeX;
    index dy = (oy-bb.yl)/(bb.yh-bb.yl)*decompsizeY;
    index dt = (ot-bb.tl)/(bb.th-bb.tl)*decompsizeT;

    //handling points out of rnage. Processing them with near by boundary
    dx = std::max(dx, (index)0);
    dy = std::max(dy, (index)0);
    dt = std::max(dt, (index)0);
    dx = std::min(dx, decompsizeX-1);
    dy = std::min(dy, decompsizeY-1);
    dt = std::min(dt, decompsizeT-1);
    
    //it does intersect
    load(dx,dy,dt) ++;
    //inter++;
    decompX(dx,dy,dt).push_back(ox);
    decompY(dx,dy,dt).push_back(oy);
    decompT(dx,dy,dt).push_back(ot);

  }
  //std::cerr<<"intersect: "<<inter<<std::endl;
  util::timestamp decend;

  std::cerr<<"decomposition time: "<<decend-decbeg<<" seconds"<<std::endl;
  index mostloaded = 0;
  index totalload = 0;
  for (int dt = 0; dt<decompsizeT; ++dt) {
    for (int dy = 0; dy<decompsizeY; ++dy) {
      for (int dx = 0; dx<decompsizeX; ++dx) {
	mostloaded = std::max(mostloaded, load(dx,dy,dt));
	totalload += load(dx,dy,dt);
     	//std::cerr<<load(dx,dy,dt)<<" ";
	if (load(dx,dy,dt) > ((double)inst.obsx.size())/omp_get_max_threads()/10)
	  std::cerr<<dx<<","<<dy<<","<<dt<<" : "<<load(dx,dy,dt)<<std::endl;
      }
      // std::cerr<<std::endl;
     }
    //   std::cerr<<std::endl;
   }
  std::cerr<<"max load: "<<mostloaded<<" avgloadperbox: "<<((double)inst.obsx.size())/(decompsizeX*decompsizeY*decompsizeT)<<" avgloadpercore: "<<((double)inst.obsx.size())/omp_get_max_threads()<<" total load: "<<totalload<<std::endl;


  util::timestamp computebeg;
  //compute
  index maxobs = 0;
#pragma omp parallel reduction(+:eval)
  {
    
    util::Compact2D<values> disk (c.voxX, c.voxY); //naive version of symmetry uses a disk buffer of the map size

    std::vector<values> bar(c.voxT);

  //
  
    for (int xbase=0; xbase < 2; ++xbase) {
      for (int ybase=0; ybase < 2; ++ybase) {
	for (int tbase=0; tbase < 2; ++tbase) {
	  
#pragma omp for collapse(3) schedule(dynamic,1) reduction(max:maxobs)
	  for (int dx = xbase; dx<decompsizeX; dx+=2) {
	    for (int dy = ybase; dy<decompsizeY; dy+=2) {
	      for (int dt = tbase; dt<decompsizeT; dt+=2) {

		maxobs = std::max(maxobs, load(dx,dy,dt));
		
		index ret =  process_observation_boxed_sym (c, //comp
							    0, c.voxX, 0, c.voxY, 0, c.voxT, //box
							    decompX(dx,dy,dt), decompY(dx,dy,dt), decompT(dx,dy,dt), //observations
							    disk, bar //workbuffer
							    );
		eval +=  ret;
	      }
	    }
	  }
#pragma omp master
	  {
	    std::cerr<<"maxin round:"<<maxobs<<std::endl;
	    maxobs = 0;
	  }
#pragma omp barrier


	}
      }
    }
  }
  util::timestamp computeend;

  std::cerr<<"compute: "<<computeend-computebeg<<" seconds"<<std::endl;
  
  // eval +=  process_observation_boxed_sym (c, //comp
  // 					  0, c.voxX, 0, c.voxY, 0, c.voxT, //box
  // 					  inst.obsx, inst.obsy, inst.obst, //observations
  // 					  disk, bar //workbuffer
  // 					  );

  
  std::cerr<<"evaluations: "<<eval<<std::endl;
  
  return c.p;
}


#endif
