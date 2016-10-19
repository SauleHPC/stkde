#ifndef POINT_BASED_SYMOMP_OBSDECOMP_COLORSCHED_REP_HPP
#define POINT_BASED_SYMOMP_OBSDECOMP_COLORSCHED_REP_HPP

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

#include "graphanalysis.hpp"

std::shared_ptr<util::Compact3D<values>> stkde_pointbased_symomp_obsdecomp_colorsched_rep(const bounding_box& bb,
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
	//	if (load(dx,dy,dt) > ((double)inst.obsx.size())/omp_get_max_threads()/3)
	//  std::cerr<<dx<<","<<dy<<","<<dt<<" : "<<load(dx,dy,dt)<<std::endl;
      }
      // std::cerr<<std::endl;
     }
    //   std::cerr<<std::endl;
   }
  std::cerr<<"max load: "<<mostloaded<<" avgloadperbox: "<<((double)inst.obsx.size())/(decompsizeX*decompsizeY*decompsizeT)<<" avgloadpercore: "<<((double)inst.obsx.size())/omp_get_max_threads()<<" total load: "<<totalload<<std::endl;

  if (0) {
    std::ofstream out ("loadmap");
    for (int dt = 0; dt<decompsizeT; ++dt) {
      for (int dy = 0; dy<decompsizeY; ++dy) {
	for (int dx = 0; dx<decompsizeX; ++dx) {
	  out<<dx<<" "<<dy<<" "<<dt<<" "<<load(dx,dy,dt)<<std::endl;
	}
      }
    }
  }


  std::vector<triplet> orderedblocks;

  util::Compact3D<int> replication (decompsizeX, decompsizeY, decompsizeT);

  for (int dx = 0; dx<decompsizeX; ++dx) {
    for (int dy = 0; dy<decompsizeY; ++dy) {
      for (int dt = 0; dt<decompsizeT; ++dt) {
	replication(dx,dy,dt) = 1;
      }
    }
  }

  
  int nb_replication;
  {
    util::timestamp schedbeg;
    
    util::Compact3D<loadv> color (decompsizeX, decompsizeY, decompsizeT);
    util::Compact3D<std::vector<triplet> > graph_out (decompsizeX, decompsizeY, decompsizeT);

    loadbased_color(color, load);
    
    graph_out_from_color_stencil (color, graph_out);

    topo_sort(graph_out, orderedblocks);
    
    std::cout<<"load based longest path:"<<longest_path(graph_out, load)<<std::endl;

    loadv targetCP = inst.obsx.size()/(2*omp_get_max_threads());
    
    nb_replication = replicate (graph_out, load, replication, targetCP);
    
    util::timestamp schedend;

    std::cerr<<"nb replication: "<<nb_replication<<std::endl;
    std::cerr<<"schedule time: "<<schedend-schedbeg<<" seconds"<<std::endl; 
  }

  //array for storing replicated chunks
  std::shared_ptr<util::Compact3D<values>> *ptemp = new std::shared_ptr<util::Compact3D<values>> [nb_replication];
  
  
  for (int i=0; i<nb_replication; ++i)  {
    ptemp[i] = std::make_shared<util::Compact3D<values>>(c.voxX, c.voxY, c.voxT);

    (*(ptemp[i])).zero_parallel();
  }

  util::Compact3D<values>& co = *(ptemp[0]);



  
  util::timestamp computebeg;
  //compute
  index maxobs = 0;

  std::cerr<<"mem: "<<(decompsizeX+2)*(decompsizeY+2)*(decompsizeT+2)<<std::endl;
  int* dummy = new int[nb_replication*(decompsizeX+2)*(decompsizeY+2)*(decompsizeT+2)];

  util::Compact2D<values> **diskbuf = new util::Compact2D<values> *[omp_get_max_threads()];
  std::vector<values> **barbuf = new std::vector<values> *[omp_get_max_threads()];
  for (int i=0; i<omp_get_max_threads(); ++i) {

    barbuf[i] = new std::vector<values> (c.voxT);
    diskbuf[i] = new util::Compact2D<values> (c.voxX, c.voxY); //naive version of symmetry uses a disk buffer of the map size
  }
  

