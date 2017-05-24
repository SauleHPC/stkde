#!/bin/sh


. ./common.sh

(
    echo '\\begin{tabular}{|l|rr|rrrr|r|}'
    echo \\hline
    echo          \& \\multicolumn{6}{c\|}{Time \(in seconds\)}        \& speedup'\\\\'
    echo Instance \& VB \& VB-DEC \& PB \&  PB-DISK \& PB-BAR \& PB-SYM \& PB-SYM '\\\\'
    echo \\hline
	    
    for inst in $INSTANCES
    do
	echo $(prettyname ${inst}) \\t\& \
	     $(gettime ${inst} VOXELBASED-OMP_t1) \\t\& $(gettime ${inst} VOXELBASED-OMP-OBSDECOMP_1024_1024_1024_t1) \\t\& \
	     $(gettime ${inst} POINTBASED) \\t\& $(gettime ${inst} POINTBASED-SYMDISK) \\t\& \
	     $(gettime ${inst} POINTBASED-SYMBAR) \\t\& $(gettime ${inst} POINTBASED-SYM) \\t\& \
	     $(echo $(gettime ${inst} POINTBASED) / $(gettime ${inst} POINTBASED-SYM) | bc -l | awk '{printf "%.3f", $1}' ) '\\\\' 
    done

    echo \\hline
    echo '\\end{tabular}'
) > seq_table.tex
