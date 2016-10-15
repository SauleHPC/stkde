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
#include "point-based-symomp.hpp"

int main (int argc, char* argv[]) {

  if (argc < 5 ) {
    std::cerr<<"usage: "<<argv[0]<<" boundary observations param method"<<std::endl;
    return -1;	
  }
    
  
  std::string bbfile = argv[1];
  std::string obsfile = argv[2];
  std::string paramfile = argv[3];
  std::string method = argv[4];

  index decompX=-1, decompY=-1, decompT=-1;
  
  if (method.compare("POINTBASED-SYMOMP") == 0) {
    decompX = atoi(argv[5]);
    decompY = atoi(argv[6]);
    decompT = atoi(argv[7]);
  }
  
  bounding_box bb = load_bounding_box(bbfile);

  
  std::cerr.precision(10);
  
  std::cerr<<"bounding: "
	   <<"["<<bb.xl<<";"<<bb.xh<<"]"<<"x"
    	   <<"["<<bb.yl<<";"<<bb.yh<<"]"<<"x"
    	   <<"["<<bb.tl<<";"<<bb.th<<"]"<<std::endl;

  instance inst;

  load_observations (obsfile, inst.obsx, inst.obsy, inst.obst);

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
    std::cerr.precision(std::numeric_limits< coordinate > ::max_digits10);
    for (int i=0; i<std::min((size_t)5, (size_t)inst.obsx.size()); ++i) {
      std::cerr<<inst.obsx[i]<<" "
	       <<inst.obsy[i]<<" "
	       <<inst.obst[i]<<std::endl;
    }
  }

  parameters param = load_parameters(paramfile);

  std::cerr<<"res: "<<param.xres<<" "<<param.yres<<" "<<param.tres<<" "
	   <<"bw: "<<param.xbw<<" "<<param.ybw<<" "<<param.tbw<<std::endl;

  assert (param.xres == param.yres);
  assert (param.xbw == param.ybw);
  
  //
  util::timestamp beg;
  std::shared_ptr<util::Compact3D<values>> dens;

  if (method.compare("POINTBASED") == 0)
    dens = stkde_pointbased (bb, inst, param);
  if (method.compare("POINTBASED-SYMDISK") == 0)
    dens = stkde_pointbased_symdisk (bb, inst, param);
  if (method.compare("POINTBASED-SYMBAR") == 0)
    dens = stkde_pointbased_symbar (bb, inst, param);
  if (method.compare("POINTBASED-SYM") == 0)
    dens = stkde_pointbased_sym (bb, inst, param);
  if (method.compare("POINTBASED-SYMOMP") == 0)
    dens = stkde_pointbased_symomp (bb, inst, param, 8, 8, 8);
  util::timestamp end;

  if (0) {
    std::ofstream out ("log");
    //for debugging purpose
    for (int k=0; k< dens->getSizeZ(); ++k) {
	for (int j=0; j<std::min(dens->getSizeY(), (index)4000); ++j) {
	  for (int i=0; i<std::min(dens->getSizeX(), (index)4000); ++i) {
	  //      for (int k=0; k<std::min(dens->getSizeZ(), 10); ++k)
	  out<<(*dens)(i, j, k)<<" ";
	}
	out<<std::endl;
      }
      out<<std::endl;
    }
  }

  std::cerr.precision(2);
  std::cerr<<"time: "<<end-beg<<" seconds"<<std::endl;
  
  return 0;
}
