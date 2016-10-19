#ifndef GRAPH_ANALYSIS_HPP
#define GRAPH_ANALYSIS_HPP

#include <iostream>
#include "compact.hpp"
#include <vector>
#include <queue>
#include <algorithm>


typedef long loadv;

struct triplet {
  int x,y,z;
  
  triplet(){};
  triplet(int px, int py, int pz)
    :x(px), y(py), z(pz)
  {};

  bool operator == (const triplet& t) const {
    return x == t.x && y == t.y &&z ==t.z;
  }
};


////utils
template<typename T>
void print_compact(const util::Compact3D<T> & color) {
  
  for (int i=0; i<color.getSizeX(); ++i){
    for (int j=0; j<color.getSizeY(); ++j) {
      for (int k=0; k<color.getSizeZ(); ++k) {
	std::cerr<<color(i,j,k)<<" ";
      }
      std::cerr<<std::endl;
    }
    std::cerr<<std::endl;
  }
}

void generate_in ( const util::Compact3D<std::vector<triplet> > & graph_out,
		   util::Compact3D<std::vector<triplet> > & graph_in) {

  for (int i=0; i<graph_in.getSizeX(); ++i)
    for (int j=0; j<graph_in.getSizeY(); ++j)
      for (int k=0; k<graph_in.getSizeZ(); ++k) {
	graph_in(i,j,k).clear();
      }

  for (int i=0; i<graph_in.getSizeX(); ++i)
    for (int j=0; j<graph_in.getSizeY(); ++j)
      for (int k=0; k<graph_in.getSizeZ(); ++k) {
	for (auto t : graph_out(i,j,k)) {
	  graph_in(t.x, t.y, t.z).push_back(triplet(i,j,k));
	}
      }  
}

////make graphs froms colors
void graph_out_from_color_stencil(const util::Compact3D<loadv> & color,
				  util::Compact3D<std::vector<triplet> > & graph_out ){
  for (int i=0; i<graph_out.getSizeX(); ++i)
    for (int j=0; j<graph_out.getSizeY(); ++j)
      for (int k=0; k<graph_out.getSizeZ(); ++k) {
	graph_out(i,j,k).clear();
      }

  for (int i=0; i<graph_out.getSizeX(); ++i)
    for (int j=0; j<graph_out.getSizeY(); ++j)
      for (int k=0; k<graph_out.getSizeZ(); ++k) {

	for (int dx = -1; dx < 2; ++dx)
	  for (int dy = -1; dy < 2; ++dy)
	    for (int dz = -1; dz < 2; ++dz) {
	      if (dx == 0 && dy == 0 && dz == 0) continue;
	      if (i+dx < 0) continue;
	      if (j+dy < 0) continue;
	      if (k+dz < 0) continue;
	      if (i+dx >= color.getSizeX()) continue;
	      if (j+dy >= color.getSizeY()) continue;
	      if (k+dz >= color.getSizeZ()) continue;

	      if (color(i+dx, j+dy, k+dz) > color(i,j,k)) {
		graph_out(i,j,k).push_back(triplet(i+dx, j+dy, k+dz));
	      }
	    }	      
      }  
}


void graph_out_from_sync_color(const util::Compact3D<loadv> & color,
			      util::Compact3D<std::vector<triplet> > & graph_out ){
  for (int i=0; i<graph_out.getSizeX(); ++i)
    for (int j=0; j<graph_out.getSizeY(); ++j)
      for (int k=0; k<graph_out.getSizeZ(); ++k) {
	graph_out(i,j,k).clear();
      }

  for (int i=0; i<graph_out.getSizeX(); ++i)
    for (int j=0; j<graph_out.getSizeY(); ++j)
      for (int k=0; k<graph_out.getSizeZ(); ++k) {

	for (int di=0; di<graph_out.getSizeX(); ++di)
	  for (int dj=0; dj<graph_out.getSizeY(); ++dj)
	    for (int dk=0; dk<graph_out.getSizeZ(); ++dk) {
	      if (i == di && j == dj && k==dk) continue;

	      if (color(di, dj, dk) > color(i,j,k)) {
		graph_out(i,j,k).push_back(triplet(di, dj, dk));
	      }
	    }	      
      }  
}


