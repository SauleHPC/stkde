#!/bin/sh

. ./common.sh

(
    echo Instance \& PB-SYM-DR \& PB-SYM-DD \&  PB-SYM-PD \& PB-SYM-PD-SCHED \& PB-SYM-PD-SCHED-REP '\\\\'

    for inst in $INSTANCES
    do
	seq=$(gettime ${inst} POINTBASED-SYM)
	
	echo -n $(prettyname ${inst}) \  
	
	# for meth in POINTBASED-SYMOMP-POINTDECOMP_t16 \
	# 		POINTBASED-SYMOMP_64_64_64_t16 \
	# 		POINTBASED-SYMOMP-OBSDECOMP_64_64_64_t16 \
	# 		POINTBASED-SYMOMP-OBSDECOMP-COLORSCHED_64_64_64_t16 \
	# 		POINTBASED-SYMOMP-OBSDECOMP-COLORSCHED-REP_64_64_64_t16 
	for meth in POINTBASED-SYMOMP-POINTDECOMP \
			POINTBASED-SYMOMP \
			POINTBASED-SYMOMP-OBSDECOMP \
			POINTBASED-SYMOMP-OBSDECOMP-COLORSCHED \
			POINTBASED-SYMOMP-OBSDECOMP-COLORSCHED-REP

	do
	    echo -n \  \&  \ 
	    
#	    tim=$(gettime_def ${inst} ${meth})

	    tim=$(grep ^time $RESULTDIR/${inst}_${meth}_* | cut -d \  -f 2 | awk 'BEGIN{min=0;} {if (FNR == 1){min = $1} if ($1 < min){min=$1}} END{print min}')
	    
	    if [ "$tim" = "" -o "$tim" = "0" ]
	    then
		echo -n 0 \ 
	    else
		echo -n $(echo ${seq}/${tim}  | bc -l) \ 
	    fi
	done
	
	echo '\\\\'
	
    done
) > bestof.data
    

gnuplot <<EOF
set terminal pdf size 10in,3in 
set output 'bestof.pdf'

set key font ",15" samplen 1 maxrows 3
set xtics font ",15"
set ytics font ",15"
set ylabel font ",15"
set style data histogram
set style histogram cluster gap 3

set xtics rotate by -45
set style fill solid border rgb "black"
set xrange [-.5:${NB_INSTANCES}.5]
set yrange [0:18]
set ylabel 'Speedup'

plot 'bestof.data' u 3:xtic(1) title col, \
     '' u 5:xtic(1) title col, \
     '' u 7:xtic(1) title col, \
     '' u 9:xtic(1) title col, \
     '' u 11:xtic(1) title col

EOF
