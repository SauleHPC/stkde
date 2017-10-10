#ifndef COMPUTATION_STR_HPP
#define COMPUTATION_STR_HPP

namespace stkde {
  struct computation {
    //size of the space in voxels
    index voxX;
    index voxY;
    index voxT;
    
    //span of bandwidth in voxels
    index voxsbw;
    index voxtbw;
    
    //return
    std::shared_ptr<util::Compact3D<values>> p;
    
    //total obs and box
    index totalobs;
    bounding_box bb;
    
    //parameters
    parameters pa;
  };


void init_stkde (computation& c) {
  util::Compact3D<values>& co = *(c.p);

  util::timestamp init_b;
  
  //fill in zeroes
// #pragma omp parallel for collapse(2) schedule(dynamic,1)
//   for (index i=0; i< co.getSizeX(); ++i)
//     for (index j=0; j< co.getSizeY(); ++j)
//       for (index k=0; k< co.getSizeZ(); ++k)
// 	co(i,j,k) = 0;

  co.zero_parallel();
  util::timestamp init_e;
  
  std::cerr<<"Initialization time: "<<init_e-init_b<<std::endl;

}
}

#endif
