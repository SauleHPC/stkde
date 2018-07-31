#ifndef PART_OCTREE_HPP_
#define PART_OCTREE_HPP_

#include <limits>
#include <queue>

namespace stkde{
  

  std::vector<stkde::voxelbox> split_box_oct(const stkde::voxelbox& vb) {
    index voxXmid = (vb.voxXmin + vb.voxXmax) / 2;
    index voxYmid = (vb.voxYmin + vb.voxYmax) / 2;
    index voxTmid = (vb.voxTmin + vb.voxTmax) / 2;

    stkde::voxelbox vb1 (vb.voxXmin, voxXmid, vb.voxYmin, voxYmid, vb.voxTmin, voxTmid);
    stkde::voxelbox vb2 (voxXmid, vb.voxXmax, vb.voxYmin, voxYmid, vb.voxTmin, voxTmid);
    stkde::voxelbox vb3 (vb.voxXmin, voxXmid, voxYmid, vb.voxYmax, vb.voxTmin, voxTmid);
    stkde::voxelbox vb4 (voxXmid, vb.voxXmax, voxYmid, vb.voxYmax, vb.voxTmin, voxTmid);
    stkde::voxelbox vb5 (vb.voxXmin, voxXmid, vb.voxYmin, voxYmid, voxTmid, vb.voxTmax);
    stkde::voxelbox vb6 (voxXmid, vb.voxXmax, vb.voxYmin, voxYmid, voxTmid, vb.voxTmax);
    stkde::voxelbox vb7 (vb.voxXmin, voxXmid, voxYmid, vb.voxYmax, voxTmid, vb.voxTmax);
    stkde::voxelbox vb8 (voxXmid, vb.voxXmax, voxYmid, vb.voxYmax, voxTmid, vb.voxTmax);

    
    std::vector<stkde::voxelbox> ret;
    if (!vb1.empty()) ret.push_back(vb1);
    if (!vb2.empty()) ret.push_back(vb2);
    if (!vb3.empty()) ret.push_back(vb3);
    if (!vb4.empty()) ret.push_back(vb4);
    if (!vb5.empty()) ret.push_back(vb5);
    if (!vb6.empty()) ret.push_back(vb6);
    if (!vb7.empty()) ret.push_back(vb7);
    if (!vb8.empty()) ret.push_back(vb8);

    return ret;
  }
  
  std::vector<stkde::voxelbox> partition_octree(const stkde::bounding_box& bb,
						const stkde::instance& inst,
						const stkde::parameters& pa,
						int nbparts //nbparts to make
						) {//stepping

    computation c (bb,inst, pa, false);

    stkde::voxelbox vb(0, c.voxX,
		       0, c.voxY,
		       0, c.voxT);
    

    dp_parameters param(c);

    
    std::cerr<<"box size: "<<c.voxX<<" x "<<c.voxY<<" x "<<c.voxT<<std::endl;
    std::cerr<<"nbparts: "<<nbparts<<std::endl;
    
    double naive = cost_of_box(param, vb, inst);


    struct load_compare {
      const stkde::instance& inst;
      dp_parameters param;

      load_compare(const stkde::instance& insta, const dp_parameters& parama)
	:inst(insta), param(parama)
      {}
      
      bool operator()  (const stkde::voxelbox& vb1, const stkde::voxelbox& vb2) const {
	return cost_of_box(param, vb1, inst) < cost_of_box(param, vb2, inst);
      }
    };

    load_compare lc (inst, param);

    
    std::priority_queue<stkde::voxelbox,
			std::vector<stkde::voxelbox>,
			load_compare
			> pq (lc);
    pq.push(vb);
    //std::cerr<<"pushing "<<vb<<std::endl;
    
    while (pq.size() < nbparts) {
      auto b = pq.top();
      pq.pop();
      //std::cerr<<"popping "<<b<<std::endl;
      
      auto sp = split_box_oct(b);

      for (auto b1: sp) {
	pq.push(b1);
	//std::cerr<<"pushing "<<b1<<std::endl;
      }

      if (sp.size() == 1) //the biggest box is unsplittable
	break;
    }
    

    std::vector<stkde::voxelbox> sol;
    while (! pq.empty()) {
      auto b = pq.top();
      pq.pop();
      sol.push_back(b);
    }
    


    

    std::sort(sol.begin(), sol.end(),
     	      [&] (const voxelbox& a, const voxelbox&b){
    		return cost_of_box(param, a, inst) > cost_of_box(param, b, inst);
     	      });

    double maxload = cost_of_box(param, sol[0], inst);
    double sumload = 0.;
    for (auto b : sol) {
      sumload += cost_of_box(param, b, inst);
    }

    std::cerr<<"solution: "<<maxload<<" in "<<sol.size()<<" boxes."<<std::endl
     	     <<"naive: "<<naive<<" speedup:"<<naive/maxload<<std::endl
     	     <<"sumload:"<<sumload<<" overhead:"<<sumload/naive<<std::endl;

    
    
    for (auto b : sol) {
      std::cerr<<b<<" cost "<<cost_of_box(param, b, inst)<<std::endl;
    }

    return sol;
  }
}

#endif
