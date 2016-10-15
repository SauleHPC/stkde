#!/bin/sh

gettime() {
    inst=$1
    meth=$2

    grep \^time: ${RESULTDIR}/${inst}_${meth} | cut -d \  -f 2 | awk '{printf "%.3f", $1}'
}


RESULTDIR=../results

(
    echo          \&    \&             \&           \&        \& SYM '\\\\'
    echo Instance \& PB \&  PB-SYMDISK \& PB-SYMBAR \& PB-SYM \& speedup '\\\\'
        

    for inst in Dengue_lowres-lowbw Dengue_lowres-highbw Dengue_highres-lowbw Dengue_highres-highbw Dengue_highres-veryhighbandwidth \
				    Pollen_lowres-lowbw Pollen_highres-lowbw Pollen_highres-medbw Pollen_highres-highbw \
				    Flu-Animal_lowres-lowbw Flu-Animal_lowres-highbw Flu-Animal_medres-lowbw Flu-Animal_medres-highbw Flu-Animal_highres-lowbw Flu-Animal_highres-highbw
    do
	echo $inst \\t\& \
	     $(gettime ${inst} POINTBASED) \\t\& $(gettime ${inst} POINTBASED-SYMDISK) \\t\& \
	     $(gettime ${inst} POINTBASED-SYMBAR) \\t\& $(gettime ${inst} POINTBASED-SYM) \\t\& \
	     $(echo $(gettime ${inst} POINTBASED) / $(gettime ${inst} POINTBASED-SYM) | bc -l | awk '{printf "%.3f", $1}' ) '\\\\' 
    done | sed 's/_/\\_/g' | sed 's/veryhighbandwidth/vhighbw/'
) > seq_table.tex
