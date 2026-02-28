#include <stdio.h>
#include <iostream>
#include <omp.h>
#include <fstream>
#include <chrono>
#include <cmath>

#define NSTEP 160'000'000


double func(double x) {
    return exp(-x*x);
};

// template<typename Func>
// double integrate_omp(Func&& func, double a, double b) {

//     double h = (b - a) / NSTEP;
//     double sum = 0.0;

//     #pragma omp parallel
//     {
//         int num_threads = omp_get_num_threads();
//         int thread_id = omp_get_thread_num();
//         int new_size = NSTEP / num_threads;

//         int start = thread_id * new_size;
//         int finish = (thread_id == num_threads - 1) ? NSTEP : (thread_id + 1) * new_size;

//         double local_sum = 0;

//         for (int i = start; i < finish; i++) {

//             local_sum += func(a + h * (i + 0.5));
//         }

//         #pragma omp atomic
//         sum += local_sum;
//     }
//     sum *= h;
//     return sum;
// };

int main() {
    
    double a = -4.0;
    double b = 4.0;

    double h = (b - a) / NSTEP;
    double sum = 0.0;


    auto start = std::chrono::high_resolution_clock::now();
    #pragma omp parallel
    {
        int num_threads = omp_get_num_threads();
        int thread_id = omp_get_thread_num();
        int new_size = NSTEP / num_threads;

        int start = thread_id * new_size;
        int finish = (thread_id == num_threads - 1) ? NSTEP : (thread_id + 1) * new_size;

        double local_sum = 0;

        for (int i = start; i < finish; i++) {

            local_sum += func(a + h * (i + 0.5));
        }

        #pragma omp atomic
        sum += local_sum;
    }
    sum *= h;
    auto end = std::chrono::high_resolution_clock::now();

    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;
};