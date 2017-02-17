gettime() {
    inst=$1
    meth=$2

    grep \^time: ${RESULTDIR}/${inst}_${meth} | cut -d \  -f 2 | awk '{printf "%.3f", $1}'
}

#if no time, this returns zero
gettime_def() {
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

getinit() {
    inst=$1
    meth=$2

    grep \^Initialization\ time: ${RESULTDIR}/${inst}_${meth} | cut -d \  -f 3 | awk '{printf "%.4f", $1}'
}

getcompute() {
    inst=$1
    meth=$2

    grep \^compute: ${RESULTDIR}/${inst}_${meth} | cut -d \  -f 2 | awk '{printf "%.4f", $1}'
}


prettyname() {
    name=$1

    echo ${name}  | sed 's/-Animal//' \
	| sed 's/lowres/Lr/g' \
	| sed 's/medres/Mr/g' \
	| sed 's/veryhighres/VHr/g' \
	| sed 's/highres/Hr/g' \
	| sed 's/verylowbw/VLb/g' \
	| sed 's/lowbw/Lb/g' \
	| sed 's/medbw/Mb/g' \
	| sed 's/highbw/Hb/g' \
	| sed 's/veryhighbandwidth/VHb/g' \
	| sed 's/_/\\_/g'
}



cpobs() {
    instance=$1

    grep maxin\ round ../results/${instance}_POINTBASED-SYMOMP-OBSDECOMP_64_64_64_t16 | cut -d: -f 2 | awk '{sum += $1;} END{print sum}'
}

cpcolorsched() {
    instance=$1

    grep load\ based\ longest\ path ../results/${instance}_POINTBASED-SYMOMP-OBSDECOMP-COLORSCHED_64_64_64_t16 | cut -d: -f 2 | awk '{sum += $1;} END{print sum}'

}

nbpoint() {
    instance=$1

    grep read ../results/${instance}_POINTBASED-SYM | grep observations | cut -d \   -f 2 
}

spatialbw() {
    instance=$1

    FILE=../results/${instance}_POINTBASED-SYM
    grep voxBW: $FILE | cut -d \   -f 2
}

temporalbw() {
    instance=$1

    FILE=../results/${instance}_POINTBASED-SYM
    grep voxBW: $FILE | cut -d \   -f 3
}

symflop() {
    instance=$1

    coef_sp=9
    coef_time=4
    coef_vol=2

    flop_sp=$(echo 4 \* $(spatialbw $instance) \* $(spatialbw $instance) \* $coef_sp | bc -l)
    flop_time=$(echo 2 \* $(temporalbw $instance) \* $coef_time | bc -l)
    flop_vol=$(echo 8 \* $(temporalbw $instance) \* $(spatialbw $instance) \* $(spatialbw $instance) \* $coef_vol | bc -l)
    
    flop_per_pt=$(echo $flop_sp + $flop_time + $flop_vol | bc -l)

    # echo $flop_sp
    # echo $flop_time
    # echo $flop_vol
    # echo $flop_per_pt
    
    echo $(nbpoint $instance) \* $flop_per_pt | bc -l
}

gridsizex() {
    instance=$1

    FILE=../results/${instance}_POINTBASED-SYM
    grep voxsize: $FILE | cut -d \   -f 2 | cut -d x -f 1
}

gridsizey() {
    instance=$1

    FILE=../results/${instance}_POINTBASED-SYM
    grep voxsize: $FILE | cut -d \   -f 2 | cut -d x -f 2
}

gridsizet() {
    instance=$1

    FILE=../results/${instance}_POINTBASED-SYM
    grep voxsize: $FILE | cut -d \   -f 2 | cut -d x -f 3
}

gridsizeinbytes() {
    instance=$1

    bytes_per_voxel=4
    echo $(gridsizex $instance) \* $(gridsizey $instance) \* $(gridsizet $instance) \* ${bytes_per_voxel}  | bc -l
}

pointsizeinbytes() {
    instance=$1

    bytes_per_component=4
    nb_component=3
    echo ${bytes_per_component} \* ${nb_component} \* $(nbpoint $instance) | bc -l
}

RESULTDIR=../results


INSTANCES="Dengue_lowres-lowbw Dengue_lowres-highbw Dengue_highres-lowbw Dengue_highres-highbw Dengue_highres-veryhighbandwidth \
            Pollen_lowres-lowbw Pollen_highres-lowbw Pollen_highres-medbw Pollen_highres-highbw \
	    PollenUS_lowres-lowbw PollenUS_highres-lowbw PollenUS_highres-medbw PollenUS_highres-highbw PollenUS_veryhighres-lowbw PollenUS_veryhighres-verylowbw \
	    Flu-Animal_lowres-lowbw Flu-Animal_lowres-highbw Flu-Animal_medres-lowbw Flu-Animal_medres-highbw Flu-Animal_highres-lowbw Flu-Animal_highres-highbw \
            eBird_lowres-lowbw eBird_lowres-highbw eBird_highres-lowbw eBird_highres-highbw"


NB_INSTANCES=`echo $INSTANCES | wc -w`

DECOMP="1_1_1 2_2_2 4_4_4 8_8_8 16_16_16 32_32_32 64_64_64"

decompspeedupdata() {
    output=$1
    method=$2
    
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
	    meth=${method}_${decomp}_t${t}
	    
	    tim=$(gettime ${inst} ${meth})
	    if [ "$tim" = "0" -o "$tim" = "" ]
	    then
		echo -n 0 \ 
	    else
		echo -n $(echo ${seq}/${tim}  | bc -l) \ 
	    fi
	done
	echo
    done
)  > ${output}
}

decomphistogram(){
    output=$1
    datafile=$2

    gnuplot<<EOF
set terminal pdf size 10in,3in 
set output '${output}'

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

plot '${datafile}' using 2:xtic(1) title col, \
        '' using 3:xtic(1) title col, \
        '' using 4:xtic(1) title col, \
        '' using 5:xtic(1) title col, \
        '' using 6:xtic(1) title col, \
        '' using 7:xtic(1) title col, \
        '' using 8:xtic(1) title col

EOF

}
