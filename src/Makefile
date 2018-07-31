

#CXXFLAGS = -std=c++14 -O0 -g -fopenmp

#une this one for clang
#CXX=clang++-3.8
#LD=clang++-3.8
#CXXFLAGS = -fdiagnostics-color=auto -g -std=c++14 -O3 -march=native -DNO_OMP -DNDEBUG
#LDFLAGS= -O3 -march=native



#Use this one for g++
LD=g++



CXXFLAGS = -fdiagnostics-color=auto -std=c++14  -DNO_GPU
CXXFLAGS+=-O3 -march=native  -DNDEBUG
#CXXFLAGS+=-O1 -g
CXXFLAGS+=-fopenmp

LDFLAGS= -O3 -march=native -fopenmp -DNO_GPU
#LDFLAGS= -O0 -fopenmp -DNO_GPU


#use this one for nvcc
ifeq ($(MODE), nvcc)
CXX=nvcc 
LD=nvcc 
CXXFLAGS = -std=c++11 -Xcompiler "-fdiagnostics-color=auto -g -std=c++11 -O3 -march=native -DNO_OMP -DNDEBUG" -arch=sm_35
LDFLAGS= -Xlinker "-O3 -march=native" -arch=sm_35
endif

OBJS = point-based.o

all: point-based partition

point-based: point-based.o #voxel-based-gpu.o
	$(LD) $(LDFLAGS) -o point-based point-based.o #voxel-based-gpu.o

partition: partition.o #voxel-based-gpu.o
	$(LD) $(LDFLAGS) -o partition partition.o #voxel-based-gpu.o

voxel-based-gpu.o: voxel-based-gpu.cu
	$(CXX) voxel-based-gpu.cu -c -o voxel-based-gpu.o $(CXXFLAGS)

graphanalysis: graphanalysis.o
	$(LD) $(LDFLAGS) -o graphanalysis graphanalysis.o

test: point-based
	./point-based ../dataset/Dengue-allcases-2010-2011/AllCases2010_11_clip_bds.txt  ../dataset/Dengue-allcases-2010-2011/AllCases2010_11_clip.txt  ../dataset/Dengue-allcases-2010-2011/lowres-lowbw POINTBASED

test-voxel-decomp: point-based
	./point-based ../dataset/Dengue-allcases-2010-2011/AllCases2010_11_clip_bds.txt  ../dataset/Dengue-allcases-2010-2011/AllCases2010_11_clip.txt  ../dataset/Dengue-allcases-2010-2011/lowres-lowbw VOXELBASED-OBSDECOMP 8 8 8

test-voxel-omp-obsdecomp: point-based
	./point-based ../dataset/Dengue-allcases-2010-2011/AllCases2010_11_clip_bds.txt  ../dataset/Dengue-allcases-2010-2011/AllCases2010_11_clip.txt  ../dataset/Dengue-allcases-2010-2011/lowres-lowbw VOXELBASED-OMP-OBSDECOMP 8 8 8

test-voxel: point-based
	./point-based ../dataset/Dengue-allcases-2010-2011/AllCases2010_11_clip_bds.txt  ../dataset/Dengue-allcases-2010-2011/AllCases2010_11_clip.txt  ../dataset/Dengue-allcases-2010-2011/lowres-lowbw VOXELBASED

test-med: point-based
	./point-based ../dataset/Dengue-allcases-2010-2011/AllCases2010_11_clip_bds.txt  ../dataset/Dengue-allcases-2010-2011/AllCases2010_11_clip.txt  ../dataset/Dengue-allcases-2010-2011/lowres-highbw POINTBASED

test-hard: point-based
	./point-based ../dataset/Dengue-allcases-2010-2011/AllCases2010_11_clip_bds.txt  ../dataset/Dengue-allcases-2010-2011/AllCases2010_11_clip.txt  ../dataset/Dengue-allcases-2010-2011/highres-highbw POINTBASED

test-voxel-gpu: point-based
	./point-based ../dataset/Dengue-allcases-2010-2011/AllCases2010_11_clip_bds.txt  ../dataset/Dengue-allcases-2010-2011/AllCases2010_11_clip.txt  ../dataset/Dengue-allcases-2010-2011/lowres-lowbw GPU-VOXELBASED

clean:
	-rm $(OBJS)
	-rm point-based
	-rm partition
	-rm partition.o

depend:
	makedepend -Y -I . *.cpp 
# DO NOT DELETE

graphanalysis.o: graphanalysis.hpp compact.hpp compact_impl.hpp timestamp.hpp
partition.o: types.hpp compact.hpp compact_impl.hpp io.hpp timestamp.hpp
partition.o: density.hpp partition.hpp partition-over.hpp
partition.o: partition-jagged.hpp partition-jagged-over.hpp
partition.o: partition-octree.hpp
point-based.o: types.hpp compact.hpp compact_impl.hpp io.hpp timestamp.hpp
point-based.o: density.hpp point-based.hpp point-based-symdisk.hpp
point-based.o: point-based-symbar.hpp point-based-sym.hpp
point-based.o: point-based-symomp.hpp decompositions.hpp computation.hpp
point-based.o: point-based-symomp-boxdecomposition.hpp
point-based.o: point-based-symomp-pointdecomp.hpp
point-based.o: point-based-symomp-obsdecomp.hpp
point-based.o: point-based-symomp-obsdecomp-sched.hpp
point-based.o: point-based-symomp-obsdecomp-colorsched.hpp graphanalysis.hpp
point-based.o: point-based-symomp-obsdecomp-colorsched-rep.hpp
point-based.o: voxel-based.hpp voxel-based-gpu.hpp voxel-based-omp.hpp
point-based.o: voxel-based-obsdecomp.hpp voxel-based-omp-obsdecomp.hpp
t.o: timestamp.hpp
