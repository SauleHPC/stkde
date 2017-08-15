#ifndef DECOMP_HPP
#define DECOMP_HPP


//for a particular decomposition size, decomposition put each
//observation exactly in the decomposed region it belongs to
void decomposition(//problem
		    const bounding_box& bb,
		    const instance& inst,
		    const parameters& pa,
		    indexi decompsizeX,
		    indexi decompsizeY,
		    indexi decompsizeT,
		    //decomp
		    util::Compact3D<indexi> &load,
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


    indexi dx = (ox-bb.xl)/(bb.xh-bb.xl)*decompsizeX;
    indexi dy = (oy-bb.yl)/(bb.yh-bb.yl)*decompsizeY;
    indexi dt = (ot-bb.tl)/(bb.th-bb.tl)*decompsizeT;

    //handling points out of rnage. Processing them with near by boundary
    dx = std::max(dx, (indexi)0);
    dy = std::max(dy, (indexi)0);
    dt = std::max(dt, (indexi)0);
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

///Is [min1;max1] inter [min2;max2] non empty?
bool intersect1d (coordinate min1, coordinate max1,
		  coordinate min2, coordinate max2) {
  if (max2 < min1) return false;
  if (max1 < min2) return false;

  return true;
}


//place each observation in each decomposed space that intersect with
//the cylinder of the observation
void decomposition_ghost(//problem
			 const bounding_box& bb,
			 const instance& inst,
			 const parameters& pa,
			 indexi decompsizeX,
			 indexi decompsizeY,
			 indexi decompsizeT,
			 //decomp
			 util::Compact3D<indexi> &load,
			 util::Compact3D<std::vector<coordinate>> &decompX,
			 util::Compact3D<std::vector<coordinate>> &decompY,
			 util::Compact3D<std::vector<coordinate>> &decompT
			 )
{

  for (int dx = 0; dx<decompsizeX; ++dx) {
    for (int dy = 0; dy<decompsizeY; ++dy) {
      for (int dt = 0; dt<decompsizeT; ++dt) {
	load(dx,dy,dt) = 0;
      }
    }
  }
  

  indexi decompBWx = std::lround(std::ceil(pa.xbw/((bb.xh-bb.xl)/decompsizeX)));
  indexi decompBWy = std::lround(std::ceil(pa.ybw/((bb.yh-bb.yl)/decompsizeY)));
  indexi decompBWt = std::lround(std::ceil(pa.tbw/((bb.th-bb.tl)/decompsizeT)));
  
  //
  long inter = 0;
  for (int i=0; i< inst.obsx.size(); ++i) {
    auto ox = inst.obsx[i];
    auto oy = inst.obsy[i];
    auto ot = inst.obst[i];


    indexi odx = (ox-bb.xl)/(bb.xh-bb.xl)*decompsizeX;
    indexi ody = (oy-bb.yl)/(bb.yh-bb.yl)*decompsizeY;
    indexi odt = (ot-bb.tl)/(bb.th-bb.tl)*decompsizeT;

    //handling points out of range. Processing them with near by boundary
    odx = std::max(odx, (indexi)0);
    ody = std::max(ody, (indexi)0);
    odt = std::max(odt, (indexi)0);
    odx = std::min(odx, decompsizeX-1);
    ody = std::min(ody, decompsizeY-1);
    odt = std::min(odt, decompsizeT-1);

    
    //place in appropriate decomposition
    for (indexi dx = std::max(odx-decompBWx, (indexi)0); dx<std::min(odx+decompBWx+1, decompsizeX); ++dx) {
      coordinate decxmin = bb.xl + ( dx   *(bb.xh-bb.xl)/decompsizeX );
      coordinate decxmax = bb.xl + ((dx+1)*(bb.xh-bb.xl)/decompsizeX );

      coordinate bwxmin = ox-pa.xbw;
      coordinate bwxmax = ox+pa.xbw;
      //does it intersect?
      if (! intersect1d(bwxmin, bwxmax, decxmin, decxmax)) {
	if (0)
	    std::cerr<<"not adding "<<ox<<","<<oy<<","<<ot<<" to "
		     <<decxmin<<";"<<decxmax
		     <<std::endl;
	continue;
      }
      //this kind of intersection can cause to touch (evaluate) less
      //voxels than the sequential code would. Though it is because of
      //a rounding error in sequential. So it is begnin.
      
      
      for (indexi dy = std::max(ody-decompBWy, (indexi)0); dy < std::min(ody+decompBWy+1, decompsizeY); ++dy) {
	coordinate decymin = bb.yl + ( dy   *(bb.yh-bb.yl)/decompsizeY );
	coordinate decymax = bb.yl + ((dy+1)*(bb.yh-bb.yl)/decompsizeY );

	coordinate bwymin = oy-pa.ybw;
	coordinate bwymax = oy+pa.ybw;
	//does it intersect?
	if (! intersect1d(bwymin, bwymax, decymin, decymax)) {
	  if (0)
	    std::cerr<<"not adding "<<ox<<","<<oy<<","<<ot<<" to "
		     <<decxmin<<";"<<decxmax<<" "
		     <<decymin<<";"<<decymax
		     <<std::endl;
	  continue;
	}

	
	for (indexi dt = std::max(odt-decompBWt, (indexi)0); dt < std::min(odt+decompBWt+1, decompsizeT); ++dt) {
	  coordinate dectmin = bb.tl + ( dt   *(bb.th-bb.tl)/decompsizeT );
	  coordinate dectmax = bb.tl + ((dt+1)*(bb.th-bb.tl)/decompsizeT );

	  coordinate bwtmin = ot-pa.tbw;
	  coordinate bwtmax = ot+pa.tbw;
	  //does it intersect?
	  if (! intersect1d(bwtmin, bwtmax, dectmin, dectmax)) {
	    if (0)
	      std::cerr<<"not adding "<<ox<<","<<oy<<","<<ot<<" to "
		       <<decxmin<<";"<<decxmax<<" "
		       <<decymin<<";"<<decymax<<" "
		       <<dectmin<<";"<<dectmax<<std::endl;
	    continue;
	  }

	  if (0){
	    std::cerr<<"adding "<<ox<<","<<oy<<","<<ot<<" to "
		     <<decxmin<<";"<<decxmax<<" "
		     <<decymin<<";"<<decymax<<" "
		     <<dectmin<<";"<<dectmax<<std::endl;
	  }
	  
	  //it does intersect
	  load(dx,dy,dt) ++;
	  inter++;
	  decompX(dx,dy,dt).push_back(ox);
	  decompY(dx,dy,dt).push_back(oy);
	  decompT(dx,dy,dt).push_back(ot);

	}
      }
    }
  }
  std::cerr<<"intersect: "<<inter<<std::endl;


}
#endif

