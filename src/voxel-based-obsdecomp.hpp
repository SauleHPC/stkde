
#ifndef VOXEL_BASED_OBSDECOMP_HPP
#define VOXEL_BASED_OBSDECOMP_HPP

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
#include "decompositions.hpp"
#include "computation.hpp"

std::shared_ptr<util::Compact3D<values>> stkde_voxelbased_obsdecomp(const bounding_box& bb,
									   const instance& inst,
									   const parameters& pa,
									   indexi decompsizeX,
									   indexi decompsizeY,
									   indexi decompsizeT
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
  
  util::Compact3D<indexi> load (decompsizeX, decompsizeY, decompsizeT);

  
  util::Compact3D<std::vector<coordinate>> decompX (decompsizeX, decompsizeY, decompsizeT);
  util::Compact3D<std::vector<coordinate>> decompY (decompsizeX, decompsizeY, decompsizeT);
  util::Compact3D<std::vector<coordinate>> decompT (decompsizeX, decompsizeY, decompsizeT);

  decomposition(bb, inst, pa, decompsizeX, decompsizeY, decompsizeT,
		load,
		decompX, decompY, decompT
		);

  
  util::timestamp decend;
  std::cerr<<"decomposition time: "<<decend-decbeg<<" seconds"<<std::endl;
  
  
  util::timestamp compbeg;

  for (indexi vox=0; vox<c.voxX; ++vox) {
    for (indexi voy=0; voy<c.voxY; ++voy) {
      for (indexi vot=0; vot<c.voxT; ++vot) {
	//process voxel vox, voy, vot at coordinate vox_x, vox_y, vox_t
	coordinate vox_x = bb.xl + vox*pa.xres;
	coordinate vox_y = bb.yl + voy*pa.yres;
	coordinate vox_t = bb.tl + vot*pa.tres;

	//voxel is in decomposition decX, decY, decT
	indexi decX = (vox_x-bb.xl)/(bb.xh-bb.xl)*decompsizeX;
	indexi decY = (vox_y-bb.yl)/(bb.yh-bb.yl)*decompsizeY;
	indexi decT = (vox_t-bb.tl)/(bb.th-bb.tl)*decompsizeT;
	
	values v = 0;

	//enumerate all nearby decomposition
	for (indexi dx = std::max(decX-1, (indexi)0); dx <= std::min(decX+1, decompsizeX-1); dx++)
	  for (indexi dy = std::max(decY-1, (indexi)0); dy <= std::min(decY+1, decompsizeY-1); dy++)
	    for (indexi dt = std::max(decT-1, (indexi)0); dt <= std::min(decT+1, decompsizeT-1); dt++) {

	      //enumerate all observation in that decomposition
	      for(int o = 0; o < decompX(dx, dy, dt).size(); o++) {
		//get observation at ox, oy, ot
		coordinate ox = decompX(dx,dy,dt)[o];
		coordinate oy = decompY(dx,dy,dt)[o];
		coordinate ot = decompT(dx,dy,dt)[o];
		
		if(std::abs(vox_t - ot) <= pa.tbw) {
		  if(std::sqrt((vox_x - ox) * (vox_x - ox) + (vox_y - oy) * (vox_y - oy)) <= pa.xbw) {
		    v += densityF(vox_x, vox_y, vox_t,
				  ox, oy, ot,
				  inst.obsx.size(), pa.xbw, pa.tbw);
		    eval++;
		  }
		}
	      }
	      
	    }
	
	
	co(vox, voy, vot) = v;
      }
    }
  }
  
  util::timestamp compend;

    
  
  std::cerr<<"compute: "<<(compend-compbeg)<<" seconds"<<std::endl;
  std::cerr<<"evaluations: "<<eval<<std::endl;
  return c.p;
}


#endif