////compute longest path
loadv longest_path(const util::Compact3D<std::vector<triplet> > & graph_out,
		   const util::Compact3D<loadv>& load) {

  util::Compact3D<std::vector<triplet> > graph_in(graph_out.getSizeX(), graph_out.getSizeY(), graph_out.getSizeZ() );
  generate_in (graph_out, graph_in);

  //init in_degree
  util::Compact3D<int> in_degree(graph_out.getSizeX(), graph_out.getSizeY(), graph_out.getSizeZ() );
  for (int i=0; i<graph_in.getSizeX(); ++i)
    for (int j=0; j<graph_in.getSizeY(); ++j)
      for (int k=0; k<graph_in.getSizeZ(); ++k)
	in_degree(i,j,k) = graph_in(i,j,k).size();

  //init distance
  util::Compact3D<loadv> dist(graph_out.getSizeX(), graph_out.getSizeY(), graph_out.getSizeZ() );
  for (int i=0; i<graph_in.getSizeX(); ++i)
    for (int j=0; j<graph_in.getSizeY(); ++j)
      for (int k=0; k<graph_in.getSizeZ(); ++k)
	dist(i,j,k) = 0;
	
  std::queue<triplet> ready;
  //init ready
  for (int i=0; i<graph_in.getSizeX(); ++i)
    for (int j=0; j<graph_in.getSizeY(); ++j)
      for (int k=0; k<graph_in.getSizeZ(); ++k)
	if (in_degree(i,j,k) == 0)
	  ready.push(triplet(i,j,k));

  while (! ready.empty()) {
    triplet t = ready.front();
    ready.pop();

    //compute distance to end of t
    loadv maxpa = 0;
    for (auto pa : graph_in(t.x, t.y, t.z)) {
      maxpa = std::max(maxpa, dist(pa.x, pa.y, pa.z));
    }
    maxpa += load(t.x, t.y, t.z);
    
    dist(t.x, t.y, t.z) = maxpa;

    //remove (virtually) t from graph
    for (auto ch : graph_out(t.x, t.y, t.z)) {
      in_degree(ch.x, ch.y, ch.z) --;
      if (in_degree(ch.x, ch.y, ch.z) == 0)
	ready.push(ch);
    }
  }

  loadv maxd = 0;
  for (int i=0; i<graph_in.getSizeX(); ++i)
    for (int j=0; j<graph_in.getSizeY(); ++j)
      for (int k=0; k<graph_in.getSizeZ(); ++k)
	maxd = std::max(maxd, dist(i,j,k));
	
	
  return maxd;
}


