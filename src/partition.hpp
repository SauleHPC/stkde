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
      index cyl_xmin = std::max(obsvx - c.voxsbw, (stkde::index)0);
      index cyl_xmax = std::min(obsvx + c.voxsbw+1, c.voxX);
      index cyl_ymin = std::max(obsvy - c.voxsbw, (stkde::index)0);
      index cyl_ymax = std::min(obsvy + c.voxsbw+1, c.voxY);
      index cyl_tmin = std::max(obsvt - c.voxtbw, (stkde::index)0);
      index cyl_tmax = std::min(obsvt + c.voxtbw+1, c.voxT);

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
  
  std::vector<stkde::voxelbox> partition_hier(const stkde::bounding_box& bb,
					      const stkde::instance& inst,
					      const stkde::parameters& pa) {


    computation c (bb,inst, pa);

    std::vector<stkde::voxelbox> ret;

    stkde::voxelbox vb(0, c.voxX,
		       0, c.voxY,
		       0, c.voxT);

    std::cout<<"cost of part: "<<cost_of_box(vb, c, inst)<<std::endl;
    
    ret.push_back(vb);

    
    return ret;    
  }
}

#endif
