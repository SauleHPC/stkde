#ifndef PART_JAGGED_OVER_HPP_
#define PART_JAGGED_OVER_HPP_

#include <limits>

namespace stkde{

  struct jagged_level_one_key {
    index xmax;
    int nbpart;    

    bool operator<(const jagged_level_one_key& k2) const {
      if (this->xmax < k2.xmax)
	return true;
      if (this->xmax > k2.xmax)
	return false;

      return this->nbpart < k2.nbpart;
    }
  };

  struct jagged_level_two_key {
    index xmin, xmax;
    int nbpart;    

    bool operator<(const jagged_level_two_key& k2) const {
      if (this->xmax < k2.xmax)
	return true;
      if (this->xmax > k2.xmax)
	return false;
      
      if (this->xmin < k2.xmin)
	return true;
      if (this->xmin > k2.xmin)
	return false;
      
      return this->nbpart < k2.nbpart;
    }

  };

  struct jagged_level_three_key {
    index xmin, xmax;
    index ymax;
    int nbpart;    

    bool operator<(const jagged_level_three_key& k2) const {
      if (this->xmax < k2.xmax)
	return true;
      if (this->xmax > k2.xmax)
	return false;
      
      if (this->xmin < k2.xmin)
	return true;
      if (this->xmin > k2.xmin)
	return false;

      if (this->ymax < k2.ymax)
	return true;
      if (this->ymax > k2.ymax)
	return false;

      
      return this->nbpart < k2.nbpart;
    }

  };

  
  struct jagged_level_four_key {
    index xmin, xmax;
    index ymin, ymax;
    int nbpart;    

    bool operator<(const jagged_level_four_key& k2) const {
      if (this->xmax < k2.xmax)
	return true;
      if (this->xmax > k2.xmax)
	return false;
      
      if (this->xmin < k2.xmin)
	return true;
      if (this->xmin > k2.xmin)
	return false;

      if (this->ymax < k2.ymax)
	return true;
      if (this->ymax > k2.ymax)
	return false;

      if (this->ymin < k2.ymin)
	return true;
      if (this->ymin > k2.ymin)
	return false;

      
      return this->nbpart < k2.nbpart;
    }

  };
  
  struct jagged_level_five_key {
    index xmin, xmax;
    index ymin, ymax;
    index tmax;
    int nbpart;    

    bool operator<(const jagged_level_five_key& k2) const {
      if (this->xmax < k2.xmax)
	return true;
      if (this->xmax > k2.xmax)
	return false;
      
      if (this->xmin < k2.xmin)
	return true;
      if (this->xmin > k2.xmin)
	return false;

      if (this->ymax < k2.ymax)
	return true;
      if (this->ymax > k2.ymax)
	return false;

      if (this->ymin < k2.ymin)
	return true;
      if (this->ymin > k2.ymin)
	return false;

      if (this->tmax < k2.tmax)
	return true;
      if (this->tmax > k2.tmax)
	return false;

      
      return this->nbpart < k2.nbpart;
    }

  };

  
  
  struct dp_jagged_parameters: public dp_parameters {

    index xstep;
    index ystep;
    index tstep;

    dp_jagged_parameters (const computation& co)
      :dp_parameters(co)
    {}

    std::map<jagged_level_one_key, dp_hier_val> memo_l1; // an xcut cube
    std::map<jagged_level_two_key, dp_hier_val> memo_l2; //a full X slice
    std::map<jagged_level_three_key, dp_hier_val> memo_l3; //an xslice ycut
    std::map<jagged_level_four_key, dp_hier_val> memo_l4; //a full XY cut
    std::map<jagged_level_five_key, dp_hier_val> memo_l5; //a z cut of an XY box
    size_t nbstate;
  };


