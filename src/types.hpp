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
		const parameters& pa,
		bool init = true){
      this->voxX = std::lround(std::ceil((bb.xh-bb.xl)/pa.xres))+1;
      this->voxY = std::lround(std::ceil((bb.yh-bb.yl)/pa.yres))+1;
      this->voxT = std::lround(std::ceil((bb.th-bb.tl)/pa.tres))+1;
      
      this->voxsbw = std::lround(std::ceil(pa.xbw/pa.xres));
      this->voxtbw = std::lround(std::ceil(pa.tbw/pa.tres));
      
      this->totalobs = inst.obsx.size();
      this->bb = bb;
      this->pa = pa;

      if (init)
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
    

    index volume() const {
      return (voxXmax - voxXmin) *
	(voxYmax - voxYmin)*
	(voxTmax - voxTmin);
    }
  };

  
  bool intersect (const voxelbox& a, const voxelbox& b) {
    if (b.voxXmax <= a.voxXmin) return false;
    if (a.voxXmax <= b.voxXmin) return false;

    if (b.voxYmax <= a.voxYmin) return false;
    if (a.voxYmax <= b.voxYmin) return false;
    
    if (b.voxTmax <= a.voxTmin) return false;
    if (a.voxTmax <= b.voxTmin) return false;

    return true;
  }
  
  template<typename ostream>
  ostream& operator<< (ostream& out, const voxelbox& b){
    out<<b.voxXmin<<" "<<b.voxXmax
       <<" "<<b.voxYmin<<" "<<b.voxYmax
       <<" "<<b.voxTmin<<" "<<b.voxTmax;

    return out;
  }

  template<typename istream>
  istream& operator>> (istream& in, voxelbox& b){
    in>>b.voxXmin>>b.voxXmax
      >>b.voxYmin>>b.voxYmax
      >>b.voxTmin>>b.voxTmax;

    return in;
  }

  


  stkde::instance filter_instance(const stkde::instance& inst, const voxelbox& vb, const computation& c) {
    stkde::instance ret;

    for (size_t ob=0; ob<inst.obsx.size(); ++ob) {
            //observation
      stkde::coordinate ox = inst.obsx[ob];
      stkde::coordinate oy = inst.obsy[ob];
      stkde::coordinate ot = inst.obst[ob];
      
      //voxel containing the observation
      index obsvx = (ox - c.bb.xl)/c.pa.xres;
      index obsvy = (oy - c.bb.yl)/c.pa.yres;
      index obsvt = (ot - c.bb.tl)/c.pa.tres;

      //
      index cyl_xmin = obsvx - c.voxsbw;
      index cyl_xmax = obsvx + c.voxsbw+1;

      index cyl_ymin = obsvy - c.voxsbw;
      index cyl_ymax = obsvy + c.voxsbw+1;

      index cyl_tmin = obsvt - c.voxtbw;
      index cyl_tmax = obsvt + c.voxtbw+1;

      voxelbox cyl(cyl_xmin, cyl_xmax,
		   cyl_ymin, cyl_ymax,
		   cyl_tmin, cyl_tmax);

      if (intersect (cyl, vb)) {
	ret.obsx.push_back(ox);
	ret.obsy.push_back(oy);
	ret.obst.push_back(ot);
      }
      // else
      // 	std::cerr<<cyl<<" does not intersect "<<vb<<std::endl;
      
    }
    return ret;
  }
}
  
#endif
