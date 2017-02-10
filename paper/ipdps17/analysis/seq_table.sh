#!/bin/sh

gettime() {
    inst=$1
    meth=$2

    grep \^time: ${RESULTDIR}/${inst}_${meth} | cut -d \  -f 2 | awk '{printf "%.3f", $1}'
}


RESULTDIR=../results

(
    echo '\\begin{tabular}{|l|rr|rrrr|r|}'
    echo \\hline
    echo          \& \\multicolumn{6}{c\|}{Time \(in seconds\)}        \& speedup'\\\\'
    echo Instance \& VB \& VB-DEC \& PB \&  PB-DISK \& PB-BAR \& PB-SYM \& PB-SYM '\\\\'
    echo \\hline
	    

    for inst in Dengue_lowres-lowbw Dengue_lowres-highbw Dengue_highres-lowbw Dengue_highres-highbw Dengue_highres-veryhighbandwidth \
				    Pollen_lowres-lowbw Pollen_highres-lowbw Pollen_highres-medbw Pollen_highres-highbw \
				    PollenUS_lowres-lowbw PollenUS_highres-lowbw PollenUS_highres-medbw PollenUS_highres-highbw PollenUS_veryhighres-lowbw PollenUS_veryhighres-verylowbw \
				    Flu-Animal_lowres-lowbw Flu-Animal_lowres-highbw Flu-Animal_medres-lowbw Flu-Animal_medres-highbw Flu-Animal_highres-lowbw Flu-Animal_highres-highbw \
				    eBird_lowres-lowbw
    do
	echo $inst \\t\& \
	     $(gettime ${inst} VOXELBASED-OMP_t1) \\t\& $(gettime ${inst} VOXELBASED-OMP-OBSDECOMP_1024_1024_1024_t1) \\t\& \
	     $(gettime ${inst} POINTBASED) \\t\& $(gettime ${inst} POINTBASED-SYMDISK) \\t\& \
	     $(gettime ${inst} POINTBASED-SYMBAR) \\t\& $(gettime ${inst} POINTBASED-SYM) \\t\& \
	     $(echo $(gettime ${inst} POINTBASED) / $(gettime ${inst} POINTBASED-SYM) | bc -l | awk '{printf "%.3f", $1}' ) '\\\\' 
    done | sed 's/-Animal//' \
	| sed 's/lowres/Lr/g' \
	| sed 's/medres/Mr/g' \
	| sed 's/veryhighres/VHr/g' \
	| sed 's/highres/Hr/g' \
	| sed 's/verylowbw/VLb/g' \
	| sed 's/lowbw/Lb/g' \
	| sed 's/medbw/Mb/g' \
	| sed 's/highbw/Hb/g' \
	| sed 's/veryhighbandwidth/VHb/g' \
	| sed 's/_/\\_/g'

    echo \\hline
    echo '\\end{tabular}'
) > seq_table.tex