//longest path taking replication into account and returning the longest path
loadv longest_path(const util::Compact3D<std::vector<triplet> > & graph_out,
		   const util::Compact3D<std::vector<triplet> > & graph_in, 
		   const util::Compact3D<loadv>& load,
		   const util::Compact3D<int>& replication,
		   std::vector<triplet>& path) {


  //init in_degree
  util::Compact3D<int> in_degree(graph_out.getSizeX(), graph_out.getSizeY(), graph_out.getSizeZ() );
  for (int i=0; i<graph_in.getSizeX(); ++i)
    for (int j=0; j<graph_in.getSizeY(); ++j)
      for (int k=0; k<graph_in.getSizeZ(); ++k)
	in_degree(i,j,k) = graph_in(i,j,k).size();

  //init distance
  util::Compact3D<loadv> dist(graph_out.getSizeX(), graph_out.getSizeY(), graph_out.getSizeZ() );
  for (int i=0; i<graph_in.getSizeX(); ++i)
    for (int j=0; j<graph_in.getSizeY(); ++j)
      for (int k=0; k<graph_in.getSizeZ(); ++k)
	dist(i,j,k) = 0;
	
  std::queue<triplet> ready;
  //init ready
  for (int i=0; i<graph_in.getSizeX(); ++i)
    for (int j=0; j<graph_in.getSizeY(); ++j)
      for (int k=0; k<graph_in.getSizeZ(); ++k)
	if (in_degree(i,j,k) == 0)
	  ready.push(triplet(i,j,k));

  //return path
  util::Compact3D<triplet> from(graph_out.getSizeX(), graph_out.getSizeY(), graph_out.getSizeZ() );
  //init to itself
  for (int i=0; i<graph_in.getSizeX(); ++i)
    for (int j=0; j<graph_in.getSizeY(); ++j)
      for (int k=0; k<graph_in.getSizeZ(); ++k)
	from(i,j,k) = triplet (i,j,k);
  
  
  while (! ready.empty()) {
    triplet t = ready.front();
    ready.pop();

    //compute distance to end of t
    loadv maxpa = 0;
    for (auto pa : graph_in(t.x, t.y, t.z)) {
      if (maxpa < dist(pa.x, pa.y, pa.z)){
	maxpa = dist(pa.x, pa.y, pa.z);
	from(t.x,t.y,t.z) = pa;
      }
    }
    maxpa += load(t.x, t.y, t.z)/replication(t.x, t.y,t.z);
    
    dist(t.x, t.y, t.z) = maxpa;

    //remove (virtually) t from graph
    for (auto ch : graph_out(t.x, t.y, t.z)) {
      in_degree(ch.x, ch.y, ch.z) --;
      if (in_degree(ch.x, ch.y, ch.z) == 0)
	ready.push(ch);
    }
  }

  //find max load and longest sink of the graph
  loadv maxd = 0;
  triplet longestsink = triplet (0,0,0);
  for (int i=0; i<graph_in.getSizeX(); ++i)
    for (int j=0; j<graph_in.getSizeY(); ++j)
      for (int k=0; k<graph_in.getSizeZ(); ++k) {
	if (maxd < dist(i,j,k)) {
	  maxd = dist(i,j,k);
	  longestsink = triplet(i,j,k);
	}
      }

  //construct longest chain
  path.clear();
  path.push_back(longestsink);
  while (1) {
    auto p = path.back();
    if (p == from(p.x, p.y, p.z)) break;
    path.push_back(from(p.x, p.y, p.z));
  } 
  
	
  return maxd;
}


////different coloring
void naive_color(util::Compact3D<loadv>& color,
		 const util::Compact3D<loadv>& load) {
  for (int i=0; i<color.getSizeX(); ++i)
    for (int j=0; j<color.getSizeY(); ++j)
      for (int k=0; k<color.getSizeZ(); ++k) {
	color(i,j,k) = ((i%2)<<2) + ((j%2)<<1) + (k%2);
      }
}

void loadbased_color(util::Compact3D<loadv>& color,
		     const util::Compact3D<loadv>& load) {

  //init color
  for (int i=0; i<color.getSizeX(); ++i)
    for (int j=0; j<color.getSizeY(); ++j)
      for (int k=0; k<color.getSizeZ(); ++k) {
	color(i,j,k) = -1;
      }

  std::vector< triplet > loadordered;

  //get load ordered list of triplet
  for (int i=0; i<color.getSizeX(); ++i)
    for (int j=0; j<color.getSizeY(); ++j)
      for (int k=0; k<color.getSizeZ(); ++k) {
	loadordered.push_back(triplet(i,j,k));
      }

  std::sort(loadordered.begin(), loadordered.end(),
	    [&load](triplet a, triplet b) {
	      return load(a.x, a.y, a.z) > load(b.x, b.y, b.z);
	    });

  std::cerr<<"most loaded: "<<loadordered[0].x<<" "<<loadordered[0].y<<" "<<loadordered[0].z<<std::endl;
  
  for (auto t :  loadordered) {
    int maxcolor = 28; //should never see a color over 27
    int forbidden[maxcolor];
    for (int i=0; i< maxcolor; ++i) {
      forbidden[i] = 0;
    }

    //mark forbidden colors
    for (int dx = -1; dx < 2; ++dx)
      for (int dy = -1; dy < 2; ++dy)
	for (int dz = -1; dz < 2; ++dz) {
	  if (dx == 0 && dy == 0 && dz == 0) continue;
	  if (t.x+dx < 0) continue;
	  if (t.y+dy < 0) continue;
	  if (t.z+dz < 0) continue;
	  if (t.x+dx >= color.getSizeX()) continue;
	  if (t.y+dy >= color.getSizeY()) continue;
	  if (t.z+dz >= color.getSizeZ()) continue;
	  
	  int fc = color(t.x+dx, t.y+dy, t.z+dz);
	  if (fc >=0 )
	    forbidden[fc] = 1;
	}
    //pick first
    int c = 0;
    while (forbidden[c] == 1)
      c++;
    color(t.x, t.y, t.z) = c;
  }
}




