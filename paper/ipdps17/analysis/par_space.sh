#!/bin/sh

gettime() {
    inst=$1
    meth=$2

    grep \^time: ${RESULTDIR}/${inst}_${meth} | cut -d \  -f 2 | awk '{printf "%.3f", $1}'
}

INSTANCES="Dengue_lowres-lowbw Dengue_lowres-highbw Dengue_highres-lowbw Dengue_highres-highbw Dengue_highres-veryhighbandwidth \
            Pollen_lowres-lowbw Pollen_highres-lowbw Pollen_highres-medbw Pollen_highres-highbw \
	    PollenUS_lowres-lowbw PollenUS_highres-lowbw PollenUS_highres-medbw PollenUS_highres-highbw PollenUS_veryhighres-lowbw PollenUS_veryhighres-verylowbw \
	    Flu-Animal_lowres-lowbw Flu-Animal_lowres-highbw Flu-Animal_medres-lowbw Flu-Animal_medres-highbw Flu-Animal_highres-lowbw Flu-Animal_highres-highbw \
            eBird_lowres-lowbw"

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
	    meth=POINTBASED-SYMOMP_${decomp}_t${t}
	    
	    tim=$(gettime ${inst} ${meth})
	    echo -n \& $tim \ 
	done
	echo '\\\\'
    done | sed 's/-Animal//' \
	| sed 's/lowres/Lr/g' \
	| sed 's/medres/Mr/g' \
	| sed 's/highres/Hr/g' \
	| sed 's/lowbw/Lb/g' \
	| sed 's/medbw/Mb/g' \
	| sed 's/highbw/Hb/g' \
	| sed 's/veryhighbandwidth/VHb/g' \
	| sed 's/_/\\_/g'
) >  parspace-t16.tex


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
	t=1
	
	echo -n ${inst} \& ${seq}  \ 
	
	
	for decomp in $DECOMP
	do
	    meth=POINTBASED-SYMOMP_${decomp}_t${t}
	    
	    tim=$(gettime ${inst} ${meth})
	    echo -n \& ${tim} \ 
	done
	
	echo '\\\\'
	
    done | sed 's/-Animal//' \
	| sed 's/lowres/Lr/g' \
	| sed 's/medres/Mr/g' \
	| sed 's/highres/Hr/g' \
	| sed 's/lowbw/Lb/g' \
	| sed 's/medbw/Mb/g' \
	| sed 's/highbw/Hb/g' \
	| sed 's/veryhighbandwidth/VHb/g' \
	| sed 's/_/\\_/g'
    
)  >  parspace-t1.tex

#clustered bar chart for 1 thread.

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
	t=1
	
	echo -n ${inst} \  

	for decomp in $DECOMP
	do
	    meth=POINTBASED-SYMOMP_${decomp}_t${t}
	    
	    tim=$(gettime ${inst} ${meth})
	    echo -n $(echo ${tim} / ${seq} | bc -l) \ 
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
    | sed 's/_/\\_/g' > SYM-DD-overhead.data


gnuplot<<EOF
set terminal pdf size 10in,3in 
set output 'SYM-DD-overhead.pdf'

set key font ",15"
set xtics font ",15"
set ytics font ",15"
set ylabel font ",15"
set style data histogram
set style histogram cluster gap 2

set xtics rotate by -45
set style fill solid border rgb "black"
set xrange [-.5:21.5]
set yrange [0:15]
set ylabel 'Time relative to PB-SYM'

plot 'SYM-DD-overhead.data' using 2:xtic(1) title col, \
        '' using 3:xtic(1) title col, \
        '' using 4:xtic(1) title col, \
        '' using 5:xtic(1) title col, \
        '' using 6:xtic(1) title col, \
        '' using 7:xtic(1) title col, \
        '' using 8:xtic(1) title col

EOF


#clustered bar chart for 16 threads.

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
	    meth=POINTBASED-SYMOMP_${decomp}_t${t}
	    
	    tim=$(gettime ${inst} ${meth})
	    echo -n $(echo ${seq}/${tim}  | bc -l) \ 
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
    | sed 's/_/\\_/g' > SYM-DD-speedup16.data


gnuplot<<EOF
set terminal pdf size 10in,3in 
set output 'SYM-DD-speedup16.pdf'

set key font ",15"
set xtics font ",15"
set ytics font ",15"
set ylabel font ",15"
set style data histogram
set style histogram cluster gap 2

set xtics rotate by -45
set style fill solid border rgb "black"
set xrange [-.5:21.5]
set yrange [0:*]
set ylabel 'Speedup'

plot 'SYM-DD-speedup16.data' using 2:xtic(1) title col, \
        '' using 3:xtic(1) title col, \
        '' using 4:xtic(1) title col, \
        '' using 5:xtic(1) title col, \
        '' using 6:xtic(1) title col, \
        '' using 7:xtic(1) title col, \
        '' using 8:xtic(1) title col

EOF


exit

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

