#include <cstdio>
#include <cmath>
#include <iostream>
#include "types.hpp"
#include "io.hpp"
#include "density.hpp"
#include <memory>
#include "compact.hpp"

#define N 5000

__device__ int function(int m, int n){

  return(m+n);

}

__global__ void densityG(char* f, int n){

//..
//	function(f,n);
  
  
    
  //int id = threadIdx.x + blockIdx.x * blockDim.x;
  //if (id < n)
    //c[id] = a[id] + b[id];
    //c[id]= function(a[id],b[id]);
    
//..

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
 
  // do every observation point
	for(int o = 0; o < n; o++) {
	  coordinate ox = inst.obsx[o];
	  coordinate oy = inst.obsy[o];
	  coordinate ot = inst.obst[o];	  
	  if(std::abs(pt - ot) <= pa.tbw) {
	    if(std::sqrt((px - ox) * (px - ox) + (py - oy) * (py - oy)) <= pa.xbw) {
	      v += densityF(px, py, pt,
				      ox, oy, ot,
				      n, pa.xbw, pa.tbw);
	      evals++;
	    }
	  }
	}
 
 
  co(i, j, k) = v;
     }
   }
 }
  
  std::cout<<"evals: "<< evals << std::endl;
  return p;
}