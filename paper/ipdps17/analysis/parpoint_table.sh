#!/bin/sh

. ./common.sh

# (
#     echo Instance \& PB-SYM \& t1 \& t2 \& t4 \& t8 \& t16 '\\\\'
    
#     for inst in $INSTANCES
#     do
# 	echo -n $(prettyname ${inst}) \& $(gettime $inst POINTBASED-SYM) \ 
# 	for t in 1 2 4 8 16
# 	do
# 	    meth="POINTBASED-SYMOMP-POINTDECOMP_t${t}"
# 	    echo -n \& $(gettime $inst $meth) \ 
# 	done
# 	echo '\\\\'
#     done 


# ) >  parpoint_table.tex


(
    echo -n Instance \ 
    for dec in 1 2 4 8 16
    do
	echo -n ${dec} \  
    done |tr _ x
    echo
        
    for inst in ${INSTANCES}
    do
	
	echo -n $(prettyname ${inst}) \ 

	for t in 1 2 4 8 16
	do
	
	    seq=$(gettime $inst POINTBASED-SYM) 
	    
	    meth="POINTBASED-SYMOMP-POINTDECOMP_t${t}"
            algotime=$(gettime $inst $meth) 
	    
	    if [ "$algotime" = "0" -o "$algotime" = "" ]
	    then
		echo -n - \ 
	    else
		echo -n $(echo ${seq} / ${algotime} | bc -l) \  
	    fi
	done
	echo
    done
) > pointpointplot.data



gnuplot<<EOF
set terminal pdf size 10in,3.5in 
set output 'SYM-DR-speedup.pdf'

set key font ",15" samplen 1 maxrows 4
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

plot 'pointpointplot.data' using 2:xtic(1) title col, \
        '' using 3:xtic(1) title col, \
        '' using 4:xtic(1) title col, \
        '' using 5:xtic(1) title col, \
        '' using 6:xtic(1) title col
EOF
