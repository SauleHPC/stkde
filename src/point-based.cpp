#include <unistd.h>
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
#include "point-based.hpp"
#include "point-based-symdisk.hpp"
#include "point-based-symbar.hpp"
#include "point-based-sym.hpp"

#include "partition.hpp"

#ifndef NO_OMP
#include "point-based-symomp.hpp"
#include "point-based-symomp-boxdecomposition.hpp"
#include "point-based-symomp-pointdecomp.hpp"
#include "point-based-symomp-obsdecomp.hpp"
#include "point-based-symomp-obsdecomp-sched.hpp"
#include "point-based-symomp-obsdecomp-colorsched.hpp"
#include "point-based-symomp-obsdecomp-colorsched-rep.hpp"

#endif

#include "voxel-based.hpp"
#include "voxel-based-gpu.hpp"
#ifndef NO_OMP
#include "voxel-based-omp.hpp"
#endif
#include "voxel-based-obsdecomp.hpp"
#ifndef NO_OMP
#include "voxel-based-omp-obsdecomp.hpp"
#endif

namespace stkde {

void density_compare(std::shared_ptr<util::Compact3D<values>> dens1, 
		     std::shared_ptr<util::Compact3D<values>> dens2,
		     values* dist_sum,
		     values* dist_max,
		     values* total_density) {
  
  assert (dens1->getSizeX() == dens2->getSizeX());
  assert (dens1->getSizeY() == dens2->getSizeY());
  assert (dens1->getSizeZ() == dens2->getSizeZ());

  *dist_sum = 0.;
  *dist_max = 0.;
  *total_density = 0.;

  for (long x = 0; x < dens1->getSizeX(); ++x)
    for (long y = 0; y < dens1->getSizeY(); ++y)
      for (long z = 0; z < dens1->getSizeZ(); ++z) {
	*total_density += (*dens1)(x,y,z);
	values d = (*dens1)(x,y,z) - (*dens2)(x,y,z);
	d = std::abs(d);

	if (d > *dist_max)
	  *dist_max = d;
	
	*dist_sum += d;
      }
}
}


