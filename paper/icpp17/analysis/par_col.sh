#!/bin/sh

. ./common.sh

RESULTDIR=../results


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
decompspeedupdata SYM-PD-speedup16.data POINTBASED-SYMOMP-OBSDECOMP 

decomphistogram SYM-PD-speedup16.pdf SYM-PD-speedup16.data
