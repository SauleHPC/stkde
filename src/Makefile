LD=g++
#CXXFLAGS = -std=c++14 -O0 -g -fopenmp
CXXFLAGS = -std=c++14 -O3 -march=native -fopenmp -DNDEBUG
#CXXFLAGS = -std=c++14 -O3 -march=native  -DNDEBUG
#LDFLAGS= -O3 -march=native -fopenmp
LDFLAGS= -O3 -march=native -fopenmp
#LDFLAGS= -O0 -fopenmp

OBJS = point-based.o

point-based: point-based.o
	$(LD) $(LDFLAGS) -o point-based point-based.o

graphanalysis: graphanalysis.o
	$(LD) $(LDFLAGS) -o graphanalysis graphanalysis.o

test: point-based
	./point-based ../dataset/Dengue-allcases-2010-2011/AllCases2010_11_clip_bds.txt  ../dataset/Dengue-allcases-2010-2011/AllCases2010_11_clip.txt  ../dataset/Dengue-allcases-2010-2011/lowres-lowbw POINTBASED

test-voxel-decomp: point-based
	./point-based ../dataset/Dengue-allcases-2010-2011/AllCases2010_11_clip_bds.txt  ../dataset/Dengue-allcases-2010-2011/AllCases2010_11_clip.txt  ../dataset/Dengue-allcases-2010-2011/lowres-lowbw VOXELBASED-OBSDECOMP 8 8 8

test-voxel: point-based
	./point-based ../dataset/Dengue-allcases-2010-2011/AllCases2010_11_clip_bds.txt  ../dataset/Dengue-allcases-2010-2011/AllCases2010_11_clip.txt  ../dataset/Dengue-allcases-2010-2011/lowres-lowbw VOXELBASED

test-med: point-based
	./point-based ../dataset/Dengue-allcases-2010-2011/AllCases2010_11_clip_bds.txt  ../dataset/Dengue-allcases-2010-2011/AllCases2010_11_clip.txt  ../dataset/Dengue-allcases-2010-2011/lowres-highbw POINTBASED

test-hard: point-based
	./point-based ../dataset/Dengue-allcases-2010-2011/AllCases2010_11_clip_bds.txt  ../dataset/Dengue-allcases-2010-2011/AllCases2010_11_clip.txt  ../dataset/Dengue-allcases-2010-2011/highres-highbw POINTBASED

clean:
	-rm $(OBJS)
	-rm point-based

depend:
	makedepend -Y -I . *.cpp 
# DO NOT DELETE

graphanalysis.o: graphanalysis.hpp compact.hpp compact_impl.hpp timestamp.hpp
point-based.o: types.hpp io.hpp compact.hpp compact_impl.hpp timestamp.hpp
point-based.o: density.hpp point-based.hpp point-based-symdisk.hpp
point-based.o: point-based-symbar.hpp point-based-sym.hpp
point-based.o: point-based-symomp.hpp point-based-symomp-pointdecomp.hpp
point-based.o: point-based-symomp-obsdecomp.hpp
point-based.o: point-based-symomp-obsdecomp-sched.hpp
point-based.o: point-based-symomp-obsdecomp-colorsched.hpp graphanalysis.hpp
point-based.o: point-based-symomp-obsdecomp-colorsched-rep.hpp
point-based.o: voxel-based.hpp voxel-based-obsdecomp.hpp
t.o: timestamp.hpp
