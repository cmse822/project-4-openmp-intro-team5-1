#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <cmath>
#include <iomanip>
#include <mpi.h>
#include <omp.h>
#include <cstdlib> 
#include <sstream> 

using namespace std;

vector<vector<int>> matrix_generator(int num) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> number(1, 10);
    vector<vector<int>> matrix(num, vector<int>(num, 0));
    for (int i = 0; i < num; ++i) {
        for (int j = 0; j < num; ++j) {
            matrix[i][j] = number(gen);
        }
    }
    return matrix;
}

void matrix_printer(const vector<vector<int>>& matrix_temp) {
    for (const vector<int>& row : matrix_temp) {
        for (int val : row) {
            cout << val << " ";
        }
        cout << endl;
    }
}

double matrix_multiplication(int num, int rank, int size) {
    double start, end;
    vector<vector<int>> matrix_one = matrix_generator(num);
    vector<vector<int>> matrix_two = matrix_generator(num);
    vector<vector<int>> result(num, vector<int>(num, 0));

    int rows_per_rank = num / size;
    int start_row = rank * rows_per_rank;
    int end_row = (rank + 1) * rows_per_rank;

    MPI_Barrier(MPI_COMM_WORLD);
    start = MPI_Wtime();

    #pragma omp parallel for collapse(2)
    for (int i = start_row; i < end_row; ++i) {
        for (int j = 0; j < num; ++j) {
            for (int k = 0; k < num; ++k) {
                result[i][j] += matrix_one[i][k] * matrix_two[k][j];
            }
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);
    end = MPI_Wtime();

    return end - start;
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    vector<int> Ns = {1, 10, 25, 50, 80, 100, 150, 200, 300, 400, 600, 1000, 1200, 1500, 2000};
    vector<double> times;

    for (int n : Ns) {
        double timeTaken = matrix_multiplication(n, rank, size);
        if (rank == 0) {
            times.push_back(timeTaken);
            cout << "Matrix size: " << n << ", Time taken: " << timeTaken << " seconds" << endl;
        }
    }

    char* ompThreadsChar = getenv("OMP_NUM_THREADS");
    int ompThreads = ompThreadsChar ? atoi(ompThreadsChar) : 1; // Default to 1 if not set
    std::stringstream ss;
    ss << "omp" << ompThreads << "_mpi" << size << ".csv";
    std::string filename = ss.str();

    if (rank == 0) {
    std::ofstream outFile(filename);
    if (outFile.is_open()) {
        outFile << "Time Taken\n"; 
        for (double time : times) {
            outFile << time << "\n"; 
        }
        outFile.close();
    } else {
        cerr << "Unable to open file for writing." << endl;
    }
    }

    MPI_Finalize();
    return 0;
}
