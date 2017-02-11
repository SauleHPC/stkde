#!/bin/sh

. ./common.sh

(
    echo Instance Time Initialization Compute

    for inst in $INSTANCES
    do
	echo $(prettyname ${inst}) $(gettime ${inst} POINTBASED-SYM) $(getinit ${inst} POINTBASED-SYM) $(getcompute ${inst} POINTBASED-SYM) 
    done 

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
