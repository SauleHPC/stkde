#!/bin/sh


. ./common.sh


(
    echo instance PB-SYM-PD PB-SYM-PD-SCHED

    for instance in ${INSTANCES}
    do
	echo $(prettyname ${instance}) \
	     $(echo $(cpobs $instance) / $(nbpoint $instance) | bc -l) \
	     $(echo $(cpcolorsched $instance) / $(nbpoint $instance) | bc -l)
    done  
    
)> PB-SYM-PD-imbalance.data

gnuplot<<EOF

set terminal pdf
set output 'PB-SYM-PD-imbalance.pdf'

set xtic rotate by -45

set ylabel 'Relative length of the critical path'

plot 'PB-SYM-PD-imbalance.data' u 2:xtic(1) title col, \
     '' u 3:xtic(1) title col lc 3

EOF
