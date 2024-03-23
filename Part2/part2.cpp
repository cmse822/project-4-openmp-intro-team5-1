#include <iostream>
#include <sstream>
#include "mpi.h"
#include "omp.h" 
using namespace std;

// int main(int argc, char *argv[]) {
//     int numtasks;
//     int rank;
//     cout << "before init" << endl;
//     MPI_Init(&argc, &argv);
//     MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
//     MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//     cout << "hello world! " << "from process: " << rank << endl;

//     MPI_Finalize();
//     cout << "after finalize, " << "rank: " << rank << endl;
//     return 0;
// }

// to run the old program 
// salloc -n 4 --time 00:30:00
// mpic++ task3_hello.cpp -o task3_hello
// mpiexec -n 4 ./task3_hello > task3_hello.output

int main(int argc, char *argv[]) {
    int numtasks, rank, provided;

    MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &provided);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    stringstream ss;

    #pragma omp parallel
    {
        int thread_id = omp_get_thread_num();
        #pragma omp critical
        {
            ss << "Hello world! from process: " << rank << ", thread: " << thread_id << endl;
        }
    }

    cout << ss.str();

    MPI_Finalize();

    if (rank == 0) {
        cout << "After finalize, from rank: " << rank << endl;
    }

    return 0;
}

// mpic++ -fopenmp part2.cpp -o part2
// export OMP_NUM_THREADS=4
// mpiexec -n 2 ./part2

// Hello world! from process: 1, thread: 0
// Hello world! from process: 1, thread: 2
// Hello world! from process: 1, thread: 1
// Hello world! from process: 1, thread: 3
// Hello world! from process: 0, thread: 1
// Hello world! from process: 0, thread: 2
// Hello world! from process: 0, thread: 0
// Hello world! from process: 0, thread: 3
// After finalize, from rank: 0