#!/bin/sh

gettime() {
    inst=$1
    meth=$2

    basetime=$(grep \^time: ${RESULTDIR}/${inst}_${meth} | cut -d \  -f 2)
    if [ "$basetime" = "" ]
    then
	echo 0
    else
	echo $basetime | awk '{printf "%.3f", $1}'
    fi
}


INSTANCES="Dengue_lowres-lowbw Dengue_lowres-highbw Dengue_highres-lowbw Dengue_highres-highbw Dengue_highres-veryhighbandwidth \
            Pollen_lowres-lowbw Pollen_highres-lowbw Pollen_highres-medbw Pollen_highres-highbw \
	    Flu-Animal_lowres-lowbw Flu-Animal_lowres-highbw Flu-Animal_medres-lowbw Flu-Animal_medres-highbw Flu-Animal_highres-lowbw Flu-Animal_highres-highbw"

RESULTDIR=../results

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
	seq=$(gettime ${inst} POINTBASED-SYM)
	t=16
	
	echo -n ${inst} \& ${seq}  \ 
	
	
	for decomp in $DECOMP
	do
	    meth=POINTBASED-SYMOMP-OBSDECOMP-SCHED_${decomp}_t${t}
	    
	    tim=$(gettime ${inst} ${meth})
	    echo -n \& $tim \ 
	done
	
	echo '\\\\'
    done| sed 's/-Animal//' \
	| sed 's/lowres/Lr/g' \
	| sed 's/medres/Mr/g' \
	| sed 's/highres/Hr/g' \
	| sed 's/lowbw/Lb/g' \
	| sed 's/medbw/Mb/g' \
	| sed 's/highbw/Hb/g' \
	| sed 's/veryhighbandwidth/VHb/g' \
	| sed 's/_/\\_/g'
    
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
	seq=$(gettime ${inst} POINTBASED-SYM)
	t=16
	
	echo -n ${inst} \  

	for decomp in $DECOMP
	do
	    meth=POINTBASED-SYMOMP-OBSDECOMP-SCHED_${decomp}_t${t}
	    
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
) | sed 's/-Animal//' \
    | sed 's/lowres/Lr/g' \
    | sed 's/medres/Mr/g' \
    | sed 's/highres/Hr/g' \
    | sed 's/lowbw/Lb/g' \
    | sed 's/medbw/Mb/g' \
    | sed 's/highbw/Hb/g' \
    | sed 's/veryhighbandwidth/VHb/g' \
    | sed 's/_/\\_/g' > SYM-PD-COLOR-SCHED-speedup16.data


gnuplot<<EOF
set terminal pdf size 10in,3in 
set output 'SYM-PD-COLOR-SCHED-speedup16.pdf'

set key font ",15"
set xtics font ",15"
set ytics font ",15"
set ylabel font ",15"
set style data histogram
set style histogram cluster gap 2

set xtics rotate by -45
set style fill solid border rgb "black"
set xrange [-.5:14.5]
set yrange [0:18]
set ylabel 'Speedup'

plot 'SYM-PD-COLOR-SCHED-speedup16.data' using 2:xtic(1) title col, \
        '' using 3:xtic(1) title col, \
        '' using 4:xtic(1) title col, \
        '' using 5:xtic(1) title col, \
        '' using 6:xtic(1) title col, \
        '' using 7:xtic(1) title col, \
        '' using 8:xtic(1) title col

EOF


