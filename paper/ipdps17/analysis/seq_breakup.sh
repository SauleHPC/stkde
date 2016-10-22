#!/bin/sh

gettime() {
    inst=$1
    meth=$2

    grep \^time: ${RESULTDIR}/${inst}_${meth} | cut -d \  -f 2 | awk '{printf "%.3f", $1}'
}

getinit() {
    inst=$1
    meth=$2

    grep \^Initialization\ time: ${RESULTDIR}/${inst}_${meth} | cut -d \  -f 3 | awk '{printf "%.4f", $1}'
}

getcompute() {
    inst=$1
    meth=$2

    grep \^compute: ${RESULTDIR}/${inst}_${meth} | cut -d \  -f 2 | awk '{printf "%.4f", $1}'
}


RESULTDIR=../results

(
    echo Instance Time Initialization Compute

    for inst in Dengue_lowres-lowbw Dengue_lowres-highbw Dengue_highres-lowbw Dengue_highres-highbw Dengue_highres-veryhighbandwidth \
				    Pollen_lowres-lowbw Pollen_highres-lowbw Pollen_highres-medbw Pollen_highres-highbw \
				    Flu-Animal_lowres-lowbw Flu-Animal_lowres-highbw Flu-Animal_medres-lowbw Flu-Animal_medres-highbw Flu-Animal_highres-lowbw Flu-Animal_highres-highbw
    do
	echo ${inst} $(gettime ${inst} POINTBASED-SYM) $(getinit ${inst} POINTBASED-SYM) $(getcompute ${inst} POINTBASED-SYM) 
    done | sed 's/-Animal//' \
	| sed 's/lowres/Lr/g' \
	| sed 's/medres/Mr/g' \
	| sed 's/highres/Hr/g' \
	| sed 's/lowbw/Lb/g' \
	| sed 's/medbw/Mb/g' \
	| sed 's/highbw/Hb/g' \
	| sed 's/veryhighbandwidth/VHb/g' \
	| sed 's/_/\\_/g'

) > seq_breakup.data

gnuplot<<EOF
set terminal pdf;
set output 'seq_breakup.pdf'

set yrange [0:1.4]
set xtics rotate -135

#set border 3
set style data histograms
set style histogram rowstacked
set style fill solid border -1
set boxwidth 0.75
#
plot 'seq_breakup.data' using (\$3/\$2):xtic(1) t column(3), \
     '' using (\$4/\$2):xtic(1) t column(4)
#     '' using ((\$2-\$3-\$4)/\$2):xtic(1) notitle

EOF
