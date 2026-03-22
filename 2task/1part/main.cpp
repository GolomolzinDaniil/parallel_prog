#include <omp.h>
#include <iostream>
#include <memory>
#include <chrono>


int main(int argc, char *argv[]) {

    size_t n = 20'000;
    size_t m = 20'000;
    
    if (argc > 1) m = atoi(argv[1]);
    if (argc > 2) n = atoi(argv[2]);

    auto a = std::make_unique<double[]>(m * n);
    auto b = std::make_unique<double[]>(n);
    auto c = std::make_unique<double[]>(m);

    #pragma omp parallel
    {   
        // разделение данных для строк
        size_t nthreads = omp_get_num_threads();
        size_t threadid = omp_get_thread_num();

        size_t row_per_thread = m / nthreads;
        size_t lb_row = threadid * row_per_thread;
        size_t ub_row = (threadid == nthreads - 1) ? (m) : (lb_row + row_per_thread);
        
        // параллельное заполнение по строкам
        for (size_t i = lb_row; i < ub_row; i++) {
            size_t curr = i * n;
            for (size_t j = 0; j < n; j++) {
                a[curr + j] = i + j;
            }
        }

        // разделение данных для столбцов
        size_t col_per_thread = n / nthreads;
        size_t lb_col = threadid * col_per_thread;
        size_t ub_col = (threadid == nthreads - 1) ? (n) : (lb_col + col_per_thread);
        
        // параллельное заполнение по столбцам
        for (size_t j = lb_col; j < ub_col; j++) b[j] = j;

        // умножение
        #pragma omp barrier
        const auto start = std::chrono::steady_clock::now();
        
        for (size_t i = lb_row; i < ub_row; i++) {
            double sum = 0.0;
            size_t curr = i * n;
            for (size_t j = 0; j < n; j++) {
                sum += a[curr + j] * b[j];
            }
            c[i] = sum;
        }
        #pragma omp barrier
        const auto end = std::chrono::steady_clock::now();

        #pragma omp single
        std::cout << std::chrono::duration<double>(end - start).count() << std::endl;
    }
};