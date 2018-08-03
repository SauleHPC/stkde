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
echo $PARTPRE

#DECOMP="64 32 16 8 4 2 1"
#THREAD="16 8 4 2 1"
#DECOMP="64 32 16 8 4 2 1"
THREAD=16


method=POINTBASED-SYMOMP-BOXDECOMP

PARTDIR=parts


# POINTBASED POINTBASED-SYMDISK POINTBASED-SYMBAR VOXELBASED-OMP-OBSDECOMP VOXELBASED-OMP 

for part in $(ls ${PARTDIR}/${PARTPRE}*);
do
    partbase=$(basename $part | sed "s/${PARTPRE}//g")
#    echo $partbase
    FILE=results/${PARTPRE}_${method}_${partbase}_t${THREAD}-OUT
    FILEERR=results/${PARTPRE}_${method}_${partbase}_t${THREAD}-ERR
    if [ ! -f $FILE ]
    then
	echo OMP_NUM_THREADS=${THREAD}  $BIN $BOUNDARY $OBSERVATIONS $BANDWIDTH $method $part
	echo OMP_NUM_THREADS=${THREAD}  $BIN $BOUNDARY $OBSERVATIONS $BANDWIDTH $method $part 1>&2
	OMP_NUM_THREADS=${THREAD}  $BIN $BOUNDARY $OBSERVATIONS $BANDWIDTH $method $part > $FILE 2>${FILEERR}
    fi
done