#pragma omp parallel reduction(+:eval)
  {

  //
      
    
#pragma omp single
    {
      for (auto t : orderedblocks) {
	index dx = t.x;
	index dy = t.y;
	index dt = t.z;
	
	if (decompX(dx,dy,dt).size() > 0) { //not generating task if there is no work to decrease scheduler pressure
	  for (int rep = 0; rep < replication(dx,dy,dt); ++rep) {
#pragma omp task depend(inout:dummy[rep*(decompsizeX+2)*(decompsizeY+2)*(decompsizeT+2) + (dx+1)*(decompsizeY+2)*(decompsizeT+2)+(dy+1)*(decompsizeT+2)+(dt+1)]) \
                    depend(in:dummy[rep*(decompsizeX+2)*(decompsizeY+2)*(decompsizeT+2) + (dx)*(decompsizeY+2)*(decompsizeT+2)+(dy)*(decompsizeT+2)+(dt)], \
                              dummy[rep*(decompsizeX+2)*(decompsizeY+2)*(decompsizeT+2) + (dx)*(decompsizeY+2)*(decompsizeT+2)+(dy)*(decompsizeT+2)+(dt+1)], \
                              dummy[rep*(decompsizeX+2)*(decompsizeY+2)*(decompsizeT+2) + (dx)*(decompsizeY+2)*(decompsizeT+2)+(dy)*(decompsizeT+2)+(dt+2)], \
                              dummy[rep*(decompsizeX+2)*(decompsizeY+2)*(decompsizeT+2) + (dx)*(decompsizeY+2)*(decompsizeT+2)+(dy+1)*(decompsizeT+2)+(dt)], \
			      dummy[rep*(decompsizeX+2)*(decompsizeY+2)*(decompsizeT+2) + (dx)*(decompsizeY+2)*(decompsizeT+2)+(dy+1)*(decompsizeT+2)+(dt+1)], \
                              dummy[rep*(decompsizeX+2)*(decompsizeY+2)*(decompsizeT+2) + (dx)*(decompsizeY+2)*(decompsizeT+2)+(dy+1)*(decompsizeT+2)+(dt+2)], \
			      dummy[rep*(decompsizeX+2)*(decompsizeY+2)*(decompsizeT+2) + (dx)*(decompsizeY+2)*(decompsizeT+2)+(dy+2)*(decompsizeT+2)+(dt)], \
			      dummy[rep*(decompsizeX+2)*(decompsizeY+2)*(decompsizeT+2) + (dx)*(decompsizeY+2)*(decompsizeT+2)+(dy+2)*(decompsizeT+2)+(dt+1)], \
                              dummy[rep*(decompsizeX+2)*(decompsizeY+2)*(decompsizeT+2) + (dx)*(decompsizeY+2)*(decompsizeT+2)+(dy+2)*(decompsizeT+2)+(dt+2)], \
			      dummy[rep*(decompsizeX+2)*(decompsizeY+2)*(decompsizeT+2) + (dx+1)*(decompsizeY+2)*(decompsizeT+2)+(dy)*(decompsizeT+2)+(dt)], \
			      dummy[rep*(decompsizeX+2)*(decompsizeY+2)*(decompsizeT+2) + (dx+1)*(decompsizeY+2)*(decompsizeT+2)+(dy)*(decompsizeT+2)+(dt+1)], \
                              dummy[rep*(decompsizeX+2)*(decompsizeY+2)*(decompsizeT+2) + (dx+1)*(decompsizeY+2)*(decompsizeT+2)+(dy)*(decompsizeT+2)+(dt+2)], \
                              dummy[rep*(decompsizeX+2)*(decompsizeY+2)*(decompsizeT+2) + (dx+1)*(decompsizeY+2)*(decompsizeT+2)+(dy+1)*(decompsizeT+2)+(dt)], \
			      \
                              dummy[rep*(decompsizeX+2)*(decompsizeY+2)*(decompsizeT+2) + (dx+1)*(decompsizeY+2)*(decompsizeT+2)+(dy+1)*(decompsizeT+2)+(dt+2)], \
			      dummy[rep*(decompsizeX+2)*(decompsizeY+2)*(decompsizeT+2) + (dx+1)*(decompsizeY+2)*(decompsizeT+2)+(dy+2)*(decompsizeT+2)+(dt)], \
			      dummy[rep*(decompsizeX+2)*(decompsizeY+2)*(decompsizeT+2) + (dx+1)*(decompsizeY+2)*(decompsizeT+2)+(dy+2)*(decompsizeT+2)+(dt+1)], \
                              dummy[rep*(decompsizeX+2)*(decompsizeY+2)*(decompsizeT+2) + (dx+1)*(decompsizeY+2)*(decompsizeT+2)+(dy+2)*(decompsizeT+2)+(dt+2)], \
			      dummy[rep*(decompsizeX+2)*(decompsizeY+2)*(decompsizeT+2) + (dx+2)*(decompsizeY+2)*(decompsizeT+2)+(dy)*(decompsizeT+2)+(dt)], \
			      dummy[rep*(decompsizeX+2)*(decompsizeY+2)*(decompsizeT+2) + (dx+2)*(decompsizeY+2)*(decompsizeT+2)+(dy)*(decompsizeT+2)+(dt+1)], \
                              dummy[rep*(decompsizeX+2)*(decompsizeY+2)*(decompsizeT+2) + (dx+2)*(decompsizeY+2)*(decompsizeT+2)+(dy)*(decompsizeT+2)+(dt+2)], \
                              dummy[rep*(decompsizeX+2)*(decompsizeY+2)*(decompsizeT+2) + (dx+2)*(decompsizeY+2)*(decompsizeT+2)+(dy+1)*(decompsizeT+2)+(dt)], \
			      dummy[rep*(decompsizeX+2)*(decompsizeY+2)*(decompsizeT+2) + (dx+2)*(decompsizeY+2)*(decompsizeT+2)+(dy+1)*(decompsizeT+2)+(dt+1)], \
                              dummy[rep*(decompsizeX+2)*(decompsizeY+2)*(decompsizeT+2) + (dx+2)*(decompsizeY+2)*(decompsizeT+2)+(dy+1)*(decompsizeT+2)+(dt+2)], \
			      dummy[rep*(decompsizeX+2)*(decompsizeY+2)*(decompsizeT+2) + (dx+2)*(decompsizeY+2)*(decompsizeT+2)+(dy+2)*(decompsizeT+2)+(dt)], \
			      dummy[rep*(decompsizeX+2)*(decompsizeY+2)*(decompsizeT+2) + (dx+2)*(decompsizeY+2)*(decompsizeT+2)+(dy+2)*(decompsizeT+2)+(dt+1)], \
                              dummy[rep*(decompsizeX+2)*(decompsizeY+2)*(decompsizeT+2) + (dx+2)*(decompsizeY+2)*(decompsizeT+2)+(dy+2)*(decompsizeT+2)+(dt+2)] \
			)
	    {
	      index beg = rep*(decompX(dx,dy,dt).size()/replication(dx,dy,dt));
	      index end = (rep+1)*(decompX(dx,dy,dt).size()/replication(dx,dy,dt));
	      if (rep == replication(dx,dy,dt) -1 )
		end = decompX(dx,dy,dt).size();
	      
	      int tid = omp_get_thread_num();

	      computation clocal = c;
	      c.p = ptemp[rep];
	      
	      //		    std::cerr<<dx<<"x"<<dy<<"x"<<dt<<std::endl;
	      index ret =  process_observation_boxed_sym (c, //comp// DO SOMETHING HERE!
							  0, c.voxX, 0, c.voxY, 0, c.voxT, //box
							  decompX(dx,dy,dt), decompY(dx,dy,dt), decompT(dx,dy,dt), //observations
							  *(diskbuf[tid]), *(barbuf[tid]), //workbuffer
							  beg, end);
	    }
	  }
	}
	//  eval +=  ret;
      }
    }
  }

  util::timestamp computeend;

  std::cerr<<"compute: "<<computeend-computebeg<<" seconds"<<std::endl;

  //reduction
  
  util::timestamp reducebeg;
  
  util::Compact3D<values>& out = *(ptemp[0]);
  
  //reduce intermediate compacts
#pragma omp for schedule(dynamic,2048)
  for (index k=0; k< co.getSizeX()*co.getSizeY()*co.getSizeZ(); ++k) {
    double value = 0.;
    for (int t = 1; t < nb_replication; ++t) {
      value += (*(ptemp[t]))(k);
    }
    (*(ptemp[0]))(k) += value;
  }
  util::timestamp reduceend;

  std::cerr<<"reduction: "<<reduceend - reducebeg<<" seconds"<<std::endl;
  
  // eval +=  process_observation_boxed_sym (c, //comp
  // 					  0, c.voxX, 0, c.voxY, 0, c.voxT, //box
  // 					  inst.obsx, inst.obsy, inst.obst, //observations
  // 					  disk, bar //workbuffer
  // 					  );

  
  std::cerr<<"evaluations: "<<eval<<std::endl;
  
  return ((ptemp[0]));
}


#endif
