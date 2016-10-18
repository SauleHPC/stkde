#include<iostream>
#include "timestamp.hpp"
#include <string.h>

int main() {
  long size = 1024*1000*512;
  //  float* buf = new float[size];
  float* buf;

#pragma omp parallel
  {
  }
  
  posix_memalign((void**)&buf, 64, size*sizeof(float));

  for (int i=0; i< 10; ++i) {
  
  util::timestamp beg;
#pragma omp parallel for schedule(dynamic,1)
  for (int i=0; i<size/2048; ++i)
    memset(buf+2048*i, 0, 2048*sizeof(float));

  util::timestamp end;


  std::cout<<"time: "<<end-beg
	   <<" BW:"<<(long)size*sizeof(float)/(end-beg)/1024/1024/1024<<" GB/s"<<std::endl;
  }
}
