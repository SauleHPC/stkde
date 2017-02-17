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

decomphistogram SYM-PD-speedup16.pdf SYM-PD-speedup16.data
