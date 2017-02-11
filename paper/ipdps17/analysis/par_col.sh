#!/bin/sh

. ./common.sh

RESULTDIR=../results

DECOMP="1_1_1 2_2_2 4_4_4 8_8_8 16_16_16 32_32_32 64_64_64"

(
    echo '\\begin{tabular}{|l|r|rrrrrrr|}'
    echo \\hline

    echo -n Instance \& PB-SYM \ 
    for dec in $DECOMP
    do
	echo -n \& $dec \  
    done | tr _ x
    echo '\\\\'

    echo \\hline
    
    for inst in $INSTANCES
    do
	seq=$(gettime ${inst} POINTBASED-SYM)
	t=16
	
	echo -n $(prettyname ${inst}) \& ${seq}  \ 
	
	
	for decomp in $DECOMP
	do
	    meth=POINTBASED-SYMOMP-OBSDECOMP_${decomp}_t${t}
	    
	    tim=$(gettime ${inst} ${meth})
	    echo -n \& $tim \ 
	done
	echo '\\\\'
    done

    echo \\hline
    echo '\\end{tabular}'

)  >  parcol-t16.tex


#speedup bar chart
(
    echo -n Instance \ 
    for dec in $DECOMP
    do
	echo -n ${dec} \  
    done |tr _ x
    echo
    
    for inst in $INSTANCES
    do
	seq=$(gettime ${inst} POINTBASED-SYM)
	t=16
	
	echo -n $(prettyname ${inst}) \  

	for decomp in $DECOMP
	do
	    meth=POINTBASED-SYMOMP-OBSDECOMP_${decomp}_t${t}
	    
	    tim=$(gettime ${inst} ${meth})
	    if [ "$tim" = "0" ]
	    then
		echo -n 0 \ 
	    else
		echo -n $(echo ${seq}/${tim}  | bc -l) \ 
	    fi
	done
	echo
    done
)  > SYM-PD-speedup16.data


gnuplot<<EOF
set terminal pdf size 10in,3in 
set output 'SYM-PD-speedup16.pdf'

set key font ",15"
set xtics font ",15"
set ytics font ",15"
set ylabel font ",15"
set style data histogram
set style histogram cluster gap 2

set xtics rotate by -45
set style fill solid border rgb "black"
set xrange [-.5:${NB_INSTANCES}.5]
set yrange [0:18]
set ylabel 'Speedup'

plot 'SYM-PD-speedup16.data' using 2:xtic(1) title col, \
        '' using 3:xtic(1) title col, \
        '' using 4:xtic(1) title col, \
        '' using 5:xtic(1) title col, \
        '' using 6:xtic(1) title col, \
        '' using 7:xtic(1) title col, \
        '' using 8:xtic(1) title col

EOF

