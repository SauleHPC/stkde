#!/bin/sh

WALLTIME="-l walltime=8:00:00"

MACHINEDESC="-l mem=110GB -l nodes=1:ppn=16:e5_2667_v3"

QUEUE="-q copperhead"

#DENGUE
qsub ${QUEUE} -d $(pwd) ${WALLTIME} ${MACHINEDESC} -v INSTANCE=Dengue,BOUNDARY=../dataset/Dengue-allcases-2010-2011/AllCases2010_11_clip_bds.txt,OBSERVATIONS=../dataset/Dengue-allcases-2010-2011/AllCases2010_11_clip.txt,BANDWIDTH=../dataset/Dengue-allcases-2010-2011/lowres-lowbw,PARTPRE=Dengue-lrlb ./run_part.sh
qsub ${QUEUE} -d $(pwd) ${WALLTIME} ${MACHINEDESC} -v INSTANCE=Dengue,BOUNDARY=../dataset/Dengue-allcases-2010-2011/AllCases2010_11_clip_bds.txt,OBSERVATIONS=../dataset/Dengue-allcases-2010-2011/AllCases2010_11_clip.txt,BANDWIDTH=../dataset/Dengue-allcases-2010-2011/lowres-medbw,PARTPRE=Dengue-lrmb ./run_part.sh
qsub ${QUEUE} -d $(pwd) ${WALLTIME} ${MACHINEDESC} -v INSTANCE=Dengue,BOUNDARY=../dataset/Dengue-allcases-2010-2011/AllCases2010_11_clip_bds.txt,OBSERVATIONS=../dataset/Dengue-allcases-2010-2011/AllCases2010_11_clip.txt,BANDWIDTH=../dataset/Dengue-allcases-2010-2011/lowres-highbw,PARTPRE=Dengue-lrhb ./run_part.sh

qsub ${QUEUE} -d $(pwd) ${WALLTIME} ${MACHINEDESC} -v INSTANCE=Dengue,BOUNDARY=../dataset/Dengue-allcases-2010-2011/AllCases2010_11_clip_bds.txt,OBSERVATIONS=../dataset/Dengue-allcases-2010-2011/AllCases2010_11_clip.txt,BANDWIDTH=../dataset/Dengue-allcases-2010-2011/highres-lowbw,PARTPRE=Dengue-hrlb ./run_part.sh
qsub ${QUEUE} -d $(pwd) ${WALLTIME} ${MACHINEDESC} -v INSTANCE=Dengue,BOUNDARY=../dataset/Dengue-allcases-2010-2011/AllCases2010_11_clip_bds.txt,OBSERVATIONS=../dataset/Dengue-allcases-2010-2011/AllCases2010_11_clip.txt,BANDWIDTH=../dataset/Dengue-allcases-2010-2011/highres-highbw,PARTPRE=Dengue-hrhb ./run_part.sh
qsub ${QUEUE} -d $(pwd) ${WALLTIME} ${MACHINEDESC} -v INSTANCE=Dengue,BOUNDARY=../dataset/Dengue-allcases-2010-2011/AllCases2010_11_clip_bds.txt,OBSERVATIONS=../dataset/Dengue-allcases-2010-2011/AllCases2010_11_clip.txt,BANDWIDTH=../dataset/Dengue-allcases-2010-2011/highres-veryhighbandwidth,PARTPRE=Dengue-hrvhb ./run_part.sh

# #FLU
# qsub ${QUEUE} -d $(pwd) ${WALLTIME} ${MACHINEDESC} -v INSTANCE=Flu-Animal,BOUNDARY=../dataset/Flu-Animal/boundary,OBSERVATIONS=../dataset/Flu-Animal/flu_data.txt,BANDWIDTH=../dataset/Flu-Animal/lowres-lowbw ./run_part.sh
# qsub ${QUEUE} -d $(pwd) ${WALLTIME} ${MACHINEDESC} -v INSTANCE=Flu-Animal,BOUNDARY=../dataset/Flu-Animal/boundary,OBSERVATIONS=../dataset/Flu-Animal/flu_data.txt,BANDWIDTH=../dataset/Flu-Animal/lowres-highbw ./run_part.sh

# qsub ${QUEUE} -d $(pwd) ${WALLTIME} ${MACHINEDESC} -v INSTANCE=Flu-Animal,BOUNDARY=../dataset/Flu-Animal/boundary,OBSERVATIONS=../dataset/Flu-Animal/flu_data.txt,BANDWIDTH=../dataset/Flu-Animal/medres-lowbw ./run_part.sh
# qsub ${QUEUE} -d $(pwd) ${WALLTIME} ${MACHINEDESC} -v INSTANCE=Flu-Animal,BOUNDARY=../dataset/Flu-Animal/boundary,OBSERVATIONS=../dataset/Flu-Animal/flu_data.txt,BANDWIDTH=../dataset/Flu-Animal/medres-highbw ./run_part.sh

# qsub ${QUEUE} -d $(pwd) ${WALLTIME} ${MACHINEDESC} -v INSTANCE=Flu-Animal,BOUNDARY=../dataset/Flu-Animal/boundary,OBSERVATIONS=../dataset/Flu-Animal/flu_data.txt,BANDWIDTH=../dataset/Flu-Animal/highres-lowbw ./run_part.sh
# qsub ${QUEUE} -d $(pwd) ${WALLTIME} ${MACHINEDESC} -v INSTANCE=Flu-Animal,BOUNDARY=../dataset/Flu-Animal/boundary,OBSERVATIONS=../dataset/Flu-Animal/flu_data.txt,BANDWIDTH=../dataset/Flu-Animal/highres-highbw ./run_part.sh

