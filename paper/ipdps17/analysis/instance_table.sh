#!/bin/sh

INSTANCES="Dengue_lowres-lowbw Dengue_lowres-highbw Dengue_highres-lowbw Dengue_highres-highbw Dengue_highres-veryhighbandwidth \
            Pollen_lowres-lowbw Pollen_highres-lowbw Pollen_highres-medbw Pollen_highres-highbw \
	    Flu-Animal_lowres-lowbw Flu-Animal_lowres-highbw Flu-Animal_medres-lowbw Flu-Animal_medres-highbw Flu-Animal_highres-lowbw Flu-Animal_highres-highbw"

RESULTDIR=../results

(
    echo instance \& N \& Grid \& Size \& SBW \& TBW  '\\\\'
    for inst in $INSTANCES
    do
	FILE=${RESULTDIR}/${inst}_POINTBASED
	echo $inst \& \
	     $(grep read $FILE | grep observations | cut -d \   -f 2) \& \
	     $(grep voxsize: $FILE | cut -d \  -f 2) \& \
	     $(grep voxsize: $FILE | cut -d : -f 3) \& \
	     $(grep voxBW: $FILE | cut -d \   -f 2) \& \
	     $(grep voxBW: $FILE | cut -d \   -f 3)  '\\\\'
    done
) | sed 's/_/\\_/g' > instance_table.tex
