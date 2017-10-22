#ifndef PART_OVER_HPP_
#define PART_OVER_HPP_

#include <limits>

namespace stkde{


  dp_hier_val partition_hier_over_rec(dp_hier_parameters& param,
				      const stkde::instance& inst,
				      const stkde::voxelbox& b,
				      int nbparts,
				      double maxloadallowed) {

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
    ret.sumload = ((ret.maxload <= maxloadallowed)?ret.maxload : std::numeric_limits<double>::infinity());
    
    //    std::cerr<<param.nbstate<<" Computing "<<b<<" "<<nbparts<<" naive cost: "<<ret.maxload<<std::endl;

    auto considerbest = [&](dp_hier_val& left_cut, dp_hier_val& right_cut) {
	double sum_cost = left_cut.sumload + right_cut.sumload;

	if (sum_cost < ret.sumload) {
	  ret.sol.clear();
	  std::copy(left_cut.sol.begin(),
		    left_cut.sol.end(),
		    std::back_inserter(ret.sol));
	  std::copy(right_cut.sol.begin(),
		    right_cut.sol.end(),
		    std::back_inserter(ret.sol));
	  ret.maxload = std::max(left_cut.maxload, right_cut.maxload);
	  ret.sumload = left_cut.sumload + right_cut.sumload;
	}	
    };

    auto potential = [&](dp_hier_val& left_cut) {
      return left_cut.sumload < ret.sumload;
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
	
	auto left_cut = partition_hier_over_rec(param, inst, leftbox, p, maxloadallowed);

	if (!potential(left_cut))
	  break; //left_cut will only grow

	auto right_cut = partition_hier_over_rec(param, inst, rightbox, nbparts-p, maxloadallowed);

	considerbest(left_cut, right_cut);	
      }      

      //cutting y?
      for (index cut = b.voxYmin+param.ystep; cut <b.voxYmax-1; cut += param.ystep) {
	//cut is the last

	voxelbox leftbox = b;
	leftbox.voxYmax = cut;
	voxelbox rightbox = b;
	rightbox.voxYmin = cut;
	
	auto left_cut = partition_hier_over_rec(param, inst, leftbox, p, maxloadallowed);


	if (!potential(left_cut))
	  break; //left_cut will only grow
	
	auto right_cut = partition_hier_over_rec(param, inst, rightbox, nbparts-p, maxloadallowed);

	considerbest(left_cut, right_cut);	
      }      


      //cutting t?
      for (index cut = b.voxTmin+param.tstep; cut <b.voxTmax-1; cut += param.tstep) {
	//cut is the last

	voxelbox leftbox = b;
	leftbox.voxTmax = cut;
	voxelbox rightbox = b;
	rightbox.voxTmin = cut;
	
	auto left_cut = partition_hier_over_rec(param, inst, leftbox, p, maxloadallowed);


	if (!potential(left_cut))
	  break; //left_cut will only grow
	
	auto right_cut = partition_hier_over_rec(param, inst, rightbox, nbparts-p, maxloadallowed);

	considerbest(left_cut, right_cut);	
      }      


    }
    

    if (ret.sol.size() == 0) {
      std::cerr<<"WTF "<< b<<std::endl;
    }
    
    param.memo[k] = ret;
    
    return ret;    
  }


  

  std::vector<stkde::voxelbox> partition_hier_overdecompose(const stkde::bounding_box& bb,
							    const stkde::instance& inst,
							    const stkde::parameters& pa,
							    int nbparts, //nbparts to make
							    index xs, index ys, index ts, //stepping
							    double loadratio) {

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

    double naive = cost_of_box(param, vb, inst);

    double maxloadallowed=naive/nbparts*loadratio;
    
    double max_state = c.voxX/param.xstep * c.voxX/param.xstep
      * c.voxY/param.ystep * c.voxY/param.ystep
      * c.voxT/param.tstep * c.voxT/param.tstep
      * nbparts;
    
    std::cerr<<"max states: "<<max_state<<std::endl;

    double size_state = sizeof(dp_hier_key)+sizeof(dp_hier_val);//actually bigged than that, there is a vector

    std::cerr<<"mem: "<<max_state*size_state/1024./1024./1024.<<" GB (this is an order of magnitude)"<<std::endl;
    
    
    auto sol = partition_hier_over_rec(param, inst, vb, nbparts, maxloadallowed);
    

    std::cerr<<param.nbstate<<" states processed"<<std::endl;
    
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

    if (sol.sumload == std::numeric_limits<double>::infinity()) {
      sol.sol.clear();
      std::cerr<<"no good partition"<<std::endl;
    }

    return sol.sol;    
  }
}

#endif
