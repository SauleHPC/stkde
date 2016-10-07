#ifndef IO_HPP__
#define IO_HPP__

#include <iostream>
#include <fstream>
#include <assert.h>
#include "types.hpp"
#include <vector>
#include <sstream>

//the bounding box file is comma separated in format xl, xh, yl, yh, tl, th
bounding_box load_bounding_box(std::string filename) {
  char c;
  bounding_box bb;

  std::ifstream in (filename.c_str());

  in >> bb.xl;
  in >> c;
  in >> bb.xh;
  in >> c;

  assert (in.good());
  
  in >> bb.yl;
  in >> c;
  in >> bb.yh;
  in >> c;

  assert (in.good());

  in >> bb.tl;
  in >> c;
  in >> bb.th;
    
  assert (!in.fail());
  
  return bb;
}

//load the observations in filename. the file format is the first line
//contains the string x y t. Then each line is composed of three
//values separated with tabs.
//
// The output vectors are cleared before anything is added
void load_observations (const std::string& filename,
			std::vector<coordinate>& xout,
			std::vector<coordinate>& yout,
			std::vector<coordinate>& tout) {

  std::ifstream in (filename.c_str());
  xout.clear();
  yout.clear();
  tout.clear();

  //discard first line
  {
    std::string line;
    std::getline(in, line);
  }
  
  while (in) {
    std::string line;
    std::getline(in, line);
    std::stringstream ss (line);
    coordinate x, y, t;
    ss>>x>>y>>t;
    if (ss) {
      xout.push_back(x);
      yout.push_back(y);
      tout.push_back(t);
    }
  }
  
  std::cerr<<"read "<<xout.size()<<" observations"<<std::endl;

}

#endif
