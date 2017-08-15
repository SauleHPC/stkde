#!/bin/sh
# ===== PBS OPTIONS =====
### Set the job name
#PBS -N "CPU-Pollen-Hr-Mb"

### Specify queue to run in
#PBS -q copperhead

### Specify number of CPUs for job
#PBS -l nodes=1:ppn=1:gpus=1


# ==== Main ======
### make /users/tefaz/stkde/src
cd stkde/src
./point-based ../dataset/Pollen/boundary-UScropped  ../dataset/Pollen/Pollen-xyt.txt ../dataset/Pollen/highres-medbw VOXELBASED compare compare compare compare compare