  dp_hier_val partition_jaggedZ_over_rec(dp_jagged_parameters& param,
					 const stkde::instance& inst,
					 index Xmin,
					 index Xmax,
					 index Ymin,
					 index Ymax,
					 index Tmax,
					 int nbparts,
					 double maxloadallowed) {
    const computation& c = param.c;
    
    jagged_level_five_key k;
    k.xmin = Xmin;
    k.xmax = Xmax;
    k.ymax = Ymax;
    k.ymin = Ymin;
    k.tmax = Tmax;
    k.nbpart = nbparts;
    
    auto it = param.memo_l5.find(k);

    if (it != param.memo_l5.end()) {
      return it->second;
    }

    dp_hier_val ret;
    
    //naive solution: one box
    {
      stkde::voxelbox vb(Xmin, Xmax,
			 Ymin, Ymax,
			 0, Tmax);
          
      ret.sol.push_back(vb);
      ret.maxload = cost_of_box(param, vb, inst);
      ret.sumload = ((ret.maxload <= maxloadallowed)?ret.maxload : std::numeric_limits<double>::infinity());
    }

    //helper function in the DP
    auto considerbest = [&](dp_hier_val& left_cut, dp_hier_val& right_cut) {
	double sum_cost = left_cut.sumload + right_cut.sumload;

	if (sum_cost < ret.sumload) {
	  //	  std::cout<<"new sol"<<std::endl;

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

    auto potential = [&](const dp_hier_val& left_cut) {
      return left_cut.sumload < ret.sumload;
    };

    auto remain_is_small = [&](const voxelbox& vb, int nbparts) {
      double remain_cost = cost_of_box(param, vb, inst)/nbparts;
      return remain_cost < maxloadallowed;
    };
    

    if (nbparts > 1) {
      //all tcut
      for (index cut = 0+param.tstep; cut <Tmax-1; cut += param.tstep) {
	//cut is the last
	

	dp_hier_val right_cut;
	//base case is one box
	{
	  stkde::voxelbox vb(Xmin, Xmax,
			     Ymin, Ymax,
			     cut, Tmax);
          
	  right_cut.sol.push_back(vb);
	  right_cut.maxload = cost_of_box(param, vb, inst);
	  right_cut.sumload = ((ret.maxload <= maxloadallowed)?ret.maxload : std::numeric_limits<double>::infinity());
	}
	
	if (!potential(right_cut))
	  continue;

	if (!remain_is_small(voxelbox(Xmin,Xmax,Ymin,Ymax,0,cut), nbparts))
	  continue;
	
	auto left_cut = partition_jaggedZ_over_rec (param, inst,
						    Xmin, Xmax, Ymin, Ymax, cut,
						    nbparts-1, maxloadallowed); //left is a recursive jagged cut
	
	

	considerbest(left_cut, right_cut);	
      }
    }

    
    
    //save solution and return it
    param.memo_l5[k] = ret;
    
    return ret;    

    
  }
	     
  dp_hier_val partition_jaggedZ(dp_jagged_parameters& param,
				const stkde::instance& inst,
				index Xmin,
				index Xmax,
				index Ymin,
				index Ymax,
				int nbparts,
				double maxloadallowed) {
    const computation& c = param.c;

    jagged_level_four_key k;
    k.xmin = Xmin;
    k.xmax = Xmax;
    k.ymin = Ymin;
    k.ymax = Ymax;
    k.nbpart = nbparts;
    
    auto it = param.memo_l4.find(k);

    if (it != param.memo_l4.end()) {
      return it->second;
    }

    dp_hier_val ret = partition_jaggedZ_over_rec  (param, inst,
						   Xmin, Xmax, Ymin, Ymax, c.voxT,
						   nbparts, maxloadallowed);
    
    
    //save solution and return it
    param.memo_l4[k] = ret;
    
    return ret;    
  }

  
  

  dp_hier_val partition_jaggedY_over_rec(dp_jagged_parameters& param,
					 const stkde::instance& inst,
					 index Xmin,
					 index Xmax,
					 index Ymax,
					 int nbparts,
					 double maxloadallowed) {
    const computation& c = param.c;

    jagged_level_three_key k;
    k.xmin = Xmin;
    k.xmax = Xmax;
    k.ymax = Ymax;
    k.nbpart = nbparts;
    
    auto it = param.memo_l3.find(k);

    if (it != param.memo_l3.end()) {
      return it->second;
    }

    dp_hier_val ret;
    
    //naive solution: one box
    {
      stkde::voxelbox vb(Xmin, Xmax,
			 0, Ymax,
			 0, c.voxT);
          
      ret.sol.push_back(vb);
      ret.maxload = cost_of_box(param, vb, inst);
      ret.sumload = ((ret.maxload <= maxloadallowed)?ret.maxload : std::numeric_limits<double>::infinity());
    }

    //helper function in the DP
    auto considerbest = [&](dp_hier_val& left_cut, dp_hier_val& right_cut) {
	double sum_cost = left_cut.sumload + right_cut.sumload;

	if (sum_cost < ret.sumload) {
	  //	  std::cout<<"new sol"<<std::endl;

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
    
    auto remain_is_small = [&](const voxelbox& vb, int nbparts) {
      double remain_cost = cost_of_box(param, vb, inst)/nbparts;
      return remain_cost < maxloadallowed;
    };


    //all ycut
    for (int p=1; p<nbparts; ++p) {
      //how many parts on the left

      //cutting y?
      for (index cut = 0+param.ystep; cut <Ymax-1; cut += param.ystep) {
	//cut is the last
	
	auto left_cut = partition_jaggedY_over_rec (param, inst,
						    Xmin, Xmax, cut,
						    p, maxloadallowed); //left is a recursive jagged cut

	if (!potential(left_cut))
	  continue;

	// if (!remain_is_small(voxelbox(Xmin,Xmax, 0,cut, 0,c.voxT), nbparts))
	//   continue;
	
	auto right_cut = partition_jaggedZ(param, inst, Xmin, Xmax, cut, Ymax, nbparts-p, maxloadallowed);

	considerbest(left_cut, right_cut);	
      }      
    }

    //no ycut solution
    {
      auto right_cut = partition_jaggedZ(param, inst, Xmin, Xmax, 0, Ymax, nbparts, maxloadallowed);

	//fake to reuse considerbest
	dp_hier_val left_cut;
	left_cut.maxload=0;
	left_cut.sumload=0;

	considerbest(left_cut, right_cut);
    }
    
    //save solution and return it
    param.memo_l3[k] = ret;
    
    return ret;    
  }

  
  dp_hier_val partition_jaggedY(dp_jagged_parameters& param,
				const stkde::instance& inst,
				index Xmin,
				index Xmax,
				int nbparts,
				double maxloadallowed) {
    const computation& c = param.c;

    jagged_level_two_key k;
    k.xmin = Xmin;
    k.xmax = Xmax;
    k.nbpart = nbparts;
    
    auto it = param.memo_l2.find(k);

    if (it != param.memo_l2.end()) {
      return it->second;
    }

    auto ret = partition_jaggedY_over_rec (param, inst,
					   Xmin, Xmax, c.voxY,
					   nbparts, maxloadallowed);
        
    //save solution and return it
    param.memo_l2[k] = ret;
    
    return ret;    
  }

  
  dp_hier_val partition_jaggedX_over_rec(dp_jagged_parameters& param,
					 const stkde::instance& inst,
					 index X,
					 int nbparts,
					 double maxloadallowed) {

    const computation& c = param.c;

    jagged_level_one_key k;
    k.xmax = X;
    k.nbpart=nbparts;
    
    auto it = param.memo_l1.find(k);

    if (it != param.memo_l1.end()) {
      return it->second;
    }

    param.nbstate ++;
    
    dp_hier_val ret;

    //naive solution: one box
    {
      stkde::voxelbox vb(0, X,
			 0, c.voxY,
			 0, c.voxT);
          
      ret.sol.push_back(vb);
      ret.maxload = cost_of_box(param, vb, inst);
      ret.sumload = ((ret.maxload <= maxloadallowed)?ret.maxload : std::numeric_limits<double>::infinity());
    }
    
    //    std::cerr<<param.nbstate<<" Computing "<<b<<" "<<nbparts<<" naive cost: "<<ret.maxload<<std::endl;

    auto considerbest = [&](dp_hier_val& left_cut, dp_hier_val& right_cut) {
	double sum_cost = left_cut.sumload + right_cut.sumload;

	if (sum_cost < ret.sumload) {
	  //	  std::cout<<"new sol"<<std::endl;

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
    
    auto remain_is_small = [&](const voxelbox& vb, int nbparts) {
      double remain_cost = cost_of_box(param, vb, inst)/nbparts;
      return remain_cost < maxloadallowed;
    };

    //all solutions with an X cut
    for (int p=1; p<nbparts; ++p) {
      //how many parts on the left

      //cutting x?
      for (index cut = 0+param.xstep; cut <X-1; cut += param.xstep) {
	//cut is the last

	
	auto left_cut = partition_jaggedX_over_rec (param, inst, cut, p, maxloadallowed); //left is a recursive jagged cut

	if (!potential(left_cut))
	  continue;

	if (!remain_is_small(voxelbox(0,X,0,c.voxY,0,c.voxT), nbparts))
	  continue;
	
	auto right_cut = partition_jaggedY(param, inst, cut, X, nbparts-p, maxloadallowed);

	considerbest(left_cut, right_cut);	
      }      
    }

    //no xcut solution
    {
	auto right_cut = partition_jaggedY(param, inst, 0, X, nbparts, maxloadallowed);

	//fake to reuse considerbest
	dp_hier_val left_cut;
	left_cut.maxload=0;
	left_cut.sumload=0;

	considerbest(left_cut, right_cut);
    }

    if (ret.sol.size() == 0) {
      std::cerr<<"WTF "<< X<<std::endl;
    }
    
    param.memo_l1[k] = ret;
    
    return ret;    
  }


  

  std::vector<stkde::voxelbox> partition_jagged_overdecompose(const stkde::bounding_box& bb,
							      const stkde::instance& inst,
							      const stkde::parameters& pa,
							      int nbparts, //nbparts to make
							      index xs, index ys, index ts, //stepping
							      double loadratio) {

    computation c (bb,inst, pa, false);

    dp_jagged_parameters param (c);
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

    double maxloadallowed = naive/nbparts*loadratio;
    
    double max_state = c.voxX/param.xstep * c.voxX/param.xstep
      * c.voxY/param.ystep * c.voxY/param.ystep
      * c.voxT/param.tstep * c.voxT/param.tstep
      * nbparts;
    
    std::cerr<<"max states: "<<max_state<<std::endl;

    double size_state = sizeof(dp_hier_key)+sizeof(dp_hier_val);//actually bigged than that, there is a vector

    std::cerr<<"mem: "<<max_state*size_state/1024./1024./1024.<<" GB (this is an order of magnitude)"<<std::endl;
    
    
    auto sol = partition_jaggedX_over_rec(param, inst, c.voxX, nbparts, maxloadallowed);
    

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

    return sol.sol;    
  }
}

#endif