int main (int argc, char* argv[]) {

  if (argc < 5 ) {
    std::cerr<<"usage: "<<argv[0]<<" boundary observations param method [decompX] [decompY] [decompZ] [...]"<<std::endl;
    std::cerr<<"Methods: POINTBASED,"<<std::endl
	     <<"         POINTBASED-SYMDISK, POINTBASED-SYMBAR, POINTBASED-SYM"<<std::endl
	     <<"         POINTBASED-SYMOMP, POINTBASED-SYMOMP-OBSDECOMP, POINTBASED-SYMOMP-OBSDECOMP-SCHED"<<std::endl
	     <<"         POINTBASED-SYMOMP-OBSDECOMP-COLORSCHED, POINTBASED-SYMOMP-OBSDECOMP-COLORSCHED-REP"<<std::endl
	     <<"         POINTBASED-SYMOMP-POINTDECOMP"<<std::endl<<std::endl
	     <<"         VOXELBASED, VOXELBASED-OMP"<<std::endl
	     <<"         VOXELBASED-OBSDECOMP, VOXELBASED-OMP-OBSDECOMP"<<std::endl;
     
    return -1;	
  }
    
  
  std::string bbfile = argv[1];
  std::string obsfile = argv[2];
  std::string paramfile = argv[3];
  std::string method = argv[4];

  stkde::index decompX=-1, decompY=-1, decompT=-1;
  
  if (method.compare("POINTBASED-SYMOMP") == 0
      || method.compare("POINTBASED-SYMOMP-OBSDECOMP") == 0
      || method.compare("POINTBASED-SYMOMP-OBSDECOMP-COLORSCHED") == 0
      || method.compare("POINTBASED-SYMOMP-OBSDECOMP-COLORSCHED-REP") == 0
      || method.compare("VOXELBASED-OBSDECOMP") == 0
      || method.compare("VOXELBASED-OMP-OBSDECOMP") == 0
      || method.compare("POINTBASED-SYMOMP-OBSDECOMP-SCHED") == 0
      || method.compare("POINTBASED-SYMOMP-BOXDECOMP") == 0 ) {
    decompX = atoi(argv[5]);
    decompY = atoi(argv[6]);
    decompT = atoi(argv[7]);
  }

  bool compare = false;

  if (argc > 8) {
    //extra args to process
    for (int i = 8; i< argc; ++i) {
      std::string opt = argv[i] ;
      if (opt.compare("compare") == 0) {
	compare = true;
      }
    }
    
  }
  
  
  stkde::bounding_box bb = stkde::load_bounding_box(bbfile);

  
  std::cerr.precision(10);

  char host[1024];
  gethostname (host, 1024);
  host [1023] = '0';
  
  std::cerr<<"hostname : "<<host<<std::endl;
  
  std::cerr<<"bounding: "
	   <<"["<<bb.xl<<";"<<bb.xh<<"]"<<"x"
    	   <<"["<<bb.yl<<";"<<bb.yh<<"]"<<"x"
    	   <<"["<<bb.tl<<";"<<bb.th<<"]"<<std::endl;

  stkde::instance inst;

  stkde::load_observations (obsfile, inst.obsx, inst.obsy, inst.obst);

  if (0) {
    //randomize the order
    for (int i=inst.obsx.size()-1; i>1; --i) {
      int dest = rand()%(i+1);
      assert (dest>=0);
      if (dest != i) {
	std::swap (inst.obsx[i], inst.obsx[dest]);
	std::swap (inst.obsy[i], inst.obsy[dest]);
	std::swap (inst.obst[i], inst.obst[dest]);
      }
    }
  }
  
  if (0) {
    //show first 5 observation
    std::cerr<<"first 5 observations"<<std::endl;
    std::cerr.precision(std::numeric_limits< stkde::coordinate > ::max_digits10);
    for (int i=0; i<std::min((size_t)5, (size_t)inst.obsx.size()); ++i) {
      std::cerr<<inst.obsx[i]<<" "
	       <<inst.obsy[i]<<" "
	       <<inst.obst[i]<<std::endl;
    }
  }

  stkde::parameters param = stkde::load_parameters(paramfile);

  std::cerr<<"res: "<<param.xres<<" "<<param.yres<<" "<<param.tres<<" "
	   <<"bw: "<<param.xbw<<" "<<param.ybw<<" "<<param.tbw<<std::endl;

  assert (param.xres == param.yres);
  assert (param.xbw == param.ybw);
  
  //
  util::timestamp beg;
  std::shared_ptr<util::Compact3D<stkde::values>> dens;

  if (method.compare("POINTBASED") == 0)
    dens = stkde::stkde_pointbased (bb, inst, param);
  if (method.compare("POINTBASED-SYMDISK") == 0)
    dens = stkde::stkde_pointbased_symdisk (bb, inst, param);
  if (method.compare("POINTBASED-SYMBAR") == 0)
    dens = stkde::stkde_pointbased_symbar (bb, inst, param);
  if (method.compare("POINTBASED-SYM") == 0)
    dens = stkde::stkde_pointbased_sym (bb, inst, param);

  if (method.compare("PARTITION-HIER") == 0)
    stkde::partition_hier (bb, inst, param);
  
#ifndef NO_OMP
  if (method.compare("POINTBASED-SYMOMP") == 0)
    dens = stkde::stkde_pointbased_symomp (bb, inst, param, decompX, decompY, decompT);
  if (method.compare("POINTBASED-SYMOMP-OBSDECOMP") == 0)
    dens = stkde::stkde_pointbased_symomp_obsdecomp (bb, inst, param, decompX, decompY, decompT);
  if (method.compare("POINTBASED-SYMOMP-OBSDECOMP-SCHED") == 0)
    dens = stkde::stkde_pointbased_symomp_obsdecomp_sched (bb, inst, param, decompX, decompY, decompT);
  if (method.compare("POINTBASED-SYMOMP-OBSDECOMP-COLORSCHED") == 0)
    dens = stkde::stkde_pointbased_symomp_obsdecomp_colorsched (bb, inst, param, decompX, decompY, decompT);
  if (method.compare("POINTBASED-SYMOMP-OBSDECOMP-COLORSCHED-REP") == 0)
    dens = stkde::stkde_pointbased_symomp_obsdecomp_colorsched_rep (bb, inst, param, decompX, decompY, decompT);
  
  if (method.compare("POINTBASED-SYMOMP-POINTDECOMP") == 0)
    dens = stkde::stkde_pointbased_symomp_point (bb, inst, param);

  if (method.compare("POINTBASED-SYMOMP-BOXDECOMP") == 0) {
    std::vector<stkde::voxelbox> decomp;

    stkde::computation c (bb,inst, param, false);

    int decompsizeX=decompX, decompsizeY=decompY, decompsizeT=decompT;
    for (int dx = 0; dx<decompsizeX; ++dx) {
      for (int dy = 0; dy<decompsizeY; ++dy) {
	for (int dt = 0; dt<decompsizeT; ++dt) {

	  stkde::coordinate decxmin = bb.xl + ( dx   *(bb.xh-bb.xl)/decompsizeX );
	  stkde::index voxXmin = std::lround(std::ceil((decxmin-bb.xl)/param.xres));
	  stkde::coordinate decxmax = bb.xl + ((dx+1)*(bb.xh-bb.xl)/decompsizeX );
	  stkde::index voxXmax;
	  if (dx != decompsizeX-1)
	    voxXmax = std::lround(std::floor((decxmax-bb.xl)/param.xres))+1;
	  else
	    voxXmax = c.voxX;
	  
	  stkde::coordinate decymin = bb.yl + ( dy   *(bb.yh-bb.yl)/decompsizeY );
	  stkde::index voxYmin = std::lround(std::ceil((decymin-bb.yl)/param.yres));
	  stkde::coordinate decymax = bb.yl + ((dy+1)*(bb.yh-bb.yl)/decompsizeY );
	  stkde::index voxYmax;
	  if (dy != decompsizeY-1)
	    voxYmax = std::lround(std::floor((decymax-bb.yl)/param.yres))+1;
	  else
	    voxYmax = c.voxY;
	  
	  
	  stkde::coordinate dectmin = bb.tl + ( dt   *(bb.th-bb.tl)/decompsizeT );
	  stkde::index voxTmin = std::lround(std::ceil((dectmin-bb.tl)/param.tres));
	  stkde::coordinate dectmax = bb.tl + ((dt+1)*(bb.th-bb.tl)/decompsizeT );
	  stkde::index voxTmax;
	  if (dt != decompsizeT-1)
	    voxTmax = std::lround(std::floor((dectmax-bb.tl)/param.tres))+1;
	  else
	    voxTmax = c.voxT;


	  stkde::voxelbox vb(voxXmin, voxXmax,voxYmin, voxYmax,voxTmin, voxTmax);
	  decomp.push_back(vb);
	}
      }
    }
    
    dens = stkde::stkde_pointbased_symomp_boxdecomp (bb, inst, param, decomp);
  }
#endif
  
  if (method.compare("VOXELBASED") == 0)
    dens = stkde::stkde_voxelbased(bb, inst, param);

  if (method.compare("VOXELBASED-GPU") == 0)
    dens = stkde::stkde_voxelbased_gpu(bb, inst, param);
	
#ifndef NO_OMP
  if (method.compare("VOXELBASED-OMP") == 0)
    dens = stkde::stkde_voxelbased_omp(bb, inst, param);
#endif
  if (method.compare("VOXELBASED-OBSDECOMP") == 0) 
    dens = stkde::stkde_voxelbased_obsdecomp(bb, inst, param, decompX, decompY, decompT);

#ifndef NO_OMP
  if (method.compare("VOXELBASED-OMP-OBSDECOMP") == 0)
    dens = stkde::stkde_voxelbased_omp_obsdecomp(bb, inst, param, decompX, decompY, decompT);
#endif
  
  util::timestamp end;

  std::cerr.precision(2);
  std::cerr<<"time: "<<end-beg<<" seconds"<<std::endl;
  
  
  if (compare) {
    std::cerr<<"========Comparing to POINT-BASED========="<<std::endl;
    std::shared_ptr<util::Compact3D<stkde::values>> dens2 = stkde_pointbased (bb, inst, param);
    
    stkde::values dsum=0., dmax=0., total_dens=0.;

    stkde::density_compare(dens, dens2, &dsum, &dmax, &total_dens);

    std::cerr<<"Distance to POINTBASED="<<dsum<<" max="<<dmax<<" totaldensity="<<total_dens<<std::endl;
  }

  if (0) {
    std::ofstream out ("log");
    //for debugging purpose
    for (int k=0; k< dens->getSizeZ(); ++k) {
	for (int j=0; j<std::min(dens->getSizeY(), (stkde::index)4000); ++j) {
	  for (int i=0; i<std::min(dens->getSizeX(), (stkde::index)4000); ++i) {
	  //      for (int k=0; k<std::min(dens->getSizeZ(), 10); ++k)
	  out<<(*dens)(i, j, k)<<" ";
	}
	out<<std::endl;
      }
      out<<std::endl;
    }
  }

  
  return 0;
}
