#include <cstdio>
#include <cmath>
#include <iostream>
#include "types.hpp"
#include "io.hpp"
#include "density.hpp"
#include <memory>

#define N 5000

__device__ int function(int m, int n){

  return(m+n)

}

__global__ void densityG(char* f, int N){

//..
	function(f,N);
  
  
    
  int id = threadIdx.x + blockIdx.x * blockDim.x;
  if (id < N)
    //c[id] = a[id] + b[id];
    c[id]= function(a[id],b[id]);
    
//..

}


std::shared_ptr<util::Compact3D<values>> stkde_voxelbased(const bounding_box& bb,
				 const instance& inst,
				 const parameters& pa){
         
  index voxx = std::lround(std::ceil((bb.xh - bb.xl) / pa.xres)) + 1;
  index voxy = std::lround(std::ceil((bb.yh - bb.yl) / pa.yres)) + 1;
  index voxt = std::lround(std::ceil((bb.th - bb.tl) / pa.tres)) + 1;                
  
  long n = inst.obsx.size();
 
  long int evals = 0;
  

  std::shared_ptr<util::Compact3D<values>> p = std::make_shared<util::Compact3D<values>>(voxx, voxy, voxt);
  util::Compact3D<values>& co = *p;
  
  for(index i = 0; i < voxx; i++) {
    for(index j = 0; j < voxy; j++) {
      for(index k = 0; k < voxt; k++) {
	
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