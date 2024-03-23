// Part 1 Q 3 Script -- adjusting so that is accepts matrix size as command line arg
#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <cmath>
#include <iomanip>
#include <cstdlib> 
#include <omp.h>

using namespace std;

// Generates a square matrix filled with random integers
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

// Prints the matrix to the standard output.
void matrix_printer(const vector<vector<int>>& matrix_temp) {
    for (const vector<int>& row : matrix_temp) {
        for (int val : row) {
            cout << val << " ";
        }
        cout << endl;
    }
}

double matrix_multiplication(int num) {
    double start, end;
    vector<vector<int>> result(num, vector<int>(num, 0));

    vector<vector<int>> matrix_one = matrix_generator(num);
    vector<vector<int>> matrix_two = matrix_generator(num);

    start = omp_get_wtime();

    // Parallelize the outer two loops - here is where we insert Openmp threading
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < num; ++i) {
        for (int j = 0; j < num; ++j) {
            for (int k = 0; k < num; ++k) {
                result[i][j] += matrix_one[i][k] * matrix_two[k][j];
            }
        }
    }

    end = omp_get_wtime();

    return end - start;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <matrix size>" << std::endl;
        return 1;
    }
    int N = std::atoi(argv[1]);

    double timeTaken = matrix_multiplication(N);
    cout << "Matrix size: " << N << ", Time taken: " << timeTaken << " seconds" << endl;

    return 0;
}

////////// Here's what we're doing: //////////
// We're going to run the matrix-matrix mult across the different sizes (n) and then do this
// across different thread counts -- from 1, 2 ... up to T where T is the max I have
// on my local machine.        
// To compile: 'g++ -fopenmp -o Part_1_Q_3 Part_1_Q_3.cpp'
// to run with matrix size arg in command line: './Part_1_Q_3 1000' -- for 1000 size matrix
// The point -- we'll run './Part_1_Q_3 1000' but change n to be 20, 100, 1000 and do this for
// across the different thread counts -- how many do i have? 
// First! I'll need to specify the thread count with : 'export OMP_NUM_THREADS=4', for example.
// Entirely: 

//export OMP_NUM_THREADS=1
//./Part_1_Q_2_withopenmp 20
//./Part_1_Q_2_withopenmp 100
//./Part_1_Q_2_withopenmp 1000

//OMP_NUM_THREADS=2
//./Part_1_Q_2_withopenmp 20
//./Part_1_Q_2_withopenmp 100
//./Part_1_Q_2_withopenmp 1000

// etc.
