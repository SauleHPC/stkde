#ifndef POINT_BASED_SYMOMP_BOXDECOMP_HPP
#define POINT_BASED_SYMOMP_BOXDECOMP_HPP

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
#include "decompositions.hpp"
#include "computation.hpp"

namespace stkde {


  
  ///decompose the space in subboxes passed as a parameter. subboxes are processed in parallel.
  ///an observation that impacts two subboxes is (partially) computed twice.
  std::shared_ptr<util::Compact3D<values>> stkde_pointbased_symomp_boxdecomp(const bounding_box& bb,
									     const instance& inst,
									     const parameters& pa,
									     const std::vector<voxelbox>& decomp
									     ) {
    computation c (bb,inst, pa);
  
    std::cerr.precision(9);
    
    std::cerr<<"voxsize: "<<c.voxX<<"x"<<c.voxY<<"x"<<c.voxT<<" size:"<<c.voxX*c.voxY*c.voxT*sizeof(values)/1024./1024.<<"MB"<<std::endl;
    std::cerr<<"voxBW: "<<c.voxsbw<<" "<<c.voxtbw<<std::endl;

    std::cerr<<"Decomp size:"<<decomp.size()<<std::endl;
    
    util::Compact3D<values>& co = *(c.p);

    init_stkde(c);
  
    long int eval = 0;

    // util::timestamp decbeg;
    // //decomposition
  
    // util::Compact3D<index> load (decompsizeX, decompsizeY, decompsizeT);

    // util::Compact3D<std::vector<coordinate>> decompX (decompsizeX, decompsizeY, decompsizeT);
    // util::Compact3D<std::vector<coordinate>> decompY (decompsizeX, decompsizeY, decompsizeT);
    // util::Compact3D<std::vector<coordinate>> decompT (decompsizeX, decompsizeY, decompsizeT);


    // decomposition_ghost(bb, inst, pa,
    // 			decompsizeX, decompsizeY, decompsizeT,
    // 			load, decompX, decompY, decompT);

    // util::timestamp decend;

    // std::cerr<<"decomposition time: "<<decend-decbeg<<" seconds"<<std::endl;
    // index mostloaded = 0;
    // for (int dt = 0; dt<decompsizeT; ++dt) {
    //   for (int dy = 0; dy<decompsizeY; ++dy) {
    // 	for (int dx = 0; dx<decompsizeX; ++dx) {
    // 	  mostloaded = std::max(mostloaded, load(dx,dy,dt));
    // 	  //std::cerr<<load(dx,dy,dt)<<" ";
    // 	}
    // 	// std::cerr<<std::endl;
    //   }
    //   //   std::cerr<<std::endl;
    // }
    // std::cerr<<"max load: "<<mostloaded<<" avgloadperbox: "<<((double)inst.obsx.size())/(decompsizeX*decompsizeY*decompsizeT)<<" avgloadpercore: "<<((double)inst.obsx.size())/omp_get_max_threads()<<std::endl;


    util::timestamp computebeg;
    //compute
#pragma omp parallel reduction(+:eval)
    {
    
      util::Compact2D<values> disk (c.voxX, c.voxY); //naive version of symmetry uses a disk buffer of the map size

      std::vector<values> bar(c.voxT);

      //  
#pragma omp for schedule(dynamic,1)
      for (int i=0; i<decomp.size(); ++i) {
	auto& b = decomp[i];
	index ret =  process_observation_boxed_sym (c, //comp
						    b.voxXmin, b.voxXmax, b.voxYmin, b.voxYmax, b.voxTmin, b.voxTmax, //box
						    inst.obsx, inst.obsy, inst.obst, //observations
						    disk, bar //workbuffer
						    );
	eval +=  ret;
      }
    }
  
    util::timestamp computeend;

    std::cerr<<"compute: "<<computeend-computebeg<<" seconds"<<std::endl;
  
  
    std::cerr<<"evaluations: "<<eval<<std::endl;
  
    return c.p;
  }

}

#endif
