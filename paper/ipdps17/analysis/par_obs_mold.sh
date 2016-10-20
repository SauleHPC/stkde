#!/bin/sh

gettime() {
    inst=$1
    meth=$2

    grep \^time: ${RESULTDIR}/${inst}_${meth} | cut -d \  -f 2 | awk '{printf "%.3f", $1}'
}

INSTANCES="Dengue_lowres-lowbw Dengue_lowres-highbw Dengue_highres-lowbw Dengue_highres-highbw Dengue_highres-veryhighbandwidth \
            Pollen_lowres-lowbw Pollen_highres-lowbw Pollen_highres-medbw Pollen_highres-highbw \
	    Flu-Animal_lowres-lowbw Flu-Animal_lowres-highbw Flu-Animal_medres-lowbw Flu-Animal_medres-highbw Flu-Animal_highres-lowbw Flu-Animal_highres-highbw"

RESULTDIR=../results

DECOMP="1_1_1 2_2_2 4_4_4 8_8_8 16_16_16 32_32_32 64_64_64"

(
    echo -n instance \& seq \ 
    for dec in $DECOMP
    do
	echo -n \& $dec \  
    done 
    echo '\\\\'
    
    for inst in $INSTANCES
    do
	seq=$(gettime ${inst} POINTBASED-SYM)
	t=16
	
	echo -n ${inst} \& ${seq}  \ 
	
	
	for decomp in $DECOMP
	do
	    meth=POINTBASED-SYMOMP-OBSDECOMP-COLORSCHED-REP_${decomp}_t${t}
	    
	    tim=$(gettime ${inst} ${meth})
	    echo -n \& $tim \ 
	done
	echo '\\\\'
    done
) | sed 's/_/\\_/g' >  par_obs_mold-t16.tex


