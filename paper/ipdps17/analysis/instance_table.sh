#!/bin/sh


. ./common.sh


(
    echo '\\begin{tabular}{|l|rrr|rr|}'
    echo \\hline
    echo Instance \& n \& \$G_x\$ x \$G_y\$ x \$G_t\$ \& Size \& \$H_s\$ \& \$H_t\$  '\\\\'
    echo \\hline
    for inst in $INSTANCES
    do
	FILE=${RESULTDIR}/${inst}_POINTBASED
	echo $(prettyname ${inst} ) \& \
	     $(grep read $FILE | grep observations | cut -d \   -f 2) \& \
	     $(grep voxsize: $FILE | cut -d \  -f 2) \& \
	     $(grep voxsize: $FILE | cut -d : -f 3 | awk '{printf "%dMB", $1}') \& \
	     $(grep voxBW: $FILE | cut -d \   -f 2) \& \
	     $(grep voxBW: $FILE | cut -d \   -f 3)  '\\\\'
    done
    echo \\hline
    echo '\\end{tabular}'
) > instance_table.tex
