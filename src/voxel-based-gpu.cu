#include <cstdio>
#include <cmath>
#include <iostream>
#include "types.hpp"
#include "io.hpp"

#include <memory>
#include "compact.hpp"

#define N 5000

__device__ values densityF(coordinate obsX, coordinate obsY, coordinate obsT,
		 coordinate voxX, coordinate voxY, coordinate voxT,
		 int nbObs, coordinate xbw, coordinate tbw
		 ){

  values u = (obsX-voxX) / xbw;
  values v = (obsY-voxY) / xbw;
  values w = (obsT-voxT) / tbw;
    
  values constantTerm = 1000.*1000.*1000.*10 / (nbObs * pow(xbw, 2) * tbw);
  values Ks = (0.5 * M_PI) * (1 - pow(u, 2) - pow(v, 2));
  values Kt = 0.75 * (1 - pow(w, 2));
  
  values spaceTimeKDE = constantTerm * Ks * Kt;
    
  return spaceTimeKDE;

}

__global__ void voxel_gpu(value *val, long int *ev,coordinate *xx,coordinate *yy,coordinate *tt,coordinate *obxx,coordinate *obyy,coordinate *obtt,coordinate *xw,coordinate *tw){
    
  int id = threadIdx.x + blockIdx.x * blockDim.x;

  coordinate ox = obxx[id];
  coordinate oy = obyy[id];
  coordinate ot = obtt[id];	  
  if(std::abs(tt - ot) <= tw) {
    if(std::sqrt((xx - ox) * (xx - ox) + (yy - oy) * (yy - oy)) <= xw) {
      val += densityF(xx, yy, tt,
			      ox, oy, ot,
			      id, xw, tw);
      ev++;
    }
  }
}


std::shared_ptr<util::Compact3D<values>> stkde_voxelbased_gpu(const bounding_box& bb,
				 const instance& inst,
				 const parameters& pa){
         
				 std::cout<<"inside stkde_voxelbased_gpu"<<std::endl;
				 
  indexi voxx = std::lround(std::ceil((bb.xh - bb.xl) / pa.xres)) + 1;
  indexi voxy = std::lround(std::ceil((bb.yh - bb.yl) / pa.yres)) + 1;
  indexi voxt = std::lround(std::ceil((bb.th - bb.tl) / pa.tres)) + 1;                
  
  long n = inst.obsx.size();
 
  long int evals = 0;
  

  std::shared_ptr<util::Compact3D<values>> p = std::make_shared<util::Compact3D<values>>(voxx, voxy, voxt);
  util::Compact3D<values>& co = *p;
  
  for(indexi i = 0; i < voxx; i++) {
    for(indexi j = 0; j < voxy; j++) {
      for(indexi k = 0; k < voxt; k++) {
	
	// contruct the point
	coordinate px = bb.xl + i * pa.xres;
	coordinate py = bb.yl + j * pa.yres;
	coordinate pt = bb.tl + k * pa.tres;

	values v = 0;
 
 coordinate *d_px, *d_py, *d_pt;
 coordinate *d_obsx[n], *d_obsy[n], *d_obst[n];
 
 // device copy of v
 cudaMalloc((void**)&d_v, sizeof(values));
 
 // device copy of the points
 cudaMalloc((void**)&d_px, sizeof(coordinate));
 cudaMalloc((void**)&d_py, sizeof(coordinate));
 cudaMalloc((void**)&d_pt, sizeof(coordinate));
 
 //device copy of the observation points
 cudaMalloc((void**)&d_obsx, n * sizeof(coordinate));
 cudaMalloc((void**)&d_obsy, n * sizeof(coordinate));
 cudaMalloc((void**)&d_obst, n * sizeof(coordinate));
 
 //device copy of pa.xbw and pa.tbw
 cudaMalloc((void**)&d_pax, sizeof(coordinate));
 cudaMalloc((void**)&d_pat, sizeof(coordinate));
 
 //device copy of evals
 cudaMalloc((void**)&d_e, sizeof(long int));
 
 //Copying to the GPU
 
 cudaMemcpy( d_v, v, sizeof(values), cudaMemcpyHostToDevice);
 
 cudaMemcpy( d_px, px, sizeof(coordinate), cudaMemcpyHostToDevice);
 cudaMemcpy( d_py, py, sizeof(coordinate), cudaMemcpyHostToDevice);
 cudaMemcpy( d_pt, pt, sizeof(coordinate), cudaMemcpyHostToDevice);

 cudaMemcpy( d_obx, inst.obsx, n * sizeof(coordinate), cudaMemcpyHostToDevice);
 cudaMemcpy( d_oby, inst.obsy, n * sizeof(coordinate), cudaMemcpyHostToDevice);
 cudaMemcpy( d_obt, inst.obst, n * sizeof(coordinate), cudaMemcpyHostToDevice);
 
 cudaMemcpy( d_pax, pa.xbw, sizeof(coordinate), cudaMemcpyHostToDevice);
 cudaMemcpy( d_pat, pa.tbw, sizeof(coordinate), cudaMemcpyHostToDevice);
 
 cudaMemcpy( d_e, evals, sizeof(long int), cudaMemcpyHostToDevice);

 voxel_gpu<<<(n+127)/128,128>>>(d_v, d_e, d_px, d_py, d_pt, d_obx, d_oby, d_obt, d_pax, d_pat); 

 //Copying from GPU to CPU
 cudaMemcpy( v, d_v, sizeof(values), cudaMemcpyDeviceToHost);
 cudaMemcpy( evals, d_c, sizeof(long int), cudaMemcpyDeviceToHost);
 
  co(i, j, k) = v;
     }
   }
 }
  
  std::cout<<"evals: "<< evals << std::endl;
  return p;
}