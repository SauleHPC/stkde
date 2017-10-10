#ifndef DENSITY__HPP__
#define DENSITY__HPP__

#include <cmath>
#include "types.hpp"

inline
stkde::values densityF( stkde::coordinate obsX, stkde::coordinate obsY, stkde::coordinate obsT,
		 stkde::coordinate voxX, stkde::coordinate voxY, stkde::coordinate voxT,
		 int nbObs, stkde::coordinate xbw, stkde::coordinate tbw
		 ) {

  stkde::values u = (obsX-voxX) / xbw;
  stkde::values v = (obsY-voxY) / xbw;
  stkde::values w = (obsT-voxT) / tbw;
    
  stkde::values constantTerm = 1000.*1000.*1000.*10 / (nbObs * pow(xbw, 2) * tbw);
  stkde::values Ks = (0.5 * M_PI) * (1 - pow(u, 2) - pow(v, 2));
  stkde::values Kt = 0.75 * (1 - pow(w, 2));
  
  stkde::values spaceTimeKDE = constantTerm * Ks * Kt;
    
  return spaceTimeKDE;
}

//compute the part of the density estimation that comes from the disk
inline
stkde::values densityF_disk( stkde::coordinate obsX, stkde::coordinate obsY, stkde::coordinate obsT,
		      stkde::coordinate voxX, stkde::coordinate voxY, stkde::coordinate voxT,
		      int nbObs, stkde::coordinate xbw, stkde::coordinate tbw
		      ) {

  stkde::values u = (obsX-voxX) / xbw;
  stkde::values v = (obsY-voxY) / xbw;
    
  stkde::values constantTerm = 1000.*1000.*1000.*10 / (nbObs * pow(xbw, 2) * tbw);
  stkde::values Ks = (0.5 * M_PI) * (1 - pow(u, 2) - pow(v, 2));
  
  stkde::values spaceTimeKDE = constantTerm * Ks ;
    
  return spaceTimeKDE;
}

//compute the part of the density estimation that comes from the bar
inline
stkde::values densityF_bar( stkde::coordinate obsX, stkde::coordinate obsY, stkde::coordinate obsT,
		     stkde::coordinate voxX, stkde::coordinate voxY, stkde::coordinate voxT,
		     int nbObs, stkde::coordinate xbw, stkde::coordinate tbw
		     ) {

  stkde::values w = (obsT-voxT) / tbw;
    
  stkde::values Kt = 0.75 * (1 - pow(w, 2));
  
  stkde::values spaceTimeKDE = Kt;
    
  return spaceTimeKDE;
}

#endif
