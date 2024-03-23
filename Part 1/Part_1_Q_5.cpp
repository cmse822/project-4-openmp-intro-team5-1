// Part 1 Q 5 -- this is a script that verifies the answers to MMM remain accurate across the 
// different parallelization schemes -- it's going to find error using the Frobenius norm
// of the matrices compared to the first matrix using just one thread
#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <cmath>
#include <iomanip>
#include <cstdlib> 
#include <omp.h> 

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

vector<vector<int>> matrix_multiplication(int num, const vector<vector<int>>& matrix_one, const vector<vector<int>>& matrix_two) {
    vector<vector<int>> result(num, vector<int>(num, 0));
    double start, end;

    start = omp_get_wtime();
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < num; ++i) {
        for (int j = 0; j < num; ++j) {
            for (int k = 0; k < num; ++k) {
                result[i][j] += matrix_one[i][k] * matrix_two[k][j];
            }
        }
    }
    end = omp_get_wtime();

    cout << "Computation time: " << (end - start) << " seconds" << endl;
    return result;
}

double frobeniusNorm(const vector<vector<int>>& A, const vector<vector<int>>& B) {
    double sum = 0.0;
    for (size_t i = 0; i < A.size(); ++i) {
        for (size_t j = 0; j < A[i].size(); ++j) {
            double diff = A[i][j] - B[i][j];
            sum += diff * diff;
        }
    }
    return sqrt(sum);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <matrix size>" << endl;
        return 1;
    }
    int N = atoi(argv[1]);

    // Generate the matrices only once to ensure consistency across runs.
    // This is done so that we have the same matrix to compare it to
    auto matrix_one = matrix_generator(N);
    auto matrix_two = matrix_generator(N);

    // Perform multiplication with 1 thread to get the baseline result
    omp_set_num_threads(1);
    auto baseline_result = matrix_multiplication(N, matrix_one, matrix_two);

    // Compare results for different numbers of threads
    for (int threads = 2; threads <= 8; threads *= 2) { 
        cout << "Using " << threads << " threads:" << endl;
        omp_set_num_threads(threads);
        auto result = matrix_multiplication(N, matrix_one, matrix_two);

        // Compare result with baseline using Frobenius norm to measure error
        double difference = frobeniusNorm(baseline_result, result);
        cout << "Difference (Frobenius norm): " << difference << endl;

        if (difference > 1e-6) { // This will gen a warning if differences are large
            cout << "Notable differences!" << endl;
        }
    }

    return 0;
}

// to compile: 'g++ -fopenmp -o Part_1_Q_5 Part_1_Q_5.cpp'
// to runm: ./Part_1_Q_5
