#!/bin/sh

gettime() {
    inst=$1
    meth=$2

    grep \^time: ${RESULTDIR}/${inst}_${meth} | cut -d \  -f 2 | awk '{printf "%.3f", $1}'
}

INSTANCES="Dengue_lowres-lowbw Dengue_lowres-highbw Dengue_highres-lowbw Dengue_highres-highbw Dengue_highres-veryhighbandwidth \
            Pollen_lowres-lowbw Pollen_highres-lowbw Pollen_highres-medbw Pollen_highres-highbw \
	    Flu-Animal_lowres-lowbw Flu-Animal_lowres-highbw Flu-Animal_medres-lowbw Flu-Animal_medres-highbw Flu-Animal_highres-lowbw Flu-Animal_highres-highbw"

RESULTDIR=../results

DECOMP="1_1_1 1_1_4 1_1_8 4_4_1 4_4_4 4_4_8 8_8_1 8_8_4 8_8_8"

(
    echo -n instance \& seq \ 
    for dec in $DECOMP
    do
	echo -n \& $dec \  
    done 
    echo '\\\\'
    
    for inst in $INSTANCES
    do
	seq=$(gettime ${inst} POINTBASED-SYM)
	t=16
	
	echo -n ${inst} \& ${seq}  \ 
	
	
	for decomp in $DECOMP
	do
	    meth=POINTBASED-SYMOMP_${decomp}_t${t}
	    
	    tim=$(gettime ${inst} ${meth})
	    echo -n \& $tim \ 
	done
	echo '\\\\'
    done
) | sed 's/_/\\_/g' >  parspace-t16.tex


(
    echo -n instance \& seq \ 
    for dec in $DECOMP
    do
	echo -n \& $dec \  
    done 
    echo '\\\\'
    
    for inst in $INSTANCES
    do
	seq=$(gettime ${inst} POINTBASED-SYM)
	t=1
	
	echo -n ${inst} \& ${seq}  \ 
	
	
	for decomp in $DECOMP
	do
	    meth=POINTBASED-SYMOMP_${decomp}_t${t}
	    
	    tim=$(gettime ${inst} ${meth})
	    echo -n \& $tim \ 
	done
	echo '\\\\'
    done
) | sed 's/_/\\_/g' >  parspace-t1.tex

#exit

for inst in $INSTANCES
do
    
    (
	echo thread $DECOMP
	for t in 1 2 4 8 16
	do
	    echo -n ${t} \ 
	    for decomp in $DECOMP
	    do
		meth=POINTBASED-SYMOMP_${decomp}_t${t}
		
		tim=$(gettime ${inst} ${meth})
		echo -n  $tim \ 
	    done
	    echo
	done
    ) > parspacetime_${inst} 

    gnuplot<<EOF
set terminal pdf
set output 'parspacetime_${inst}.pdf'
set key autotitle columnhead
set style data linespoints
set yrange [0:]
set ylabel 'time (in seconds)'
set xlabel 'threads'
set title 'parallel space decomposition ${inst}'
plot 'parspacetime_${inst}' u 1:2, '' u 1:3, '' u 1:4, '' u 1:5, '' u 1:6, '' u 1:7, '' u 1:8, '' u 1:9
EOF

    (
	echo thread $DECOMP
	seq=$(gettime ${inst} POINTBASED-SYM)
	for t in 1 2 4 8 16
	do
	    echo -n ${t} \ 
	    for decomp in $DECOMP
	    do
		meth=POINTBASED-SYMOMP_${decomp}_t${t}
		
		tim=$(gettime ${inst} ${meth})
		echo -n  $(echo $seq / $tim | bc -l) \ 
	    done
	    echo
	done
    ) > parspacetime_${inst} 

    gnuplot<<EOF
set terminal pdf
set output 'parspacespeedup_${inst}.pdf'
set key top left
set key autotitle columnhead
set style data linespoints
set yrange [0:18]
set ylabel 'speedup'
set xlabel 'threads'
set title 'parallel space decomposition ${inst}'
plot 'parspacetime_${inst}' u 1:2, '' u 1:3, '' u 1:4, '' u 1:5, '' u 1:6, '' u 1:7, '' u 1:8, '' u 1:9
EOF

done

