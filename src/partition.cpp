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



#include "partition.hpp"


int main (int argc, char* argv[]) {

  if (argc < 5 ) {
    std::cerr<<"usage: "<<argv[0]<<" boundary observations param method [decompX] [decompY] [decompZ] [...]"<<std::endl;
    std::cerr<<"Methods: PARTITION-HIER"<<std::endl;
     
    return -1;	
  }
    
  
  std::string bbfile = argv[1];
  std::string obsfile = argv[2];
  std::string paramfile = argv[3];
  std::string method = argv[4];

  stkde::index decompX=-1, decompY=-1, decompT=-1;

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

  stkde::parameters param = stkde::load_parameters(paramfile);

  std::cerr<<"res: "<<param.xres<<" "<<param.yres<<" "<<param.tres<<" "
	   <<"bw: "<<param.xbw<<" "<<param.ybw<<" "<<param.tbw<<std::endl;

  assert (param.xres == param.yres);
  assert (param.xbw == param.ybw);
  
  //
  if (method.compare("PARTITION-HIER") == 0)
    stkde::partition_hier (bb, inst, param, 16);


    
  
  return 0;
}
