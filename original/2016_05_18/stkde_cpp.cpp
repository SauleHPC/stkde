#include<math.h>
#include<stdio.h>
#include<iostream>
using namespace std;

float constant = 0;
int hs = 0, ht = 0;

float densityf(float x, float y, int z, float xc, float yc, int zc) {
  float u = (x - xc) / hs, v = (y - yc) / hs, w = (z - zc) / ht;
  return constant * (1 - u * u - v * v) * (1 - w * w);
}

int main() {
  hs = 2500;
  ht = 14;
  int n = 11056;
  float x = 1058021.234, y = 864864.7112, z = 60.0;
  int xres = 50, yres = 50, zres = 1;
  int hs_sq = hs * hs;
  constant = pow(10.0, 10) / (0.0 + n * hs_sq * ht) * (0.5 * 0.75 * M_PI);
  int iterations = 1000;
  cout << "Iterations: " << iterations << endl;
  while(iterations--)
    for(int i = -hs; i <= hs; i += xres)
      for(int j = -hs; j <= hs; j += yres) 
        if(hs_sq >= (i * i + j * j))
          for(int k = -ht; k <= ht; k += zres)
            densityf(x, y, z, i, j, k);
            
  return 0;
}