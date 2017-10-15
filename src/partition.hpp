#ifndef PARTITION_HPP_
#define PARTITION_HPP_

namespace stkde {

  double cost_of_box (const stkde::voxelbox& b, const computation& c, const instance& inst) {
    double alpha = 1; //cost of each entry of the 2d spatial grid
    double beta = 1; //cost of each entry of the 1d spatial grid
    double gamma = 1; //cost of each entry of the 3d volume

    double total_cost = 0.;
    
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
      total_cost += alpha*cyl_xsize*cyl_ysize
	+ beta*cyl_tsize
	+ gamma*cyl_tsize*cyl_xsize*cyl_ysize;
      
    }
    
    return total_cost;
  }


  std::pair<
    std::vector<stkde::voxelbox>,
    double> partition_hier_rec(const computation& c,
			       const stkde::instance& inst,
			       const stkde::voxelbox& b,
			       int nbparts) {

    std::pair<std::vector<stkde::voxelbox>,
	      double> ret;

    ret.first.push_back(b);
    ret.second = cost_of_box(b, c, inst);

    if (nbparts == 1) {
      return ret;
    }

    for (int p=1; p<nbparts; ++p) {
      //how many parts on the left

      for (index cut = b.voxXmin+1; cut <b.voxXmax-1; cut += 4) {
	//cut is the last

	voxelbox leftbox = b;
	leftbox.voxXmax = cut;
	voxelbox rightbox = b;
	rightbox.voxXmin = cut;
	
	auto left_cut = partition_hier_rec(c, inst, leftbox, p);
	auto right_cut = partition_hier_rec(c, inst, rightbox, nbparts-p);
	
	double max_cost = std::max(left_cut.second, right_cut.second);
	if (max_cost < ret.second) {
	  ret.first.clear();
	  std::copy(left_cut.first.begin(),
		    left_cut.first.end(),
		    ret.first.end());
	  std::copy(right_cut.first.begin(),
		    right_cut.first.end(),
		    ret.first.end());
	}	
      }      
    }
    
    return ret;    
  }


  
  std::vector<stkde::voxelbox> partition_hier(const stkde::bounding_box& bb,
					      const stkde::instance& inst,
					      const stkde::parameters& pa,
					      int nbparts) {


   

    computation c (bb,inst, pa);


    stkde::voxelbox vb(0, c.voxX,
		       0, c.voxY,
		       0, c.voxT);


    std::cerr<<"box size: "<<c.voxX<<" x "<<c.voxY<<" x "<<c.voxT<<std::endl;
    std::cerr<<"nbparts: "<<nbparts<<std::endl;

    
    auto sol = partition_hier_rec(c, inst, vb, nbparts);
    
    
    return sol.first;    
  }
}

#endif
