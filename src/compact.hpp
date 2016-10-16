
// This file is part of SPart, Spatially Located Workload Partitioner.

// Copyright (C) 2011, The Ohio State University
// SPart developed by Erdeniz O. Bas, Erik Saule and Umit V. Catalyurek 

// For questions, comments, suggestions, bugs please send e-mail to: 
// Umit V. Catalyurek   catalyurek.1@osu.edu

// SPart is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation; either version 2 or (at your option) any later
// version.

// SPart is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
// for more details.

// You should have received a copy of the GNU General Public License
// along with SPart; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA.

// As a special exception, if other files instantiate templates or use
// macros or inline functions from this file, or you compile this file
// and link it with other works to produce a work based on this file,
// this file does not by itself cause the resulting work to be covered by
// the GNU General Public License. However the source code for this file
// must still be made available in accordance with section (3) of the GNU
// General Public License.

// This exception does not invalidate any other reasons why a work based
// on this file might be covered by the GNU General Public License.

// The full text of the GPL license version 2 is available in "gpl.txt".

#ifndef __UTIL__COMPACT_H
#define __UTIL__COMPACT_H

#include <assert.h>

namespace util
{
  /**
   * 
   * @brief Provides a 2D array of T which is compact in memory to optimize caches and memory allocations
   *
   * @param T type of the data stored
   * @param DEBUG if set to true, the bounds on the primary dimension will be checked
   */
  template <typename T, bool DEBUG=false>
  class Compact2D
  {
  public:
    typedef long index;
  private:
    index sizeX, sizeY;
    T* data;
    
  private:
    ///this function is deactivated since it does not make sense
    Compact2D();
    ///this function is deactivated
    Compact2D(const Compact2D&);
    ///this function is deactivated
    const Compact2D& operator= (const Compact2D&);
    
  public:
    ///build an array of size x times y
    Compact2D(index x, index y);
    T* operator[] (index x);
    const T* operator[] (index x) const;
    ~Compact2D();
  };

  /**
   * @brief provide three dimensional arrays which are compact in memory (for cache performance).
   *
   * It does not use the [] operator but the () operator.
   */
  template <typename T>
  class Compact3D
  {
  public:
    typedef long index;

  private:
    index sizeX, sizeY, sizeZ;
    T* data;
    
  public:
    Compact3D(index x, index y, index z)
      :sizeX(x),sizeY(y), sizeZ(z)
    {
      assert (x>0);
      assert (y>0);
      assert (z>0);
      
      data = new T[sizeX*sizeY*sizeZ];
      assert (data);
    }

    void zero() {
      for (index i=0; i<sizeX*sizeY*sizeZ; ++i )
	data[i] = 0;
    }

    void zero_parallel() {
#pragma omp parallel for schedule(dynamic,512)
      for (index i=0; i<sizeX*sizeY*sizeZ; ++i )
	data[i] = 0;
    }
    
    index getSizeX() const {return sizeX;}
    index getSizeY() const {return sizeY;}
    index getSizeZ() const {return sizeZ;}
    
    inline
    const T& operator() (index x, index y, index z) const
    {
      assert (x>=0 && x < sizeX);
      assert (y>=0 && y < sizeY);
      assert (z>=0 && z < sizeZ);
      return *(data+sizeY*sizeX*z+sizeX*y+x);
    }

    inline
    T& operator() (index x, index y, index z)
    {
      assert (x>=0 && x < sizeX);
      assert (y>=0 && y < sizeY);
      assert (z>=0 && z < sizeZ);
      return *(data+sizeY*sizeX*z+sizeX*y+x);
    }

    inline
    T& operator() (index x)
    {
      assert (x>=0 && x < sizeX * sizeY * sizeZ);
      
      return *(data+x);
    }

    
    inline
    ~Compact3D()
    {
      delete[] data;
    }
  };

}

#include "compact_impl.hpp"

#endif
