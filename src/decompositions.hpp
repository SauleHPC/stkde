#ifndef DECOMP_HPP
#define DECOMP_HPP


//for a particular decomposition size, decomposition put each
//observation exactly in the decomposed region it belongs to
void decomposition(//problem
		    const bounding_box& bb,
		    const instance& inst,
		    const parameters& pa,
		    index decompsizeX,
		    index decompsizeY,
		    index decompsizeT,
		    //decomp
		    util::Compact3D<index> &load,
		    util::Compact3D<std::vector<coordinate>> &decompX,
		    util::Compact3D<std::vector<coordinate>> &decompY,
		    util::Compact3D<std::vector<coordinate>> &decompT
) {
  for (int dx = 0; dx<decompsizeX; ++dx) {
    for (int dy = 0; dy<decompsizeY; ++dy) {
      for (int dt = 0; dt<decompsizeT; ++dt) {
	load(dx,dy,dt) = 0;
      }
    }
  }

  for (int i=0; i< inst.obsx.size(); ++i) {
    auto ox = inst.obsx[i];
    auto oy = inst.obsy[i];
    auto ot = inst.obst[i];


    index dx = (ox-bb.xl)/(bb.xh-bb.xl)*decompsizeX;
    index dy = (oy-bb.yl)/(bb.yh-bb.yl)*decompsizeY;
    index dt = (ot-bb.tl)/(bb.th-bb.tl)*decompsizeT;

    //handling points out of rnage. Processing them with near by boundary
    dx = std::max(dx, (index)0);
    dy = std::max(dy, (index)0);
    dt = std::max(dt, (index)0);
    dx = std::min(dx, decompsizeX-1);
    dy = std::min(dy, decompsizeY-1);
    dt = std::min(dt, decompsizeT-1);
    
    //it does intersect
    load(dx,dy,dt) ++;
    //inter++;
    decompX(dx,dy,dt).push_back(ox);
    decompY(dx,dy,dt).push_back(oy);
    decompT(dx,dy,dt).push_back(ot);
  }
}
  
#endif

