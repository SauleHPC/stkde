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

values densityF( coordinate obsX, coordinate obsY, coordinate obsT,
		 coordinate voxX, coordinate voxY, coordinate voxT,
		 int nbObs, coordinate xbw, coordinate tbw
		 ) {

  values u = (obsX-voxX) / xbw;
  values v = (obsY-voxY) / xbw;
  values w = (obsT-voxT) / tbw;
    
  values constantTerm = std::pow(10.0, 10) / (nbObs * pow(xbw, 2) * tbw);
  values Ks = (0.5 * M_PI) * (1 - pow(u, 2) - pow(v, 2));
  values Kt = 0.75 * (1 - pow(w, 2));
  
  values spaceTimeKDE = constantTerm * Ks * Kt;
    
  return spaceTimeKDE;
}

std::shared_ptr<util::Compact3D<values>> stkde(const bounding_box& bb,
					       const instance& inst,
					       const parameters& pa) {
  int voxX = std::lround(std::ceil((bb.xh-bb.xl)/pa.xres))+1;
  int voxY = std::lround(std::ceil((bb.yh-bb.yl)/pa.yres))+1;
  int voxT = std::lround(std::ceil((bb.th-bb.tl)/pa.tres))+1;

  int voxsbw = std::lround(std::ceil(pa.xbw/pa.xres));
  int voxtbw = std::lround(std::ceil(pa.tbw/pa.tres));

  std::cerr.precision(4);
    
  std::cerr<<"voxsize: "<<voxX<<"x"<<voxY<<"x"<<voxT<<" size:"<<voxX*voxY*voxT*sizeof(values)/1024./1024.<<"MB"<<std::endl;
  std::cerr<<"voxBW: "<<voxsbw<<" "<<voxtbw<<std::endl;
  
  std::shared_ptr<util::Compact3D<values>> p = std::make_shared<util::Compact3D<values>>(voxX, voxY, voxT);

  util::Compact3D<values>& co = *p;

  //fill in zeroes
  for (int i=0; i< co.getSizeX(); ++i)
    for (int j=0; j< co.getSizeY(); ++j)
      for (int k=0; k< co.getSizeZ(); ++k)
	co(i,j,k) = 0;

  int eval = 0;
    
  //account for observations
  for (int ob=0; ob<inst.obsx.size(); ++ob) {
    //observation
    coordinate ox = inst.obsx[ob];
    coordinate oy = inst.obsy[ob];
    coordinate ot = inst.obst[ob];

    //voxel containing the observation
    int obsvx = (ox - bb.xl)/pa.xres;
    int obsvy = (oy - bb.yl)/pa.yres;
    int obsvt = (ot - bb.tl)/pa.tres;

    //std::cerr<<"obsv: "<<obsvx<<" "<<obsvy<<" "<<obsvt<<std::endl;

    //BW around observation
    for (int i = std::max(obsvx - voxsbw, 0); i< std::min(obsvx + voxsbw, voxX); ++i) {
      for (int j = std::max(obsvy - voxsbw, 0); j< std::min(obsvy + voxsbw, voxY); ++j) {
	for (int k = std::max(obsvt - voxtbw, 0); k< std::min(obsvt + voxtbw, voxT); ++k) {
	  coordinate vox_x = bb.xl + i*pa.xres;
	  coordinate vox_y = bb.yl + j*pa.yres;
	  coordinate vox_t = bb.tl + k*pa.tres;

	  //std::cerr<<i<<" "<<j<<" "<<k<<std::endl;	   
	  if (std::abs(vox_t - ot) <= pa.tbw
	      && std::sqrt((ox - vox_x)*(ox - vox_x) + (oy - vox_y)*(oy - vox_y)) <= pa.xbw ) {
	    
	    values val = densityF(ox, oy, ot,
				  vox_x, vox_y, vox_t,
				  inst.obsx.size(), pa.xbw, pa.tbw);
	    
	    //std::cerr<<vox_x<<" "<<vox_y<<" "<<vox_t<<" "<<val<<std::endl;
	    
	    co(i,j,k) += val;
	    eval++;
	  }
	}
      }
    }
  }

  std::cerr<<"evaluations: "<<eval<<std::endl;
  
  return p;
}

int main (int argc, char* argv[]) {

  std::string bbfile = argv[1];
  std::string obsfile = argv[2];
  std::string paramfile = argv[3];
  
  bounding_box bb = load_bounding_box(bbfile);

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
  std::shared_ptr<util::Compact3D<values>> dens = stkde (bb, inst, param);
  util::timestamp end;

  if (1) {
    std::ofstream out ("log");
    //for debugging purpose
    for (int k=15; k< 16; ++k) {
      for (int i=0; i<std::min(dens->getSizeX(), 400); ++i) {
	for (int j=0; j<std::min(dens->getSizeY(), 400); ++j) { 
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
