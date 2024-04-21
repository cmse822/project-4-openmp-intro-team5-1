// Part 1 Q2 -- Updated script for MMM without openmp
#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <cmath>
#include <iomanip>
#include "get_walltime.h" 

using namespace std;

vector<vector<int> >matrix_generator(int num) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> number(1, 10);
    vector<vector<int> > matrix(num, vector<int>(num, 0));
    for (int i = 0; i < num; ++i) {
        for (int j = 0; j < num; ++j) {
            matrix[i][j] = number(gen);
        }
    }
    return matrix;
}

void matrix_pinter(const vector<vector<int> >& matrix_temp){
    for (vector<int> row: matrix_temp) {
        for (int val : row) {
            cout << val << " ";
        }
        cout << endl;
    }
}

double matrix_multiplication(int num){
    double time_begin, time_end;
    vector<vector<int> > result(num, vector<int>(num, 0));

    vector<vector<int> > matrix_one = matrix_generator(num);

    vector<vector<int> > matrix_two = matrix_generator(num);

    get_walltime(&time_begin);

    for(int i=0;i<num;++i){
        for(int j=0;j<num;++j){
            for(int k=0;k<num;++k){
                result[i][j] += matrix_one[i][k] * matrix_two[k][j];
            }
        }
    }

    get_walltime(&time_end);

    //cout<<"Performance: "<<(time_end-time_begin)<<endl;
    return time_end-time_begin;
}

double getMflops(int num) {
    double average_time = 0.0;
    /// reduce noise by taking avg of 10 tries
    for(int i=0;i<10;++i){
       average_time += matrix_multiplication(num);
    }
    average_time/=10;

    cout<<"Average Time Computing: " << average_time<<endl;
    double Mflop_s = (pow(num,3)*2)/average_time/pow(10,6);
    cout << scientific << setprecision(2);
    cout<<"Performance in Mflop/s(scientific format): " << Mflop_s<<endl;
    cout << fixed << setprecision(2);
    cout<<"Performance in Mflop/s: " << Mflop_s<<endl;
    return Mflop_s;
}

double getGflops(int num) {
    double average_time = 0.0;
    /// reduce noise by taking avg of 10 tries
    for(int i=0;i<10;++i){
       average_time += matrix_multiplication(num);
    }
    average_time/=10;
    
    cout<<"Average Time Computing: " << average_time<<endl;
    double Gflop_s = (pow(num,3)*2)/average_time/pow(10,9);
    cout << scientific << setprecision(2);
    cout<<"Performance in Gflop/s(scientific format): " << Gflop_s<<endl;
    cout << fixed << setprecision(2);
    cout<<"Performance in Gflop/s: " << Gflop_s<<endl;
    return Gflop_s;
}

int main(){
    ///q3
    //getMflops(100);

    ///q5
    std::vector<int> Ns ={1,10,25,50,80,100,150,200,300,400,600,1000,1200,1500,2000,3000,4000,5000,6000};
    
    std::vector<double> Gflops;

    for (int n : Ns) {
        cout << "Current n: " << n << endl;
        double gflop = getGflops(n);
        Gflops.push_back(gflop);
    }

    std::ofstream outFile("gflops.txt");
    if (outFile.is_open()) {
        for (size_t i = 0; i < Gflops.size(); ++i) {
            outFile << Gflops[i] << ",";
        }
        outFile.close();
    } else {
        std::cerr << "Unable to open file for writing." << std::endl;
    }

    return 0;
}

// Compile like this: g++ -o Part_1_Q_2_nonopenmp Part_1_Q_2_nonopenmp.cpp get_walltime.c