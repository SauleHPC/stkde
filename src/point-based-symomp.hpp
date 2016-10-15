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
#include "omp.h"

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
// #pragma omp parallel for collapse(2) schedule(dynamic,1)
//   for (index i=0; i< co.getSizeX(); ++i)
//     for (index j=0; j< co.getSizeY(); ++j)
//       for (index k=0; k< co.getSizeZ(); ++k)
// 	co(i,j,k) = 0;

  co.zero_parallel();
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
  // std::cerr<<"processing voxel box: "
  // 	   <<"["<< voxXmin<<";"<<voxXmax<<"["
  //   	   <<" ["<< voxYmin<<";"<<voxYmax<<"["
  //   	   <<" ["<< voxTmin<<";"<<voxTmax<<"["<<std::endl;
	
    
  long eval = 0;

  util::Compact3D<values>& co = *(c.p);
  
//account for observations
  for (index ob=0; ob<obsx.size(); ++ob) {
    //observation
    coordinate ox = obsx[ob];
    coordinate oy = obsy[ob];
    coordinate ot = obst[ob];

    //voxel containing the observation
    index obsvx = (ox - c.bb.xl)/c.pa.xres;
    index obsvy = (oy - c.bb.yl)/c.pa.yres;
    index obsvt = (ot - c.bb.tl)/c.pa.tres;

    //std::cerr<<"obsv: "<<obsvx<<" "<<obsvy<<" "<<obsvt<<std::endl;


    for (index i = std::max(obsvx - c.voxsbw, (index)voxXmin); i< std::min(obsvx + c.voxsbw, voxXmax); ++i) {
      for (index j = std::max(obsvy - c.voxsbw, (index)voxYmin); j< std::min(obsvy + c.voxsbw, voxYmax); ++j) {
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

    for (index k = std::max(obsvt - c.voxtbw, (index)voxTmin); k< std::min(obsvt + c.voxtbw, voxTmax); ++k) {
      coordinate vox_t = c.bb.tl + k*c.pa.tres;
      
      if (std::abs(vox_t - ot) <= c.pa.tbw) { //is this test even necessary?
	
	bufferbar[k] = densityF_bar(ox, oy, ot,
				    -1, -1, vox_t,
				    c.totalobs, c.pa.xbw, c.pa.tbw);
      }
    }
    
    
    //BW around observation
    for (index i = std::max(obsvx - c.voxsbw, (index)voxXmin); i< std::min(obsvx + c.voxsbw, voxXmax); ++i) {
      for (index j = std::max(obsvy - c.voxsbw, (index)voxYmin); j< std::min(obsvy + c.voxsbw, voxYmax); ++j) {
	
	for (index k = std::max(obsvt - c.voxtbw, (index)voxTmin); k< std::min(obsvt + c.voxtbw, voxTmax); ++k) {
	  values val = bufferdisk[i][j]*bufferbar[k];
	  
	  //std::cerr<<vox_x<<" "<<vox_y<<" "<<vox_t<<" "<<val<<std::endl;
	  
	  co(i,j,k) += val;
	  eval++;
	}
	
      }
    }
  }

  return eval;  
}

bool intersect1d (coordinate min1, coordinate max1,
		  coordinate min2, coordinate max2) {
  if (max2 < min1) return false;
  if (min2 > max1) return false;

  return true;
}

std::shared_ptr<util::Compact3D<values>> stkde_pointbased_symomp(const bounding_box& bb,
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
  long inter = 0;
  for (int i=0; i< inst.obsx.size(); ++i) {
    auto ox = inst.obsx[i];
    auto oy = inst.obsy[i];
    auto ot = inst.obst[i];

    //place in appropriate decomposition
    for (int dx = 0; dx<decompsizeX; ++dx) {
      coordinate decxmin = bb.xl + ( dx   *(bb.xh-bb.xl)/decompsizeX );
      coordinate decxmax = bb.xl + ((dx+1)*(bb.xh-bb.xl)/decompsizeX );

      coordinate bwxmin = ox-pa.xbw;
      coordinate bwxmax = ox+pa.xbw;
      //does it intersect?
      if (! intersect1d(bwxmin, bwxmax, decxmin, decxmax)) {continue;}
      //this kind of intersection can cause to touch (evaluate) less
      //voxels than the sequential code would. Though it is because of
      //a rounding error in sequential. So it is begnin.
      
      
      for (int dy = 0; dy<decompsizeY; ++dy) {
	coordinate decymin = bb.yl + ( dy   *(bb.yh-bb.yl)/decompsizeY );
	coordinate decymax = bb.yl + ((dy+1)*(bb.yh-bb.yl)/decompsizeY );

	coordinate bwymin = oy-pa.ybw;
	coordinate bwymax = oy+pa.ybw;
	//does it intersect?
	if (! intersect1d(bwymin, bwymax, decymin, decymax)) {continue;}

	
	for (int dt = 0; dt<decompsizeT; ++dt) {
	  coordinate dectmin = bb.tl + ( dt   *(bb.th-bb.tl)/decompsizeT );
	  coordinate dectmax = bb.tl + ((dt+1)*(bb.th-bb.tl)/decompsizeT );

	  coordinate bwtmin = ot-pa.tbw;
	  coordinate bwtmax = ot+pa.tbw;
	  //does it intersect?
	  if (! intersect1d(bwtmin, bwtmax, dectmin, dectmax)) {continue;}

	  if ( i == 0){
	    std::cerr<<"adding "<<ox<<","<<oy<<","<<ot<<" to "
		     <<decxmin<<";"<<decxmax<<" "
		     <<decymin<<";"<<decymax<<" "
		     <<dectmin<<";"<<dectmax<<std::endl;
	  }
	  
	  //it does intersect
	  load(dx,dy,dt) ++;
	  inter++;
	  decompX(dx,dy,dt).push_back(ox);
	  decompY(dx,dy,dt).push_back(oy);
	  decompT(dx,dy,dt).push_back(ot);

	}
      }
    }
  }
  std::cerr<<"intersect: "<<inter<<std::endl;
  util::timestamp decend;

  std::cerr<<"decomposition time: "<<decend-decbeg<<" seconds"<<std::endl;
  index mostloaded = 0;
  for (int dt = 0; dt<decompsizeT; ++dt) {
    for (int dy = 0; dy<decompsizeY; ++dy) {
      for (int dx = 0; dx<decompsizeX; ++dx) {
	mostloaded = std::max(mostloaded, load(dx,dy,dt));
     	//std::cerr<<load(dx,dy,dt)<<" ";
      }
      // std::cerr<<std::endl;
     }
    //   std::cerr<<std::endl;
   }
  std::cerr<<"max load: "<<mostloaded<<" avgloadperbox: "<<((double)inst.obsx.size())/(decompsizeX*decompsizeY*decompsizeT)<<" avgloadpercore: "<<((double)inst.obsx.size())/omp_get_max_threads()<<std::endl;


  util::timestamp computebeg;
  //compute
#pragma omp parallel reduction(+:eval)
  {
    
    util::Compact2D<values> disk (c.voxX, c.voxY); //naive version of symmetry uses a disk buffer of the map size

    std::vector<values> bar(c.voxT);

  //
  
#pragma omp for collapse(3) schedule(dynamic,1)
    for (int dx = 0; dx<decompsizeX; ++dx) {
      
      for (int dy = 0; dy<decompsizeY; ++dy) {
	
	for (int dt = 0; dt<decompsizeT; ++dt) {
	  
	  coordinate decxmin = bb.xl + ( dx   *(bb.xh-bb.xl)/decompsizeX );
	  index voxXmin = std::lround((decxmin-bb.xl)/pa.xres);
	  coordinate decxmax = bb.xl + ((dx+1)*(bb.xh-bb.xl)/decompsizeX );
	  index voxXmax;
	  if (dx != decompsizeX-1)
	    voxXmax = std::lround((decxmax-bb.xl)/pa.xres);
	  else
	    voxXmax = c.voxX;
	  
	  coordinate decymin = bb.yl + ( dy   *(bb.yh-bb.yl)/decompsizeY );
	  index voxYmin = std::lround((decymin-bb.yl)/pa.yres);
	  coordinate decymax = bb.yl + ((dy+1)*(bb.yh-bb.yl)/decompsizeY );
	  index voxYmax;
	  if (dy != decompsizeY-1)
	    voxYmax = std::lround((decymax-bb.yl)/pa.yres);
	  else
	    voxYmax = c.voxY;

	  
	  coordinate dectmin = bb.tl + ( dt   *(bb.th-bb.tl)/decompsizeT );
	  index voxTmin = std::lround((dectmin-bb.tl)/pa.tres);
	  coordinate dectmax = bb.tl + ((dt+1)*(bb.th-bb.tl)/decompsizeT );
	  index voxTmax;
	  if (dt != decompsizeT-1)
	    voxTmax = std::lround((dectmax-bb.tl)/pa.tres);
	  else
	    voxTmax = c.voxT;
	  

	  index ret =  process_observation_boxed_sym (c, //comp
						      voxXmin, voxXmax, voxYmin, voxYmax, voxTmin, voxTmax, //box
						      decompX(dx,dy,dt), decompY(dx,dy,dt), decompT(dx,dy,dt), //observations
						      disk, bar //workbuffer
						      );
	  eval +=  ret;
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
