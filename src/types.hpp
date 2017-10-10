#ifndef TYPES_HPP_
#define TYPES_HPP_

#include <vector>
#include <memory>
#include "compact.hpp"

namespace stkde {

  typedef long index;
  typedef double coordinate;
  typedef float values;
  
  struct bounding_box {
    coordinate xl, xh;
    coordinate yl, yh;
    coordinate tl, th;
  };
  
  struct instance {
    std::vector<coordinate> obsx;
    std::vector<coordinate> obsy;
    std::vector<coordinate> obst;
  };
  
  struct parameters {
    coordinate xres;
    coordinate yres;
    coordinate tres;
    
    coordinate xbw;
    coordinate ybw;
    coordinate tbw;  
  };

  struct computation {
    //size of the space in voxels
    index voxX;
    index voxY;
    index voxT;
    
    //span of bandwidth in voxels
    index voxsbw;
    index voxtbw;
    
    //return
    std::shared_ptr<util::Compact3D<values>> p;
    
    //total obs and box
    index totalobs;
    bounding_box bb;
    
    //parameters
    parameters pa;

    computation(){} //for compatibility
    computation(const bounding_box& bb,
		const instance& inst,
		const parameters& pa){
      this->voxX = std::lround(std::ceil((bb.xh-bb.xl)/pa.xres))+1;
      this->voxY = std::lround(std::ceil((bb.yh-bb.yl)/pa.yres))+1;
      this->voxT = std::lround(std::ceil((bb.th-bb.tl)/pa.tres))+1;
      
      this->voxsbw = std::lround(std::ceil(pa.xbw/pa.xres));
      this->voxtbw = std::lround(std::ceil(pa.tbw/pa.tres));
      
      this->totalobs = inst.obsx.size();
      this->bb = bb;
      this->pa = pa;

          
      this->p = std::make_shared<util::Compact3D<values>>(this->voxX, this->voxY, this->voxT);
    }
    
  };
  
  
  ///represent a box of voxel. convention is [voXmin;voXmax[
  struct voxelbox {
    index voxXmin, voxXmax;
    index voxYmin, voxYmax;
    index voxTmin, voxTmax;

    voxelbox()//for compatibility
    {}
    
    voxelbox(index xmi, index xma,
	     index ymi, index yma,
	     index tmi, index tma)
      :voxXmin(xmi), voxXmax(xma),
       voxYmin(ymi), voxYmax(yma),
       voxTmin(tmi), voxTmax(tma)
    {}
	     
  };


}
  
#endif
