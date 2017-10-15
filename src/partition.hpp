#ifndef PARTITION_HPP_
#define PARTITION_HPP_

#include <map>

namespace stkde {

  struct dp_hier_key {
    stkde::voxelbox box;
    int nbpart;

    bool operator<(const dp_hier_key& k2) const {
      if (this->box.voxXmin < k2.box.voxXmin)
	return true;
      if (this->box.voxXmin > k2.box.voxXmin)
	return false;
      if (this->box.voxXmax < k2.box.voxXmax)
	return true;
      if (this->box.voxXmax > k2.box.voxXmax)
	return false;

      if (this->box.voxYmin < k2.box.voxYmin)
	return true;
      if (this->box.voxYmin > k2.box.voxYmin)
	return false;
      if (this->box.voxYmax < k2.box.voxYmax)
	return true;
      if (this->box.voxYmax > k2.box.voxYmax)
	return false;

      if (this->box.voxTmin < k2.box.voxTmin)
	return true;
      if (this->box.voxTmin > k2.box.voxTmin)
	return false;
      if (this->box.voxTmax < k2.box.voxTmax)
	return true;
      if (this->box.voxTmax > k2.box.voxTmax)
	return false;
      return this->nbpart < k2.nbpart;
    }
  };

  struct dp_hier_val {
    std::vector<stkde::voxelbox> sol;
    double maxload;
  };
  
  struct dp_hier_parameters {
    double alpha; //cost of each entry of the 2d spatial grid
    double beta; //cost of each entry of the 1d spatial grid
    double gamma; //cost of each entry of the 3d volume

    const computation& c;

    index xstep;
    index ystep;
    index tstep;
    
    dp_hier_parameters (const computation& co)
      :c(co)
    {}

    std::map<dp_hier_key, dp_hier_val> memo;
    size_t nbstate;
  };
  
  double cost_of_box (const dp_hier_parameters& param, const stkde::voxelbox& b,  const instance& inst) {

    double total_cost = 0.;

    const computation& c = param.c;
    
    for (size_t ob = 0; ob < inst.obsx.size(); ++ob) {
      //observation
      stkde::coordinate ox = inst.obsx[ob];
      stkde::coordinate oy = inst.obsy[ob];
      stkde::coordinate ot = inst.obst[ob];
      
      //voxel containing the observation
      index obsvx = (ox - c.bb.xl)/c.pa.xres;
      index obsvy = (oy - c.bb.yl)/c.pa.yres;
      index obsvt = (ot - c.bb.tl)/c.pa.tres;

      //
      index cyl_xmin = std::max(obsvx - c.voxsbw, b.voxXmin);
      index cyl_xmax = std::min(obsvx + c.voxsbw+1, b.voxXmax);
      index cyl_ymin = std::max(obsvy - c.voxsbw, b.voxYmin);
      index cyl_ymax = std::min(obsvy + c.voxsbw+1, b.voxYmax);
      index cyl_tmin = std::max(obsvt - c.voxtbw, b.voxTmin);
      index cyl_tmax = std::min(obsvt + c.voxtbw+1, b.voxTmax);

      //
      index cyl_xsize = std::max(cyl_xmax-cyl_xmin, (stkde::index)0);
      index cyl_ysize = std::max(cyl_ymax-cyl_ymin, (stkde::index)0);
      index cyl_tsize = std::max(cyl_ymax-cyl_tmin, (stkde::index)0);

      //
      total_cost += param.alpha*cyl_xsize*cyl_ysize
	+ param.beta*cyl_tsize
	+ param.gamma*cyl_tsize*cyl_xsize*cyl_ysize;
      
    }
    
    return total_cost;
  }


  dp_hier_val partition_hier_rec(dp_hier_parameters& param,
			       const stkde::instance& inst,
			       const stkde::voxelbox& b,
			       int nbparts) {

    const computation& c = param.c;

    dp_hier_key k;
    k.box = b;
    k.nbpart=nbparts;
    
    auto it = param.memo.find(k);

    if (it != param.memo.end()) {
      return it->second;
    }


    param.nbstate ++;
    
    dp_hier_val ret;

    ret.sol.push_back(b);
    ret.maxload = cost_of_box(param, b, inst);

    std::cerr<<param.nbstate<<" Computing "<<b<<" "<<nbparts<<" naive cost: "<<ret.maxload<<std::endl;

    
    for (int p=1; p<nbparts; ++p) {
      //how many parts on the left

      for (index cut = b.voxXmin+1; cut <b.voxXmax-1; cut += param.xstep) {
	//cut is the last

	voxelbox leftbox = b;
	leftbox.voxXmax = cut;
	voxelbox rightbox = b;
	rightbox.voxXmin = cut;
	
	auto left_cut = partition_hier_rec(param, inst, leftbox, p);

	//TODO: maybe don't look for the next cut if you already have
	//a better solution
	auto right_cut = partition_hier_rec(param, inst, rightbox, nbparts-p);
	
	double max_cost = std::max(left_cut.maxload, right_cut.maxload);
	
	if (max_cost < ret.maxload) {
	  ret.sol.clear();
	  std::copy(left_cut.sol.begin(),
		    left_cut.sol.end(),
		    ret.sol.end());
	  std::copy(right_cut.sol.begin(),
		    right_cut.sol.end(),
		    ret.sol.end());
	  ret.maxload = max_cost;
	}	
      }      
    }
    

    param.memo[k] = ret;
    
    return ret;    
  }


  
  std::vector<stkde::voxelbox> partition_hier(const stkde::bounding_box& bb,
					      const stkde::instance& inst,
					      const stkde::parameters& pa,
					      int nbparts) {

    computation c (bb,inst, pa);

    dp_hier_parameters param (c);
    param.alpha = 1;
    param.beta = 1;
    param.gamma = 1;

    param.nbstate = 0;
    param.xstep = 16;
    param.ystep = 16;
    param.tstep = 16;
    
    stkde::voxelbox vb(0, c.voxX,
		       0, c.voxY,
		       0, c.voxT);


    std::cerr<<"box size: "<<c.voxX<<" x "<<c.voxY<<" x "<<c.voxT<<std::endl;
    std::cerr<<"nbparts: "<<nbparts<<std::endl;

    
    auto sol = partition_hier_rec(param, inst, vb, nbparts);
    

    std::cerr<<"solution of "<<sol.maxload<<std::endl;
    
    return sol.sol;    
  }
}

#endif