#Pollen
# qsub ${QUEUE} -d $(pwd) ${WALLTIME} ${MACHINEDESC} -v INSTANCE=Pollen,BOUNDARY=../dataset/Pollen/boundary,OBSERVATIONS=../dataset/Pollen/Pollen-xyt.txt,BANDWIDTH=../dataset/Pollen/lowres-lowbw ./run_part.sh

# qsub ${QUEUE} -d $(pwd) ${WALLTIME} ${MACHINEDESC} -v INSTANCE=Pollen,BOUNDARY=../dataset/Pollen/boundary,OBSERVATIONS=../dataset/Pollen/Pollen-xyt.txt,BANDWIDTH=../dataset/Pollen/highres-lowbw ./run_part.sh
# qsub ${QUEUE} -d $(pwd) ${WALLTIME} ${MACHINEDESC} -v INSTANCE=Pollen,BOUNDARY=../dataset/Pollen/boundary,OBSERVATIONS=../dataset/Pollen/Pollen-xyt.txt,BANDWIDTH=../dataset/Pollen/highres-medbw ./run_part.sh
# qsub ${QUEUE} -d $(pwd) ${WALLTIME} ${MACHINEDESC} -v INSTANCE=Pollen,BOUNDARY=../dataset/Pollen/boundary,OBSERVATIONS=../dataset/Pollen/Pollen-xyt.txt,BANDWIDTH=../dataset/Pollen/highres-highbw ./run_part.sh


#PollenUS
qsub ${QUEUE} -d $(pwd) ${WALLTIME} ${MACHINEDESC} -v INSTANCE=PollenUS,BOUNDARY=../dataset/Pollen/boundary-UScropped,OBSERVATIONS=../dataset/Pollen/Pollen-xyt.txt,BANDWIDTH=../dataset/Pollen/lowres-lowbw,PARTPRE=PollenUS-lrlb ./run_part.sh

qsub ${QUEUE} -d $(pwd) ${WALLTIME} ${MACHINEDESC} -v INSTANCE=PollenUS,BOUNDARY=../dataset/Pollen/boundary-UScropped,OBSERVATIONS=../dataset/Pollen/Pollen-xyt.txt,BANDWIDTH=../dataset/Pollen/highres-lowbw,PARTPRE=PollenUS-hrlb ./run_part.sh
qsub ${QUEUE} -d $(pwd) ${WALLTIME} ${MACHINEDESC} -v INSTANCE=PollenUS,BOUNDARY=../dataset/Pollen/boundary-UScropped,OBSERVATIONS=../dataset/Pollen/Pollen-xyt.txt,BANDWIDTH=../dataset/Pollen/highres-medbw,PARTPRE=PollenUS-hrmb ./run_part.sh
qsub ${QUEUE} -d $(pwd) ${WALLTIME} ${MACHINEDESC} -v INSTANCE=PollenUS,BOUNDARY=../dataset/Pollen/boundary-UScropped,OBSERVATIONS=../dataset/Pollen/Pollen-xyt.txt,BANDWIDTH=../dataset/Pollen/highres-highbw,PARTPRE=PollenUS-hrhb ./run_part.sh

qsub ${QUEUE} -d $(pwd) ${WALLTIME} ${MACHINEDESC} -v INSTANCE=PollenUS,BOUNDARY=../dataset/Pollen/boundary-UScropped,OBSERVATIONS=../dataset/Pollen/Pollen-xyt.txt,BANDWIDTH=../dataset/Pollen/veryhighres-lowbw,PARTPRE=PollenUS-vhrlb ./run_part.sh
qsub ${QUEUE} -d $(pwd) ${WALLTIME} ${MACHINEDESC} -v INSTANCE=PollenUS,BOUNDARY=../dataset/Pollen/boundary-UScropped,OBSERVATIONS=../dataset/Pollen/Pollen-xyt.txt,BANDWIDTH=../dataset/Pollen/veryhighres-verylowbw,PARTPRE=PollenUS-vhrvlb ./run_part.sh


# #eBird
# qsub ${QUEUE} -d $(pwd) ${WALLTIME} ${MACHINEDESC} -v INSTANCE=eBird,BOUNDARY=../dataset/ebird/boundary-world-20years,OBSERVATIONS=../dataset/ebird/ebird_all.txt,BANDWIDTH=../dataset/ebird/lowres-lowbw ./run_part.sh
# qsub ${QUEUE} -d $(pwd) ${WALLTIME} ${MACHINEDESC} -v INSTANCE=eBird,BOUNDARY=../dataset/ebird/boundary-world-20years,OBSERVATIONS=../dataset/ebird/ebird_all.txt,BANDWIDTH=../dataset/ebird/lowres-highbw ./run_part.sh
# qsub ${QUEUE} -d $(pwd) ${WALLTIME} ${MACHINEDESC} -v INSTANCE=eBird,BOUNDARY=../dataset/ebird/boundary-world-20years,OBSERVATIONS=../dataset/ebird/ebird_all.txt,BANDWIDTH=../dataset/ebird/highres-lowbw ./run_part.sh
# qsub ${QUEUE} -d $(pwd) ${WALLTIME} ${MACHINEDESC} -v INSTANCE=eBird,BOUNDARY=../dataset/ebird/boundary-world-20years,OBSERVATIONS=../dataset/ebird/ebird_all.txt,BANDWIDTH=../dataset/ebird/highres-highbw ./run_part.sh
