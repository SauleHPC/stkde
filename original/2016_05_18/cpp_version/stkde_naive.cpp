#include<cstring>
#include<string>
#include<cstdio>
#include<iostream>
using namespace std;

int main(int argc, char** argv) {
  int hs, ht, pts_in_box;
  int xres = 0, yres = 0, zres = 0;
  float volRatio;
  for(int k = 1; k < argc; k++) {
    char * arg = argv[k];
    int lengthOfArg = strlen(arg), tmp = 0;
    if(arg[0] == 'h' || arg[0] == 's' || arg[0] == 'p') {
      for(int p = 0; p < lengthOfArg; p++) {
        if(arg[p] >= '0' && arg[p] <= '9') {
          tmp = tmp ? (tmp * 10 + arg[p] - 48) : arg[p] - 48;
        }
      }
      if(arg[1] == 't' && arg[2] == 's')
        pts_in_box = tmp;
      else if (arg[1] == 's')
        hs = tmp;
      else ht = tmp;
    } else if(arg[0] == 'r' && arg[1] == 'e') {
      for(int p = 4; p < lengthOfArg; p++) {
        if(arg[p] >= '0' && arg[p] <= '9') {
          tmp = tmp ? (tmp * 10 + arg[p] - 48) : arg[p] - 48;
        } else {
          if(!xres) xres = tmp;
          else if(!yres) yres = tmp;
          tmp = 0;
        }
      }
      zres = tmp;
    } else {
      char ratio[10], index= 0;
      for(int p = 0; p < lengthOfArg; p++) {
        if((arg[p] >= '0' && arg[p] <= '9') || arg[p] == '.') {
          ratio[index++] = arg[p];
        }
        ratio[index] = '\0';
      }
      volRatio = stof(string(ratio));
    }
  }
  cout << "hs: " << hs << ", " << "ht: " << ht << endl;
  cout << "points_in_box: " << pts_in_box << endl;
  cout << "resolution: [" << xres << ", " << yres << ", " << zres << "]" << endl;
  cout << "volume ratio: " << volRatio << endl;
  return 0;
}