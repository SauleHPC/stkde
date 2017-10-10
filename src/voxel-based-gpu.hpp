#ifndef VOXEL_GPU_
#define VOXEL_GPU_

namespace stkde {

std::shared_ptr<util::Compact3D<values>> stkde_voxelbased_gpu(const bounding_box& bb,
				 const instance& inst,
				 const parameters& pa);

#ifdef NO_GPU
std::shared_ptr<util::Compact3D<values>> stkde_voxelbased_gpu(const bounding_box& bb,
							      const instance& inst,
							      const parameters& pa) {
  std::cerr<<"stkde_voxelbased_gpu not supported!"<<std::endl;
  exit (-1);
}
  
#endif
  
}



#endif
         
