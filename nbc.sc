#!/bin/sh
#SBATCH -J mpi-oneccl
#SBATCH -A Aurora_SDL
#SBATCH -p knlall    
#SBATCH -N 16               
#SBATCH -C knl,quad,cache
#SBATCH --ntasks-per-node=1
#SBATCH -t 4:00:00                      
#SBATCH --mail-user huihuo.zheng@anl.gov 

export COBALT_JOBSIZE=16
module load intel-parallel-studio
export LD_LIBRARY_PATH=$HOME/opt/oneCCL/lib:$LD_LIBRARY_PATH
for ALG in direct ring rabenseifner ring_rma double_tree recursive_doubling starlike
do
    n=0
    CCL_ALLREDUCE=$ALG CCL_WORKER_COUNT=1 mpirun -np $COBALT_JOBSIZE -ppn 1 ./ccl_allreduce_bench --count $n | grep regular | head -1 > n$COBALT_JOBSIZE.$ALG.w1.out
    n=1
    for i in `seq 0 26`
    do
	CCL_ALLREDUCE=$ALG CCL_WORKER_COUNT=1 mpirun -np $COBALT_JOBSIZE -ppn 1 ./ccl_allreduce_bench --count $n | grep regular | head -1 >> n$COBALT_JOBSIZE.$ALG.w1.out
	n=$((n*2))
    done
done

for ALG in direct ring rabenseifner ring_rma double_tree recursive_doubling starlike
do
    n=0
    CCL_ALLREDUCE=$ALG CCL_WORKER_COUNT=2 mpirun -np $COBALT_JOBSIZE -ppn 1 ./ccl_allreduce_bench --count $n | grep regular | head -1 > n$COBALT_JOBSIZE.$ALG.w2.out
    n=1
    for i in `seq 0 26`
    do
	CCL_ALLREDUCE=$ALG CCL_WORKER_COUNT=2 mpirun -np $COBALT_JOBSIZE -ppn 1 ./ccl_allreduce_bench --count $n | grep regular | head -1 >> n$COBALT_JOBSIZE.$ALG.w2.out
	n=$((n*2))
    done
done


for ALG in direct ring rabenseifner ring_rma double_tree recursive_doubling starlike
do
    n=0
    CCL_ALLREDUCE=$ALG CCL_WORKER_COUNT=4 mpirun -np $COBALT_JOBSIZE -ppn 1 ./ccl_allreduce_bench --count $n | grep regular | head -1 > n$COBALT_JOBSIZE.$ALG.w4.out
    n=1
    for i in `seq 0 26`
    do
	CCL_ALLREDUCE=$ALG CCL_WORKER_COUNT=4 mpirun -np $COBALT_JOBSIZE -ppn 1 ./ccl_allreduce_bench --count $n | grep regular | head -1 >> n$COBALT_JOBSIZE.$ALG.w4.out
	n=$((n*2))
    done
done
