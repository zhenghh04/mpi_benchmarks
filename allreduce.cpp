/*
  This for allreduce benchmarks
  Huihuo Zheng @ ALCF
 */
#include <iostream>
#include <stdio.h>
#include "mpi.h"

using namespace std; 

int bench_allreduce(int nint, int niter, int verbose=0, bool nbc=true) {
  int *a = new int [nint]; 
  int *a_reduce = new int [nint];
  double *t = new double [niter];
  double tmin = 10000000000; 
  double tmax = 0.0; 
  double tavg = 0.0; 

  MPI_Request request; 
  if (nbc) {
    int ierr = MPI_Iallreduce(a, a_reduce, nint, MPI_INT, MPI_SUM, MPI_COMM_WORLD, &request); 
    MPI_Wait(&request, MPI_STATUS_IGNORE);
    for(int i=0; i<niter; i++) {
      MPI_Barrier(MPI_COMM_WORLD);
      double t1 = MPI_Wtime();
      int ierr = MPI_Iallreduce(a, a_reduce, nint, MPI_INT, MPI_SUM, MPI_COMM_WORLD, &request); 
      MPI_Wait(&request, MPI_STATUS_IGNORE);
      double t2 = MPI_Wtime();
      t[i] = t2 - t1; 
      if (tmin > t[i]) {
	tmin = t[i]; 
      }
      if (tmax < t[i]) tmax = t[i];
      tavg += t[i];
    }
  } else {
    int ierr = MPI_Allreduce(a, a_reduce, nint, MPI_INT, MPI_SUM, MPI_COMM_WORLD); 
    for(int i=0; i<niter; i++) {
      MPI_Barrier(MPI_COMM_WORLD);
      double t1 = MPI_Wtime();
      int ierr = MPI_Allreduce(a, a_reduce, nint, MPI_INT, MPI_SUM, MPI_COMM_WORLD); 
      double t2 = MPI_Wtime();
      t[i] = t2 - t1; 
      if (tmin > t[i]) {
	tmin = t[i]; 
      }
      if (tmax < t[i]) tmax = t[i];
      tavg += t[i];
    }
  }
  tavg = tavg/niter; 
  if (verbose==1) {
    printf("%10d  %8d    %4.4f    %8.4f     %8.4f # nbc: %d \n", nint*4, niter, tmin*1000000, tmax*1000000, tavg*1000000, nbc); 
  }
  delete [] a, a_reduce, t; 
  return 0; 
}


int main(int argc, char **argv) {
  int ierr; 
  ierr = MPI_Init(&argc, &argv); 

  int sum=0; 
  int nproc, mype; 
  ierr = MPI_Comm_size(MPI_COMM_WORLD, &nproc); 
  ierr = MPI_Comm_rank(MPI_COMM_WORLD, &mype);
  int verbose = 0; 
  if (mype == 0) verbose = 1; 
  bench_allreduce(0, 50, verbose);
  bench_allreduce(0, 50, verbose, false);
  int nint=1;
  for (int i=0; i< 28; i++) {
    bench_allreduce(nint, 50, verbose);
    bench_allreduce(nint, 50, verbose, false);
    nint = nint *2; 
  }
  ierr = MPI_Finalize();
  return ierr; 
}
