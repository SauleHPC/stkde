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
#include "partition-over.hpp"
#include "partition-jagged.hpp"
#include "partition-jagged-over.hpp"


int main (int argc, char* argv[]) {

  if (argc < 6 ) {
    std::cerr<<"usage: "<<argv[0]<<" boundary observations param outfile  method [methodparams]"<<std::endl;
    std::cerr<<"Methods: PARTITION-HIER nbparts xstep ystep tstep"<<std::endl;
    std::cerr<<"Methods: PARTITION-JAGGED nbparts xstep ystep tstep"<<std::endl;
    std::cerr<<"Methods: PARTITION-HIER-OVERDECOMPOSE nbparts xstep ystep tstep loadratio"<<std::endl;
    std::cerr<<"Methods: PARTITION-JAGGED-OVERDECOMPOSE nbparts xstep ystep tstep loadratio"<<std::endl;
     
    return -1;	
  }
    
  
  std::string bbfile = argv[1];
  std::string obsfile = argv[2];
  std::string paramfile = argv[3];
  std::string outfile = argv[4]; //file to write the partition to
  std::string method = argv[5];
  

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

  std::vector<stkde::voxelbox> parts;


  
  util::timestamp begin;

  //
  if (method.compare("PARTITION-HIER") == 0) {

    int nbparts = atoi(argv[6]);
    stkde::index xstep = atoi(argv[7]);
    stkde::index ystep = atoi(argv[8]);
    stkde::index tstep = atoi(argv[9]);
    
    parts = stkde::partition_hier (bb, inst, param, nbparts, xstep, ystep, tstep);
  }

  if (method.compare("PARTITION-HIER-OVERDECOMPOSE") == 0) {
    int nbparts = atoi(argv[6]);
    stkde::index xstep = atoi(argv[7]);
    stkde::index ystep = atoi(argv[8]);
    stkde::index tstep = atoi(argv[9]);
    double loadratio=atof(argv[10]); // max imbalance ratio allowed
    
    parts = stkde::partition_hier_overdecompose (bb, inst, param, nbparts, xstep, ystep, tstep, loadratio);
  }

  if (method.compare("PARTITION-JAGGED") == 0) {
    int nbparts = atoi(argv[6]);
    stkde::index xstep = atoi(argv[7]);
    stkde::index ystep = atoi(argv[8]);
    stkde::index tstep = atoi(argv[9]);
    
    parts = stkde::partition_jagged (bb, inst, param, nbparts, xstep, ystep, tstep);
  }

  
  if (method.compare("PARTITION-JAGGED-OVERDECOMPOSE") == 0) {
    int nbparts = atoi(argv[6]);
    stkde::index xstep = atoi(argv[7]);
    stkde::index ystep = atoi(argv[8]);
    stkde::index tstep = atoi(argv[9]);
    double loadratio=atof(argv[10]); // max imbalance ratio allowed
    
    parts = stkde::partition_jagged_overdecompose (bb, inst, param, nbparts, xstep, ystep, tstep, loadratio);
  }

  

  util::timestamp end;

  std::cerr.precision(2);
  std::cerr<<"partitioning time: "<<end-begin<<" seconds"<<std::endl;

  
  {
    std::ofstream out (outfile);
    out<<parts.size()<<std::endl;
    for (auto vb : parts)
      out<<vb<<std::endl;				   
  }
    
  
  return 0;
}
