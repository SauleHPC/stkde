#include <cstdio>
#include <cmath>
#include <iostream>
#include "types.hpp"
#include "io.hpp"
#include <memory>
#include "compact.hpp"

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

__global__ void voxel_gpu(values *val,coordinate *obxx,coordinate *obyy,coordinate *obtt, parameters pa, bounding_box bb, long n, indexi sizeX, indexi sizeY, indexi sizeZ){
    
  int idx = threadIdx.x + blockIdx.x * blockDim.x;
	int idy = threadIdx.y + blockIdx.y * blockDim.y;
	int idt = threadIdx.z + blockIdx.z * blockDim.z;
	
	coordinate px = bb.xl + idx * pa.xres;
	coordinate py = bb.yl + idy * pa.yres;
	coordinate pt = bb.tl + idt * pa.tres;
	
	if(idx < sizeX && idy <sizeY && idt < sizeZ){
			for(int o=0; o < n; o++){
				coordinate ox = obxx[o];
				coordinate oy = obyy[o];
				coordinate ot = obtt[o];	  
				if(std::abs(pt - ot) <= pa.tbw) {
					if(std::sqrt((px - ox) * (px - ox) + (py - oy) * (py - oy)) <= pa.xbw) {
						val[sizeY*sizeX*idt+sizeX*idy+idx] += densityF(px, py, pt,
									ox, oy, ot,
									n, pa.xbw, pa.tbw);
						}
				}
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

  std::shared_ptr<util::Compact3D<values>> p = std::make_shared<util::Compact3D<values>>(voxx, voxy, voxt);
  util::Compact3D<values>& co = *p;
  
	dim3 bl(16,16,16);
	dim3 gr(voxx/bl.x+1,voxy/bl.y+1,voxt/bl.z+1);
	
  indexi nv = voxx * voxy * voxt;
	
 coordinate *d_obsx, *d_obsy, *d_obst;
 values *d_v;
 
 // device copy of v
 cudaMalloc((void**)&d_v, nv * sizeof(values));
 
 //device copy of the observation points
 cudaMalloc((void**)&d_obsx, n * sizeof(coordinate));
 cudaMalloc((void**)&d_obsy, n * sizeof(coordinate));
 cudaMalloc((void**)&d_obst, n * sizeof(coordinate));
 
 cudaMemcpy( d_obsx, &(inst.obsx[0]), n * sizeof(coordinate), cudaMemcpyHostToDevice);
 cudaMemcpy( d_obsy, &(inst.obsy[0]), n * sizeof(coordinate), cudaMemcpyHostToDevice);
 cudaMemcpy( d_obst, &(inst.obst[0]), n * sizeof(coordinate), cudaMemcpyHostToDevice);

 voxel_gpu<<<gr,bl>>>(d_v, d_obsx, d_obsy, d_obst, pa, bb, n, voxx, voxy, voxt); 

 //Copying from GPU to CPU
 cudaMemcpy( co.raw(), d_v, nv * sizeof(values), cudaMemcpyDeviceToHost );

 cudaError_t err = cudaGetLastError();
 if (err != cudaSuccess) {
 std::cerr<< "Error in cuda: "<< cudaGetErrorName(err)<<" : "<<cudaGetErrorString(err)<<std::endl;
}
 
 cudaFree( d_v ); cudaFree( d_obsx );cudaFree( d_obsy );cudaFree( d_obst );
  
  //std::cout<<"evals: "<< evals << std::endl;
  return p;
}