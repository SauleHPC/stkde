#!/bin/sh

if [ ! -e results ]
then
    mkdir results
fi

BIN=./point-based
echo $INSTANCE
echo $BOUNDARY
echo $OBSERVATIONS
echo $BANDWIDTH

for method in POINTBASED POINTBASED-SYMDISK POINTBASED-SYMBAR POINTBASED-SYM POINTBASED-SYMOMP POINTBASED-SYMOMP-POINTDECOMP
do
    if [ $method = POINTBASED-SYMOMP ]
    then
	for t in 1 2 4 8 16
	do
	    for decompx in 1 4 8 
	    do
		for decompy in 1 4 8 
		do	
		    for decompt in 1 4 8
		    do
			FILE=results/${INSTANCE}_$(basename $BANDWIDTH)_${method}_${decompx}_${decompy}_${decompt}_t${t}
			if [ ! -f $FILE ]
			then
			    echo OMP_NUM_THREADS=$t  $BIN $BOUNDARY $OBSERVATIONS $BANDWIDTH $method $decompx $decompy $decompt
			    OMP_NUM_THREADS=$t  $BIN $BOUNDARY $OBSERVATIONS $BANDWIDTH $method $decompx $decompy $decompt > $FILE 2>&1
			fi
		    done
		done
	    done
	done
    else

	if [ $method = POINTBASED-SYMOMP-POINTDECOMP ]
	then
	    for t in 1 2 4 8 16
	    do
		FILE=results/${INSTANCE}_$(basename $BANDWIDTH)_${method}_t${t}
		if [ ! -f $FILE ]
		then
		  echo OMP_NUM_THREADS=$t  $BIN $BOUNDARY $OBSERVATIONS $BANDWIDTH $method 
		  OMP_NUM_THREADS=$t  $BIN $BOUNDARY $OBSERVATIONS $BANDWIDTH $method > $FILE 2>&1
		fi
	    done
	else
	    FILE=results/${INSTANCE}_$(basename $BANDWIDTH)_${method}
	    if [ ! -f $FILE ]
	    then
		echo $BIN $BOUNDARY $OBSERVATIONS $BANDWIDTH $method
		$BIN $BOUNDARY $OBSERVATIONS $BANDWIDTH $method > $FILE 2>&1 
	    fi
	fi
    fi
done
