#!/bin/sh

#DENGUE
qsub -q steelhead -d $(pwd) -l mem=110GB -l nodes=1:ppn=16 -v INSTANCE=Dengue,BOUNDARY=../dataset/Dengue-allcases-2010-2011/AllCases2010_11_clip_bds.txt,OBSERVATIONS=../dataset/Dengue-allcases-2010-2011/AllCases2010_11_clip.txt,BANDWIDTH=../dataset/Dengue-allcases-2010-2011/lowres-lowbw ./run_one.sh
qsub -q steelhead -d $(pwd) -l mem=110GB -l nodes=1:ppn=16 -v INSTANCE=Dengue,BOUNDARY=../dataset/Dengue-allcases-2010-2011/AllCases2010_11_clip_bds.txt,OBSERVATIONS=../dataset/Dengue-allcases-2010-2011/AllCases2010_11_clip.txt,BANDWIDTH=../dataset/Dengue-allcases-2010-2011/lowres-medbw ./run_one.sh
qsub -q steelhead -d $(pwd) -l mem=110GB -l nodes=1:ppn=16 -v INSTANCE=Dengue,BOUNDARY=../dataset/Dengue-allcases-2010-2011/AllCases2010_11_clip_bds.txt,OBSERVATIONS=../dataset/Dengue-allcases-2010-2011/AllCases2010_11_clip.txt,BANDWIDTH=../dataset/Dengue-allcases-2010-2011/lowres-highbw ./run_one.sh

qsub -q steelhead -d $(pwd) -l mem=110GB -l nodes=1:ppn=16 -v INSTANCE=Dengue,BOUNDARY=../dataset/Dengue-allcases-2010-2011/AllCases2010_11_clip_bds.txt,OBSERVATIONS=../dataset/Dengue-allcases-2010-2011/AllCases2010_11_clip.txt,BANDWIDTH=../dataset/Dengue-allcases-2010-2011/highres-lowbw ./run_one.sh
qsub -q steelhead -d $(pwd) -l mem=110GB -l nodes=1:ppn=16 -v INSTANCE=Dengue,BOUNDARY=../dataset/Dengue-allcases-2010-2011/AllCases2010_11_clip_bds.txt,OBSERVATIONS=../dataset/Dengue-allcases-2010-2011/AllCases2010_11_clip.txt,BANDWIDTH=../dataset/Dengue-allcases-2010-2011/highres-highbw ./run_one.sh
qsub -q steelhead -d $(pwd) -l mem=110GB -l nodes=1:ppn=16 -v INSTANCE=Dengue,BOUNDARY=../dataset/Dengue-allcases-2010-2011/AllCases2010_11_clip_bds.txt,OBSERVATIONS=../dataset/Dengue-allcases-2010-2011/AllCases2010_11_clip.txt,BANDWIDTH=../dataset/Dengue-allcases-2010-2011/highres-veryhighbandwidth ./run_one.sh

#FLU
qsub -q steelhead -d $(pwd) -l mem=110GB -l nodes=1:ppn=16 -v INSTANCE=Flu-Animal,BOUNDARY=../dataset/Flu-Animal/boundary,OBSERVATIONS=../dataset/Flu-Animal/flu_data.txt,BANDWIDTH=../dataset/Flu-Animal/lowres-lowbw ./run_one.sh
qsub -q steelhead -d $(pwd) -l mem=110GB -l nodes=1:ppn=16 -v INSTANCE=Flu-Animal,BOUNDARY=../dataset/Flu-Animal/boundary,OBSERVATIONS=../dataset/Flu-Animal/flu_data.txt,BANDWIDTH=../dataset/Flu-Animal/lowres-highbw ./run_one.sh

qsub -q steelhead -d $(pwd) -l mem=110GB -l nodes=1:ppn=16 -v INSTANCE=Flu-Animal,BOUNDARY=../dataset/Flu-Animal/boundary,OBSERVATIONS=../dataset/Flu-Animal/flu_data.txt,BANDWIDTH=../dataset/Flu-Animal/medres-lowbw ./run_one.sh
qsub -q steelhead -d $(pwd) -l mem=110GB -l nodes=1:ppn=16 -v INSTANCE=Flu-Animal,BOUNDARY=../dataset/Flu-Animal/boundary,OBSERVATIONS=../dataset/Flu-Animal/flu_data.txt,BANDWIDTH=../dataset/Flu-Animal/medres-highbw ./run_one.sh

qsub -q steelhead -d $(pwd) -l mem=110GB -l nodes=1:ppn=16 -v INSTANCE=Flu-Animal,BOUNDARY=../dataset/Flu-Animal/boundary,OBSERVATIONS=../dataset/Flu-Animal/flu_data.txt,BANDWIDTH=../dataset/Flu-Animal/highres-lowbw ./run_one.sh
qsub -q steelhead -d $(pwd) -l mem=110GB -l nodes=1:ppn=16 -v INSTANCE=Flu-Animal,BOUNDARY=../dataset/Flu-Animal/boundary,OBSERVATIONS=../dataset/Flu-Animal/flu_data.txt,BANDWIDTH=../dataset/Flu-Animal/highres-highbw ./run_one.sh

#Pollen
qsub -q steelhead -d $(pwd) -l mem=110GB -l nodes=1:ppn=16 -v INSTANCE=Pollen,BOUNDARY=../dataset/Pollen/boundary,OBSERVATIONS=../dataset/Pollen/Pollen-xyt.txt,BANDWIDTH=../dataset/Pollen/lowres-lowbw ./run_one.sh

qsub -q steelhead -d $(pwd) -l mem=110GB -l nodes=1:ppn=16 -v INSTANCE=Pollen,BOUNDARY=../dataset/Pollen/boundary,OBSERVATIONS=../dataset/Pollen/Pollen-xyt.txt,BANDWIDTH=../dataset/Pollen/highres-lowbw ./run_one.sh
qsub -q steelhead -d $(pwd) -l mem=110GB -l nodes=1:ppn=16 -v INSTANCE=Pollen,BOUNDARY=../dataset/Pollen/boundary,OBSERVATIONS=../dataset/Pollen/Pollen-xyt.txt,BANDWIDTH=../dataset/Pollen/highres-medbw ./run_one.sh
qsub -q steelhead -d $(pwd) -l mem=110GB -l nodes=1:ppn=16 -v INSTANCE=Pollen,BOUNDARY=../dataset/Pollen/boundary,OBSERVATIONS=../dataset/Pollen/Pollen-xyt.txt,BANDWIDTH=../dataset/Pollen/highres-highbw ./run_one.sh

