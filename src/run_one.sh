#!/bin/sh

if [ ! -e results ]
then
    mkdir results
fi

hostname

echo

cat /proc/cpuinfo

echo

sleep 10

echo

uptime

echo


BIN=./point-based
echo $INSTANCE
echo $BOUNDARY
echo $OBSERVATIONS
echo $BANDWIDTH

for method in POINTBASED POINTBASED-SYMDISK POINTBASED-SYMBAR POINTBASED-SYM POINTBASED-SYMOMP POINTBASED-SYMOMP-POINTDECOMP POINTBASED-SYMOMP-OBSDECOMP POINTBASED-SYMOMP-OBSDECOMP-SCHED POINTBASED-SYMOMP-OBSDECOMP-COLORSCHED-REP POINTBASED-SYMOMP-OBSDECOMP-COLORSCHED VOXELBASED-OMP-OBSDECOMP VOXELBASED-OMP 
do
    if [ $method = POINTBASED-SYMOMP ]
    then
	for t in 16 8 4 2 1
	do
	    for decomp in 64 32 16 8 4 2 1
	    do
		FILE=results/${INSTANCE}_$(basename $BANDWIDTH)_${method}_${decomp}_${decomp}_${decomp}_t${t}
		if [ ! -f $FILE ]
		then
		    echo OMP_NUM_THREADS=$t  $BIN $BOUNDARY $OBSERVATIONS $BANDWIDTH $method $decomp $decomp $decomp
		    OMP_NUM_THREADS=$t  $BIN $BOUNDARY $OBSERVATIONS $BANDWIDTH $method $decomp $decomp $decomp > $FILE 2>&1
		fi
	    done
	done
    else
	if [ $method = POINTBASED-SYMOMP-POINTDECOMP ]
	then
	    for t in 16 8 4 2 1
	    do
		FILE=results/${INSTANCE}_$(basename $BANDWIDTH)_${method}_t${t}
		if [ ! -f $FILE ]
		then
		  echo OMP_NUM_THREADS=$t  $BIN $BOUNDARY $OBSERVATIONS $BANDWIDTH $method 
		  OMP_NUM_THREADS=$t  $BIN $BOUNDARY $OBSERVATIONS $BANDWIDTH $method > $FILE 2>&1
		fi
	    done
	else
	    if [ $method = POINTBASED-SYMOMP-OBSDECOMP ]
	    then
		for t in 16 8 4 2 1
		do
		    for decomp in 64 32 16 8 4 2 1
		    do
			FILE=results/${INSTANCE}_$(basename $BANDWIDTH)_${method}_${decomp}_${decomp}_${decomp}_t${t}
			if [ ! -f $FILE ]
                        then
			    echo OMP_NUM_THREADS=$t  $BIN $BOUNDARY $OBSERVATIONS $BANDWIDTH $method $decomp $decomp $decomp
			    OMP_NUM_THREADS=$t  $BIN $BOUNDARY $OBSERVATIONS $BANDWIDTH $method $decomp $decomp $decomp > $FILE 2>&1
			fi
		    done
		done
	    else
		if [ $method = POINTBASED-SYMOMP-OBSDECOMP-SCHED ]
		then
		    for t in 16 8 4 2 1
		    do
			for decomp in 64 32 16 8 4 2 1
			do
			    FILE=results/${INSTANCE}_$(basename $BANDWIDTH)_${method}_${decomp}_${decomp}_${decomp}_t${t}
			    if [ ! -f $FILE ]
                            then
				echo OMP_NUM_THREADS=$t  $BIN $BOUNDARY $OBSERVATIONS $BANDWIDTH $method $decomp $decomp $decomp
				OMP_NUM_THREADS=$t  $BIN $BOUNDARY $OBSERVATIONS $BANDWIDTH $method $decomp $decomp $decomp > $FILE 2>&1
			    fi
			done
		    done
		else
		    if [ $method = POINTBASED-SYMOMP-OBSDECOMP-COLORSCHED ]
		    then
			for t in 16 8 4 2 1
			do
			    for decomp in 64 32 16 8 4 2 1
			    do
				FILE=results/${INSTANCE}_$(basename $BANDWIDTH)_${method}_${decomp}_${decomp}_${decomp}_t${t}
				if [ ! -f $FILE ]
				then
				    echo OMP_NUM_THREADS=$t  $BIN $BOUNDARY $OBSERVATIONS $BANDWIDTH $method $decomp $decomp $decomp
				    OMP_NUM_THREADS=$t  $BIN $BOUNDARY $OBSERVATIONS $BANDWIDTH $method $decomp $decomp $decomp > $FILE 2>&1
				fi
			    done
			done
		    else
			if [ $method = POINTBASED-SYMOMP-OBSDECOMP-COLORSCHED-REP ]
			then
			    for t in 16 8 4 2 1
			    do
				for decomp in 64 32 16 8 4 2 1
				do
				    FILE=results/${INSTANCE}_$(basename $BANDWIDTH)_${method}_${decomp}_${decomp}_${decomp}_t${t}
				    if [ ! -f $FILE ]
				    then
					echo OMP_NUM_THREADS=$t  $BIN $BOUNDARY $OBSERVATIONS $BANDWIDTH $method $decomp $decomp $decomp
					OMP_NUM_THREADS=$t  $BIN $BOUNDARY $OBSERVATIONS $BANDWIDTH $method $decomp $decomp $decomp > $FILE 2>&1
				    fi
				done
			    done
			else
			    if [ $method = VOXELBASED-OMP ]
			    then
				for t in 16 8 4 2 1
				do
				    FILE=results/${INSTANCE}_$(basename $BANDWIDTH)_${method}_t${t}
				    if [ ! -f $FILE ]
                                    then
					echo OMP_NUM_THREADS=$t  $BIN $BOUNDARY $OBSERVATIONS $BANDWIDTH $method 
					OMP_NUM_THREADS=$t  $BIN $BOUNDARY $OBSERVATIONS $BANDWIDTH $method > $FILE 2>&1
				    fi
				done
			    else
				if [ $method = VOXELBASED-OMP-OBSDECOMP ]
				then
				    
				    for t in 16 8 4 2 1
				    do
					for decomp in 1024 64 
					do
					    FILE=results/${INSTANCE}_$(basename $BANDWIDTH)_${method}_${decomp}_${decomp}_${decomp}_t${t}
					    if [ ! -f $FILE ]
					    then
						echo OMP_NUM_THREADS=$t  $BIN $BOUNDARY $OBSERVATIONS $BANDWIDTH $method $decomp $decomp $decomp
						OMP_NUM_THREADS=$t  $BIN $BOUNDARY $OBSERVATIONS $BANDWIDTH $method $decomp $decomp $decomp > $FILE 2>&1
					    fi
					done
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
			fi
		    fi
		fi
	    fi
	fi
    fi
done
