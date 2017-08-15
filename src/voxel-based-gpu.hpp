#ifndef VOXEL_GPU_
#define VOXEL_GPU_

std::shared_ptr<util::Compact3D<values>> stkde_voxelbased_gpu(const bounding_box& bb,
				 const instance& inst,
				 const parameters& pa);
         
#endif
         