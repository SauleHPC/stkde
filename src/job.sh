#!/bin/sh
# ===== PBS OPTIONS =====
### Set the job name
#PBS -N "CUDA-test"

### Specify queue to run in
#PBS -q copperhead

### Specify number of CPUs for job
#PBS -l nodes=1:ppn=1:gpus=1


# ==== Main ======
### make /users/tefaz/stkde/src
cd stkde/src
./point-based ../dataset/Dengue-allcases-2010-2011/AllCases2010_11_clip_bds.txt  ../dataset/Dengue-allcases-2010-2011/AllCases2010_11_clip.txt ../dataset/Dengue-allcases-2010-2011/lowres-lowbw VOXELBASED-GPU
