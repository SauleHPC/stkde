#!/bin/sh

cpobs() {
    instance=$1

    grep maxin\ round ../results/${instance}_POINTBASED-SYMOMP-OBSDECOMP_64_64_64_t16 | cut -d: -f 2 | awk '{sum += $1;} END{print sum}'
}

cpcolorsched() {
    instance=$1

    grep load\ based\ longest\ path ../results/${instance}_POINTBASED-SYMOMP-OBSDECOMP-COLORSCHED_64_64_64_t16 | cut -d: -f 2 | awk '{sum += $1;} END{print sum}'

}

nbpoint() {
    instance=$1

    grep read ../results/${instance}_POINTBASED-SYM | grep observations | cut -d \   -f 2 
}

INSTANCES="Dengue_lowres-lowbw Dengue_lowres-highbw Dengue_highres-lowbw Dengue_highres-highbw Dengue_highres-veryhighbandwidth \
            Pollen_lowres-lowbw Pollen_highres-lowbw Pollen_highres-medbw Pollen_highres-highbw \
	    PollenUS_lowres-lowbw PollenUS_highres-lowbw PollenUS_highres-medbw PollenUS_highres-highbw PollenUS_veryhighres-lowbw PollenUS_veryhighres-verylowbw \
	    Flu-Animal_lowres-lowbw Flu-Animal_lowres-highbw Flu-Animal_medres-lowbw Flu-Animal_medres-highbw Flu-Animal_highres-lowbw Flu-Animal_highres-highbw \
            eBird_lowres-lowbw"


(
    echo instance PB-SYM-PD PB-SYM-PD-SCHED

    for instance in ${INSTANCES}
    do
	echo $instance \
	     $(echo $(cpobs $instance) / $(nbpoint $instance) | bc -l) \
	     $(echo $(cpcolorsched $instance) / $(nbpoint $instance) | bc -l)
    done  | sed 's/-Animal//' \
    | sed 's/lowres/Lr/g' \
    | sed 's/medres/Mr/g' \
    | sed 's/highres/Hr/g' \
    | sed 's/lowbw/Lb/g' \
    | sed 's/medbw/Mb/g' \
    | sed 's/highbw/Hb/g' \
    | sed 's/veryhighbandwidth/VHb/g' \
    | sed 's/_/\\_/g'
    
)> PB-SYM-PD-imbalance.data

gnuplot<<EOF

set terminal pdf
set output 'PB-SYM-PD-imbalance.pdf'

set xtic rotate by -45

set ylabel 'Relative length of the critical path'

plot 'PB-SYM-PD-imbalance.data' u 2:xtic(1) title col, \
     '' u 3:xtic(1) title col lc 3

EOF
