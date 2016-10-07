LD=g++
CXXFLAGS = -std=c++14 

OBJS = point-based.o

point-based: point-based.o
	$(LD) $(LDFLAGS) -o point-based point-based.o

test:
	./point-based ~/work/stkde/dataset/Dengue-allcases-2010-2011/AllCases2010_11_clip_bds.txt  ~/work/stkde/dataset/Dengue-allcases-2010-2011/AllCases2010_11_clip.txt 

depend:
	makedepend -Y -I . *.cpp 
# DO NOT DELETE

point-based.o: types.hpp io.hpp
