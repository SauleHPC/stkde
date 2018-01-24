#ifndef PARTITION_HPP_
#define PARTITION_HPP_

#include <map>
#include <algorithm>

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
    double sumload;
  };
  
  struct dp_parameters {
    double alpha; //cost of each entry of the 2d spatial grid
    double beta; //cost of each entry of the 1d spatial grid
    double gamma; //cost of each entry of the 3d volume

    const computation& c;

    dp_parameters (const computation& co)
      :c(co)
    {}
  };

  struct dp_hier_parameters: public dp_parameters {
    
    index xstep;
    index ystep;
    index tstep;

    dp_hier_parameters (const computation& co)
      :dp_parameters(co)
    {}
    

    std::map<dp_hier_key, dp_hier_val> memo;
    size_t nbstate;
  };
  
  double cost_of_box (const dp_parameters& param, const stkde::voxelbox& b,  const instance& inst) {

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

      index cyl_tsize = std::max(cyl_tmax-cyl_tmin, (stkde::index)0);

      //
      total_cost += param.alpha*cyl_xsize*cyl_ysize
	+ param.beta*cyl_tsize
	+ param.gamma*cyl_tsize*cyl_xsize*cyl_ysize;
      
    }

    //    std::cerr<<b<<" "<<inst.obsx.size()<<" "<<total_cost<<std::endl;
    
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
    ret.sumload = ret.maxload;
    
    //    std::cerr<<param.nbstate<<" Computing "<<b<<" "<<nbparts<<" naive cost: "<<ret.maxload<<std::endl;

    auto considerbest = [&](dp_hier_val& left_cut, dp_hier_val& right_cut) {
	double max_cost = std::max(left_cut.maxload, right_cut.maxload);

	if (max_cost < ret.maxload) {
	  ret.sol.clear();
	  std::copy(left_cut.sol.begin(),
		    left_cut.sol.end(),
		    std::back_inserter(ret.sol));
	  std::copy(right_cut.sol.begin(),
		    right_cut.sol.end(),
		    std::back_inserter(ret.sol));
	  ret.maxload = max_cost;
	  ret.sumload = left_cut.sumload + right_cut.sumload;
	}	
    };

    auto potential = [&](dp_hier_val& left_cut) {
      return left_cut.maxload < ret.maxload;
    };
    
    for (int p=1; p<nbparts; ++p) {
      //how many parts on the left

      //cutting x?
      for (index cut = b.voxXmin+param.xstep; cut <b.voxXmax-1; cut += param.xstep) {
	//cut is the last

	voxelbox leftbox = b;
	leftbox.voxXmax = cut;
	voxelbox rightbox = b;
	rightbox.voxXmin = cut;
	
	auto left_cut = partition_hier_rec(param, inst, leftbox, p);

	if (!potential(left_cut))
	  break; //left will only get larger

	auto right_cut = partition_hier_rec(param, inst, rightbox, nbparts-p);

	considerbest(left_cut, right_cut);	
      }      

      //cutting y?
      for (index cut = b.voxYmin+param.ystep; cut <b.voxYmax-1; cut += param.ystep) {
	//cut is the last

	voxelbox leftbox = b;
	leftbox.voxYmax = cut;
	voxelbox rightbox = b;
	rightbox.voxYmin = cut;
	
	auto left_cut = partition_hier_rec(param, inst, leftbox, p);


	if (!potential(left_cut))
	  break; //left will only get larger from now onx
	
	auto right_cut = partition_hier_rec(param, inst, rightbox, nbparts-p);

	considerbest(left_cut, right_cut);	
      }      


      //cutting t?
      for (index cut = b.voxTmin+param.tstep; cut <b.voxTmax-1; cut += param.tstep) {
	//cut is the last

	voxelbox leftbox = b;
	leftbox.voxTmax = cut;
	voxelbox rightbox = b;
	rightbox.voxTmin = cut;
	
	auto left_cut = partition_hier_rec(param, inst, leftbox, p);


	if (!potential(left_cut))
	  break; //left will only get larger
	
	auto right_cut = partition_hier_rec(param, inst, rightbox, nbparts-p);

	considerbest(left_cut, right_cut);	
      }      


    }
    

    if (ret.sol.size() == 0) {
      std::cerr<<"WTF "<< b<<std::endl;
    }
    
    param.memo[k] = ret;
    
    return ret;    
  }


  
  std::vector<stkde::voxelbox> partition_hier(const stkde::bounding_box& bb,
					      const stkde::instance& inst,
					      const stkde::parameters& pa,
					      int nbparts, //nbparts to make
					      index xs, index ys, index ts) { //stepping

    computation c (bb,inst, pa, false);

    dp_hier_parameters param (c);
    param.alpha = 1.2e-08;
    param.beta = 5.3e-09;
    param.gamma = 2.2e-09;

    param.nbstate = 0;
    param.xstep = xs;
    param.ystep = ys;
    param.tstep = ts;
    
    stkde::voxelbox vb(0, c.voxX,
		       0, c.voxY,
		       0, c.voxT);

    

    std::cerr<<"box size: "<<c.voxX<<" x "<<c.voxY<<" x "<<c.voxT<<std::endl;
    std::cerr<<"nbparts: "<<nbparts<<std::endl;

    double max_state = c.voxX/param.xstep * c.voxX/param.xstep
      * c.voxY/param.ystep * c.voxY/param.ystep
      * c.voxT/param.tstep * c.voxT/param.tstep
      * nbparts;
    
    std::cerr<<"max states: "<<max_state<<std::endl;

    double size_state = sizeof(dp_hier_key)+sizeof(dp_hier_val);//actually bigged than that, there is a vector

    std::cerr<<"mem: "<<max_state*size_state/1024./1024./1024.<<" GB (this is an order of magnitude)"<<std::endl;
    
    
    auto sol = partition_hier_rec(param, inst, vb, nbparts);
    
    double naive = cost_of_box(param, vb, inst);

    std::cerr<<param.memo.size()<<" states processed"<<std::endl;
    
    std::cerr<<"solution: "<<sol.maxload<<" in "<<sol.sol.size()<<" boxes."<<std::endl
	     <<"naive: "<<naive<<" speedup:"<<naive/sol.maxload<<std::endl
	     <<"sumload:"<<sol.sumload<<" overhead:"<<sol.sumload/naive<<std::endl;

    std::sort(sol.sol.begin(), sol.sol.end(),
	      [&] (const voxelbox& a, const voxelbox&b){
		return cost_of_box(param, a, inst) > cost_of_box(param, b, inst);
	      });
    
    
    for (auto b : sol.sol) {
      std::cerr<<b<<" cost "<<cost_of_box(param, b, inst)<<std::endl;
    }

    return sol.sol;    
  }

  std::vector<stkde::voxelbox> grid_partition (const stkde::bounding_box& bb,
					       const stkde::instance& inst,
					       const stkde::parameters& pa,
					       index decompsizeX, index decompsizeY, index decompsizeT) {
    std::vector<stkde::voxelbox> decomp;

    stkde::computation c (bb, inst, pa, false);

    for (int dx = 0; dx<decompsizeX; ++dx) {
      for (int dy = 0; dy<decompsizeY; ++dy) {
	for (int dt = 0; dt<decompsizeT; ++dt) {

	  stkde::coordinate decxmin = bb.xl + ( dx   *(bb.xh-bb.xl)/decompsizeX );
	  stkde::index voxXmin = std::lround(std::ceil((decxmin-bb.xl)/pa.xres));
	  stkde::coordinate decxmax = bb.xl + ((dx+1)*(bb.xh-bb.xl)/decompsizeX );
	  stkde::index voxXmax;
	  if (dx != decompsizeX-1)
	    voxXmax = std::lround(std::floor((decxmax-bb.xl)/pa.xres))+1;
	  else
	    voxXmax = c.voxX;
	  
	  stkde::coordinate decymin = bb.yl + ( dy   *(bb.yh-bb.yl)/decompsizeY );
	  stkde::index voxYmin = std::lround(std::ceil((decymin-bb.yl)/pa.yres));
	  stkde::coordinate decymax = bb.yl + ((dy+1)*(bb.yh-bb.yl)/decompsizeY );
	  stkde::index voxYmax;
	  if (dy != decompsizeY-1)
	    voxYmax = std::lround(std::floor((decymax-bb.yl)/pa.yres))+1;
	  else
	    voxYmax = c.voxY;
	  
	  
	  stkde::coordinate dectmin = bb.tl + ( dt   *(bb.th-bb.tl)/decompsizeT );
	  stkde::index voxTmin = std::lround(std::ceil((dectmin-bb.tl)/pa.tres));
	  stkde::coordinate dectmax = bb.tl + ((dt+1)*(bb.th-bb.tl)/decompsizeT );
	  stkde::index voxTmax;
	  if (dt != decompsizeT-1)
	    voxTmax = std::lround(std::floor((dectmax-bb.tl)/pa.tres))+1;
	  else
	    voxTmax = c.voxT;


	  stkde::voxelbox vb(voxXmin, voxXmax,voxYmin, voxYmax,voxTmin, voxTmax);
	  decomp.push_back(vb);
	}
      }
    }

    dp_hier_parameters param (c);
    double naive = cost_of_box(param, voxelbox(0, c.voxX, 0, c.voxY, 0, c.voxT), inst);


    double maxload = 0;
    double sumload = 0;
    for (auto& b : decomp) {
      auto c = cost_of_box (param, b, inst);
      maxload = std::max(maxload, c);
      sumload += c;
    }

    std::cerr<<"naive: "<<naive<<" speedup:"<<naive/maxload<<std::endl
	     <<"sumload:"<<sumload<<" overhead:"<<sumload/naive<<std::endl;


    return decomp;
  }


  

}

#endif
