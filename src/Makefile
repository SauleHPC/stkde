LD=g++
#CXXFLAGS = -std=c++14 -O0 -g
CXXFLAGS = -std=c++14 -O3 -march=native

OBJS = point-based.o

point-based: point-based.o
	$(LD) $(LDFLAGS) -o point-based point-based.o

test: point-based
	./point-based ../dataset/Dengue-allcases-2010-2011/AllCases2010_11_clip_bds.txt  ../dataset/Dengue-allcases-2010-2011/AllCases2010_11_clip.txt  ../dataset/Dengue-allcases-2010-2011/lowres-lowbw POINTBASED

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

point-based.o: types.hpp io.hpp compact.hpp compact_impl.hpp timestamp.hpp
point-based.o: density.hpp point-based.hpp point-based-symdisk.hpp
point-based.o: point-based-symbar.hpp
