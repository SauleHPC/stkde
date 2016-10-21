#!/bin/sh

gettime() {
    inst=$1
    meth=$2

    grep \^time: ${RESULTDIR}/${inst}_${meth} | cut -d \  -f 2 | awk '{printf "%.3f", $1}'
}


RESULTDIR=../results

(
    echo instance \& seq \& t1 \& t2 \& t4 \& t8 \& t16 '\\\\'
    
    for inst in Dengue_lowres-lowbw Dengue_lowres-highbw Dengue_highres-lowbw Dengue_highres-highbw Dengue_highres-veryhighbandwidth \
				    Pollen_lowres-lowbw Pollen_highres-lowbw Pollen_highres-medbw Pollen_highres-highbw \
				    Flu-Animal_lowres-lowbw Flu-Animal_lowres-highbw Flu-Animal_medres-lowbw Flu-Animal_medres-highbw Flu-Animal_highres-lowbw Flu-Animal_highres-highbw
    do
	echo -n $inst \& $(gettime $inst POINTBASED-SYM) \ 
	for t in 1 2 4 8 16
	do
	    meth="POINTBASED-SYMOMP-POINTDECOMP_t${t}"
	    echo -n \& $(gettime $inst $meth) \ 
	done
	echo '\\\\'
    done | sed 's/-Animal//' \
	| sed 's/lowres/Lr/g' \
	| sed 's/medres/Mr/g' \
	| sed 's/highres/Hr/g' \
	| sed 's/lowbw/Lb/g' \
	| sed 's/medbw/Mb/g' \
	| sed 's/highbw/Hb/g' \
	| sed 's/veryhighbandwidth/VHb/g' \
	| sed 's/_/\\_/g'


) >  parpoint_table.tex
