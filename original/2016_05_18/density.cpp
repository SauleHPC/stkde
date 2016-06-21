#include<math.h>
#include<stdio.h>
#include<iostream>
using namespace std;

float densityf(float x, float y, int z, float xc, float yc, int zc, int hs, int ht, float constant) {
  float u = (x - xc) / hs;
  float v = (y - yc) / hs;
  float w = (z - zc) / ht;
  float ks = M_PI * 0.5 * (1 - u * u - v * v);
  float kt = 0.75 * (1 - w * w);
  return constant * ks * kt;
}

int main() {
  int hs = 500, ht = 7, n = 11056;
  float x = 1058021.234, y = 864864.7112, z = 60.0;
  int xres = 1, yres = 1, zres = 1;
  int hs_sq = hs * hs;
  float constant = pow(10.0, 10) / (1.0 * n * hs_sq * ht);
  long count = 0;
  long total_points = ((2 * hs) / xres + 1) * ((2 * hs) / xres  + 1);
  for(int i = -hs; i <= hs; i += xres) {
    for(int j = -hs; j <= hs; j += yres) {
      if(hs_sq >= (i * i + j * j)) {
        count++;
        for(int k = -ht; k <= ht; k += zres) {
          densityf(x, y, z, i, j, k, hs, ht, constant);
        }
      }
    }
  }
  cout << count << endl;
  cout << total_points << endl;
  cout << (0.0 + count) / total_points << endl;
  return 0;
}