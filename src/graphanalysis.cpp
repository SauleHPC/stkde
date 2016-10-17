#include "graphanalysis.hpp"
#include <fstream>
#include "timestamp.hpp"



int main (int argc, char* argv[]) {
  int sizeX = atoi(argv[1]);
  int sizeY = atoi(argv[2]);
  int sizeZ = atoi(argv[3]);

  std::string filename = argv[4];
  util::Compact3D<loadv> load (sizeX, sizeY, sizeZ);
  load.zero();
  
  {
    std::ifstream in (filename);

    while (in) {
      int x,y,z;
      loadv l;
      in>>x>>y>>z>>l;
      if (in) {
	load(x,y,z) = l;
      }
    }
  }

  util::Compact3D<std::vector<triplet> > graph_out(sizeX, sizeY, sizeZ);


  util::Compact3D<loadv> color (sizeX, sizeY, sizeZ);

  {
    naive_color(color, load);
    
    graph_out_from_sync_color (color, graph_out);
  
    std::cout<<"naive sync longest path:"<<longest_path(graph_out, load)<<std::endl;
  }

  {
    naive_color(color, load);
    
    graph_out_from_color_stencil (color, graph_out);
  
    std::cout<<"naive stencil longest path:"<<longest_path(graph_out, load)<<std::endl;
  }
  
  {

    util::timestamp beg;
    loadbased_color(color, load);
    
    graph_out_from_color_stencil (color, graph_out);
    
    std::cout<<"load based longest path:"<<longest_path(graph_out, load)<<std::endl;
    util::timestamp end;
    std::cerr<<"schedule time: "<<end-beg<<" seconds"<<std::endl;
  }


  
  //  print_compact(color);
}
