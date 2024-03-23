// Part 1 Script with OpenMP
#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <cmath>
#include <iomanip>
#include <omp.h> // Include OpenMP header

using namespace std;

// Generates a square matrix filled with random integers.
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

// Prints the matrix to standard output
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

int main() {
    vector<int> Ns = {1, 10, 25, 50, 80, 100, 150, 200, 300, 400, 600, 1000, 1200, 1500, 2000, 3000, 4000, 5000, 6000};
    vector<double> times;

    for (int n : Ns) {
        double timeTaken = matrix_multiplication(n);
        times.push_back(timeTaken);
        cout << "Matrix size: " << n << ", Time taken: " << timeTaken << " seconds" << endl;
    }

    ofstream outFile("times.txt");
    if (outFile.is_open()) {
        for (double time : times) {
            outFile << time << ",";
        }
        outFile.close();
    } else {
        cerr << "Unable to open file for writing." << endl;
    }

    return 0;
}

////////// Part 1 notes and answers to questions: //////////

// Part 1 Q1: In this part, we'll modify the code from part 1 to include OpenMP commands
//            and we'll use the ompiler flag 'g++ -fopenmp Part_1.cpp -o Part_1 
// Part 2 Q2: Next, we want to compare the performance of the code under OpenMP compiled 
//            with only one thread to a non-OpenMP compiled script.  First, we'll compile
//            the Part_1.cpp without the -fopenmp flag to run without OpenMP.  We'll do this
//            with: 'g++ Part_1.cpp -o Part_1_nonopenmp'
