
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

template <typename T, bool DEBUG>
util::Compact2D<T, DEBUG>::Compact2D(int x, int y)
    :sizeX(x),sizeY(y)
{
  if (DEBUG)
    assert (!(x<=0 || y<=0));
  
  data = new T[sizeX*sizeY];
}

template <typename T, bool DEBUG>
T* util::Compact2D<T, DEBUG>::operator[] (int x)
{
  if (DEBUG)
    assert (!(x<0 || x >=sizeX));
  
  return data+sizeY*x;
}

template <typename T, bool DEBUG>
const T* util::Compact2D<T, DEBUG>::operator[] (int x) const
{
  if (DEBUG)
    assert (!(x<0 || x >=sizeX));

  return data+sizeY*x;
}

template <typename T, bool DEBUG>
util::Compact2D<T, DEBUG>::~Compact2D()
{
  delete[] data;
}
