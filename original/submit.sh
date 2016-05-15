#! /bin/bash
# ===== PBS OPTIONS =====
# Run in the queue named "viper"
#PBS -q viper 
#PBS -l nodes=1:ppn=1
#PBS -W x=nmatchpolicy:exactnode
# ===== END PBS OPTIONS =====
# Redirect standard out and standard error.
exec 1>$PBS_O_WORKDIR/Messages/$PBS_JOBID.out 2>$PBS_O_WORKDIR/Messages/$PBS_JOBID.err
# Main Program

cat $PBS_NODEFILE > /users/ahohl/domain_decomposition_dengue2010/CPU1/nodefile_20150331.txt
cd /users/ahohl/domain_decomposition_dengue2010/CPU1

j=1
for l in `cat nodefile_20150331.txt`

	do
	ssh $l "/users/ahohl/domain_decomposition_dengue2010/CPU1/run.sh $j" &
	((j++))
done
wait