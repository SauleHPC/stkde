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

#DECOMP="64 32 16 8 4 2 1"
#THREAD="16 8 4 2 1"
DECOMP="64 32 16 8 4 2 1"
THREAD="16 1"

# POINTBASED POINTBASED-SYMDISK POINTBASED-SYMBAR VOXELBASED-OMP-OBSDECOMP VOXELBASED-OMP 

for method in POINTBASED-SYM POINTBASED-SYMOMP-OBSDECOMP-SCHED  POINTBASED-SYMOMP-OBSDECOMP POINTBASED-SYMOMP POINTBASED-SYMOMP-OBSDECOMP-COLORSCHED POINTBASED-SYMOMP-OBSDECOMP-COLORSCHED-REP POINTBASED-SYMOMP-POINTDECOMP 
do
    if [ $method = POINTBASED-SYMOMP ]
    then
	for t in $THREAD
	do
	    for decomp in $DECOMP
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
	    for t in $THREAD
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
		for t in $THREAD
		do
		    for decomp in $DECOMP
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
		    for t in $THREAD
		    do
			for decomp in $DECOMP
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
			for t in $THREAD
			do
			    for decomp in $DECOMP
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
			    for t in $THREAD
			    do
				for decomp in $DECOMP
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
				for t in $THREAD
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
				    
				    for t in $THREAD
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
