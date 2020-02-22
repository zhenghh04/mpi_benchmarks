#!/bin/sh
#COBALT -n 8 -q debug-flat-quad -t 1:00:00 -A datascience --attrs mcdram=cache:numa=quad
module load oneCCL
module load intelmpi-2020

for ALG in direct ring 
do
    n=0
    aprun -n $COBALT_JOBSIZE -N 1 -e CCL_ALLREDUCE=$ALG -e CCL_WORKER_COUNT=1:CCL_WORKER_AFFINITY=2 ccl_allreduce_bench --count $n | grep regular | head -1 > n$COBALT_JOBSIZE.$ALG.w1.out
    n=1
    for i in `seq 0 26`
    do
	aprun -n $COBALT_JOBSIZE -N 1 -e CCL_ALLREDUCE=$ALG -e CCL_WORKER_COUNT=1:CCL_WORKER_AFFINITY=2 ccl_allreduce_bench --count $n | grep regular | head -1 >> n$COBALT_JOBSIZE.$ALG.w1.out
	n=$((n*2))
    done
done


for ALG in direct ring 
do
    n=0
    aprun -n $COBALT_JOBSIZE -N 1 -e CCL_ALLREDUCE=$ALG -e CCL_WORKER_COUNT=2:CCL_WORKER_AFFINITY=2,3 ccl_allreduce_bench --count $n | grep regular | head -1 > n$COBALT_JOBSIZE.$ALG.w2.out
    n=1
    for i in `seq 0 26`
    do
	aprun -n $COBALT_JOBSIZE -N 1 -e CCL_ALLREDUCE=$ALG -e CCL_WORKER_COUNT=2:CCL_WORKER_AFFINITY=2,3 ccl_allreduce_bench --count $n | grep regular | head -1 >> n$COBALT_JOBSIZE.$ALG.w2.out
	n=$((n*2))
    done
done



for ALG in direct ring 
do
    n=0
    aprun -n $COBALT_JOBSIZE -N 1 -e CCL_ALLREDUCE=$ALG -e CCL_WORKER_COUNT=4:CCL_WORKER_AFFINITY=2,3,4,5 ccl_allreduce_bench --count $n | grep regular | head -1 > n$COBALT_JOBSIZE.$ALG.w4.out
    n=1
    for i in `seq 0 26`
    do
	aprun -n $COBALT_JOBSIZE -N 1 -e CCL_ALLREDUCE=$ALG -e CCL_WORKER_COUNT=4:CCL_WORKER_AFFINITY=2,3,4,5 ccl_allreduce_bench --count $n | grep regular | head -1 >> n$COBALT_JOBSIZE.$ALG.w4.out
	n=$((n*2))
    done
done


