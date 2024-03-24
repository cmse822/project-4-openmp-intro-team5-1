#!/bin/sh
export OMP_NUM_THREADS=2
mpiexec -n 8 ./p3q1 
mpiexec -n 16 ./p3q1 
export OMP_NUM_THREADS=4
mpiexec -n 2 ./p3q1 
mpiexec -n 4 ./p3q1 
mpiexec -n 8 ./p3q1 
mpiexec -n 16 ./p3q1 
export OMP_NUM_THREADS=8
mpiexec -n 2 ./p3q1 
mpiexec -n 4 ./p3q1 
mpiexec -n 8 ./p3q1 
mpiexec -n 16 ./p3q1 
