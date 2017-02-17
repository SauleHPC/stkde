#!/bin/sh


. ./common.sh

DECOMP="1_1_1 2_2_2 4_4_4 8_8_8 16_16_16 32_32_32 64_64_64"

(
    echo -n Instance \& PB-SYM \ 
    for dec in $DECOMP
    do
	echo -n \& $dec \  
    done |tr _ x
    echo '\\\\'
    
    for inst in $INSTANCES
    do
	seq=$(gettime_def ${inst} POINTBASED-SYM)
	t=16
	
	echo -n $(prettyname ${inst}) \& ${seq}  \ 
	
	
	for decomp in $DECOMP
	do
	    meth=POINTBASED-SYMOMP-OBSDECOMP-SCHED_${decomp}_t${t}
	    
	    tim=$(gettime_def ${inst} ${meth})
	    echo -n \& $tim \ 
	done
	
	echo '\\\\'
    done
    
)  >  obssched-t16.tex

# bar chart at t=16
(
    echo -n Instance \ 
    for dec in $DECOMP
    do
	echo -n ${dec} \  
    done |tr _ x
    echo
    
    for inst in $INSTANCES
    do
	seq=$(gettime_def ${inst} POINTBASED-SYM)
	t=16
	
	echo -n $(prettyname ${inst}) \  

	for decomp in $DECOMP
	do
	    meth=POINTBASED-SYMOMP-OBSDECOMP-SCHED_${decomp}_t${t}
	    
	    tim=$(gettime_def ${inst} ${meth})
	    if [ "$tim" = "0" ]
	    then
		echo -n 0 \ 
	    else
		echo -n $(echo ${seq}/${tim}  | bc -l) \ 
	    fi
	done
	echo
    done
) > SYM-PD-COLOR-SCHED-speedup16.data


decomphistogram SYM-PD-COLOR-SCHED-speedup16.pdf SYM-PD-COLOR-SCHED-speedup16.data

