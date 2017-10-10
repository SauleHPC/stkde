#ifndef IO_HPP__
#define IO_HPP__

#include <iostream>
#include <fstream>
#include <assert.h>
#include "types.hpp"
#include <vector>
#include <sstream>

namespace stkde {

//the bounding box file is comma separated in format xl, xh, yl, yh, tl, th
static stkde::bounding_box load_bounding_box(std::string filename) {
  char c;
  stkde::bounding_box bb;

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
    
  assert (!(in.fail()));
  
  return bb;
}

//load the observations in filename. the file format is the first line
//contains the string x y t. Then each line is composed of three
//values separated with tabs.
//
//if filename is "-", read from stdin
//
// The output vectors are cleared before anything is added
static void load_observations (const std::string& filename,
			std::vector<stkde::coordinate>& xout,
			std::vector<stkde::coordinate>& yout,
			std::vector<stkde::coordinate>& tout) {

  
  std::ifstream in (filename.c_str());
  std::istream *realin = NULL;
  if (filename.compare("-") == 0)
    realin = &(std::cin);
  else
    realin = &in;
  xout.clear();
  yout.clear();
  tout.clear();

  //discard first line
  {
    std::string line;
    std::getline(*realin, line);
  }
  
  while (*realin) {
    std::string line;
    std::getline(*realin, line);
    std::stringstream ss (line);
    stkde::coordinate x, y, t;
    ss>>x>>y>>t;
    if (ss) {
      xout.push_back(x);
      yout.push_back(y);
      tout.push_back(t);
    }
  }
  
  std::cerr<<"read "<<xout.size()<<" observations"<<std::endl;

}

//the bounding box file is comma separated in format xres, yres, tres, xbw, ybw, tbw
static stkde::parameters load_parameters(std::string filename) {
  char c;
  stkde::parameters pa;

  std::ifstream in (filename.c_str());

  in >> pa.xres;
  in >> c;
  in >> pa.yres;
  in >> c;
  in >> pa.tres;
  in >> c;
  
  assert (in.good());

  in >> pa.xbw;
  in >> c;
  in >> pa.ybw;
  in >> c;
  in >> pa.tbw;

  assert (!in.fail());
  
  return pa;
}

}
#endif
