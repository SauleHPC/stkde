#ifndef DENSITY__HPP__
#define DENSITY__HPP__

#include <cmath>
#include "types.hpp"

inline
values densityF( coordinate obsX, coordinate obsY, coordinate obsT,
		 coordinate voxX, coordinate voxY, coordinate voxT,
		 int nbObs, coordinate xbw, coordinate tbw
		 ) {

  values u = (obsX-voxX) / xbw;
  values v = (obsY-voxY) / xbw;
  values w = (obsT-voxT) / tbw;
    
  values constantTerm = 1000.*1000.*1000.*10 / (nbObs * pow(xbw, 2) * tbw);
  values Ks = (0.5 * M_PI) * (1 - pow(u, 2) - pow(v, 2));
  values Kt = 0.75 * (1 - pow(w, 2));
  
  values spaceTimeKDE = constantTerm * Ks * Kt;
    
  return spaceTimeKDE;
}

inline
values densityF_disk( coordinate obsX, coordinate obsY, coordinate obsT,
		      coordinate voxX, coordinate voxY, coordinate voxT,
		      int nbObs, coordinate xbw, coordinate tbw
		      ) {

  values u = (obsX-voxX) / xbw;
  values v = (obsY-voxY) / xbw;
    
  values constantTerm = 1000.*1000.*1000.*10 / (nbObs * pow(xbw, 2) * tbw);
  values Ks = (0.5 * M_PI) * (1 - pow(u, 2) - pow(v, 2));
  
  values spaceTimeKDE = constantTerm * Ks ;
    
  return spaceTimeKDE;
}

inline
values densityF_bar( coordinate obsX, coordinate obsY, coordinate obsT,
		     coordinate voxX, coordinate voxY, coordinate voxT,
		     int nbObs, coordinate xbw, coordinate tbw
		     ) {

  values w = (obsT-voxT) / tbw;
    
  values Kt = 0.75 * (1 - pow(w, 2));
  
  values spaceTimeKDE = Kt;
    
  return spaceTimeKDE;
}

#endif
