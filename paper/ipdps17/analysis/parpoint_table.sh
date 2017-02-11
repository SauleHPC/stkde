#!/bin/sh

. ./common.sh

(
    echo Instance \& PB-SYM \& t1 \& t2 \& t4 \& t8 \& t16 '\\\\'
    
    for inst in $INSTANCES
    do
	echo -n $(prettyname ${inst}) \& $(gettime $inst POINTBASED-SYM) \ 
	for t in 1 2 4 8 16
	do
	    meth="POINTBASED-SYMOMP-POINTDECOMP_t${t}"
	    echo -n \& $(gettime $inst $meth) \ 
	done
	echo '\\\\'
    done 


) >  parpoint_table.tex


for t in 1 2 4 8 16
do
    echo -n ${t} \ 

    for inst in ${INSTANCES}
    do
	seq=$(gettime $inst POINTBASED-SYM) 
	
	meth="POINTBASED-SYMOMP-POINTDECOMP_t${t}"
        algotime=$(gettime $inst $meth) 

	echo -n $(echo ${seq} / ${algotime} | bc -l) \  
	
    done
    echo

    
done > pointpointplot.data

GNUPLOTCMD="plot "
next="2"
for inst in ${INSTANCES}
do
    GNUPLOTCMD="${GNUPLOTCMD} 'pointpointplot.data' u 1:${next} t '$(prettyname ${inst})',"
    next=$(echo $next + 1 | bc)
done
GNUPLOTCMD="${GNUPLOTCMD};"
GNUPLOTCMD=$(echo ${GNUPLOTCMD} | sed 's/,;//' )

gnuplot<<EOF
set terminal pdf;
set output 'SYM-DR-speedup.pdf';
set xlabel 'Threads';
set ylabel 'Speedup';
set key top left;
set key maxrows 8;

set style data linespoints

set yrange [0:10]

$GNUPLOTCMD
EOF