////generate a topological order
void topo_sort(const util::Compact3D<std::vector<triplet> > & graph_out,
		   std::vector<triplet>& topo_order) {

  util::Compact3D<std::vector<triplet> > graph_in(graph_out.getSizeX(), graph_out.getSizeY(), graph_out.getSizeZ() );
  generate_in (graph_out, graph_in);

  //init in_degree
  util::Compact3D<int> in_degree(graph_out.getSizeX(), graph_out.getSizeY(), graph_out.getSizeZ() );
  for (int i=0; i<graph_in.getSizeX(); ++i)
    for (int j=0; j<graph_in.getSizeY(); ++j)
      for (int k=0; k<graph_in.getSizeZ(); ++k)
	in_degree(i,j,k) = graph_in(i,j,k).size();

  //init distance
  util::Compact3D<loadv> dist(graph_out.getSizeX(), graph_out.getSizeY(), graph_out.getSizeZ() );
  for (int i=0; i<graph_in.getSizeX(); ++i)
    for (int j=0; j<graph_in.getSizeY(); ++j)
      for (int k=0; k<graph_in.getSizeZ(); ++k)
	dist(i,j,k) = 0;
	
  std::queue<triplet> ready;
  //init ready
  for (int i=0; i<graph_in.getSizeX(); ++i)
    for (int j=0; j<graph_in.getSizeY(); ++j)
      for (int k=0; k<graph_in.getSizeZ(); ++k)
	if (in_degree(i,j,k) == 0)
	  ready.push(triplet(i,j,k));

  while (! ready.empty()) {
    triplet t = ready.front();
    ready.pop();

    topo_order.push_back(t);

    //remove (virtually) t from graph
    for (auto ch : graph_out(t.x, t.y, t.z)) {
      in_degree(ch.x, ch.y, ch.z) --;
      if (in_degree(ch.x, ch.y, ch.z) == 0)
	ready.push(ch);
    }
  }
  
}

///replication based scheduling
//return max number of replication
int replicate(const util::Compact3D<std::vector<triplet> > & graph_out,
	      const util::Compact3D<loadv>& load,
	      util::Compact3D<int>& replication,
	      loadv threshold
	      ) {

  //make the reverse graph as we'll need it often
  util::Compact3D<std::vector<triplet> > graph_in(graph_out.getSizeX(), graph_out.getSizeY(), graph_out.getSizeZ() );
  generate_in (graph_out, graph_in);
  
  std::vector<triplet> path;

  loadv lp = 0;
  
  while ((lp = longest_path (graph_out, graph_in, load, replication, path)) > threshold) {
    //std::cerr<<"path is "<<lp<<" units long"<<std::endl;
    for (auto t : path) {
      // std::cerr<<"replicating: "<<t.x<<" "<<t.y<<" "<<t.z<<" : "
      // 	       <<replication(t.x, t.y, t.z)<<" times. "
      // 	       <<"original load : "<<load(t.x,t.y,t.z) <<" now: "<< load(t.x,t.y,t.z)/ replication(t.x, t.y, t.z)<<std::endl;

      replication(t.x, t.y, t.z)++;
      
    }
    
  }

  ///  return  # replication
  int max = 0;
  for (int i=0; i<graph_out.getSizeX(); ++i)
    for (int j=0; j<graph_out.getSizeY(); ++j)
      for (int k=0; k<graph_out.getSizeZ(); ++k) {
	max = std::max (max, replication(i,j,k));
      }
  return max;	  
}

#endif
