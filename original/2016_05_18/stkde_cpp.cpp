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

int main(int argc, char** argv) {
  int hslow = 250, hshigh = 2500;
  int htlow = 1, hthigh = 14;
  int xreslow = 100, yreslow = 100, zreslow = 1;
  int xreshigh = 50, yreshigh = 50, zreshigh = 1;
  
  string names[] = {"LOW RESOLUTION, LOW BANDWIDTH", "LOW RESOLUTION, HIGH BANDWIDTH", "HIGH RESOLUTION, LOW BANDWIDTH", "HIGH RESOLUTION, HIGH BANDWIDTH" };
  
  if(argc < 3) {
    cout << "I need resolution/bandwidth and iterations ." << endl
    << "usage: exe 0123 1000 - runs for all (low,low), (low,high), (high,low), (high,high) combinations" << endl << "1000 iterations each" << endl;
  } else {
    char* codes = argv[1];
    int iterations = atoi(argv[2]);
    cout << "Iterations: " << iterations << endl;
    for(int i = 0; codes[i] != '\0'; i++) {
      int runtype = (int) codes[i] - 48;
      int lowres = runtype < 2;
      int xres = lowres ? xreslow : xreshigh;
      int yres = lowres ? yreslow : yreshigh;
      int zres = lowres ? zreslow : zreshigh;
      // hs and ht are global
      hs = (runtype & 1) ? hshigh : hslow;
      ht = (runtype & 1) ? hthigh : htlow;
      int n = 11056;
      float x = 1058021.234, y = 864864.7112, z = 60.0;
      int hs_sq = hs * hs;
      constant = pow(10.0, 10) / (0.0 + n * hs_sq * ht) * (0.5 * 0.75 * M_PI);
      for(int j = 0; j < iterations; j++) {
        for(int i = -hs; i <= hs; i += xres) {
          for(int j = -hs; j <= hs; j += yres) {
            if(hs_sq >= (i * i + j * j)) {
              for(int k = -ht; k <= ht; k += zres) {
                densityf(x, y, z, i, j, k);
              }
            }
          }
        }
        
      }
      cout << names[runtype] << endl;
    }
  }
  return 0;
}