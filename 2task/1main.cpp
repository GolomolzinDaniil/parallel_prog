#include <omp.h>
#include <iostream>
#include <memory>
#include <chrono>


// void mult_parallel(double* a, double* b,  double* c, const size_t& m, const size_t& n, size_t threads) {

//     #pragma omp parallel num_threads(threads)
//     {
//         size_t nthreads = omp_get_num_threads();
//         size_t threadid = omp_get_thread_num();
//         size_t items_per_thread = m / nthreads;
//         size_t lb = threadid * items_per_thread;
//         size_t ub = (threadid == nthreads - 1) ? (m - 1) : (lb + items_per_thread - 1);

//         for (size_t i = lb; i <= ub; i++) {
//             c[i] = 0.0;
//             for (size_t j = 0; j < n; j++) {
//                 c[i] += a[i*n + j] * b[j];
//             }
//         }
//     }
// };

// void mult_serial(double* a, double* b, double* c, const size_t& m, const size_t& n) {

//     for (size_t i = 0; i < m; i++) {
//         c[i] = 0.0;
//         for (size_t j = 0; j < n; j++) {
//             c[i] += a[i*n + j] * b[j];
//         }
//     }
// };

// void init(double* a, double* b, size_t m, size_t n) {
//     for (size_t i = 0; i < m; i++) {
//         size_t curr = i * n;
//         for (size_t j = 0; j < n; j++) {
//             a[curr + j] = i + j;
//         }
//     }
//     for (size_t j = 0; j < n; j++) b[j] = j;
// };

// double run(double* a, double* b, double* c, size_t m, size_t n, bool fl=false, size_t num_threads=1) {

//     if (fl) {
//         const auto start = std::chrono::steady_clock::now();
    
//         mult_parallel(a, b, c, m, n, num_threads);
    
//         const auto end = std::chrono::steady_clock::now();
//         return std::chrono::duration<double>(end - start).count();
//     }
//     else {
//         const auto start = std::chrono::steady_clock::now();
    
//         mult_serial(a, b, c, m, n);
    
//         const auto end = std::chrono::steady_clock::now();
//         return std::chrono::duration<double>(end - start).count();  
//     }
// };


// // int main(int argc, char *argv[]) {

// //     size_t n = 20'000;
// //     size_t m = 20'000;
    
// //     if (argc > 1) m = atoi(argv[1]);
// //     if (argc > 2) n = atoi(argv[2]);

// //     auto a = std::make_unique<double[]>(m * n);
// //     auto b = std::make_unique<double[]>(n);
// //     auto c = std::make_unique<double[]>(m);

// //     init(a.get(),b.get(), m, n);

// //     double serial = run(a.get(),b.get(),c.get(),m,n);

// //     size_t pp[] = {1, 2, 4, 7, 8, 16, 20, 40};
// //     for (const auto& p : pp) {
// //         double parallel = run(a.get(), b.get(), c.get(), m,n,true,p);
// //         std::cout << parallel << " " << serial / parallel << std::endl;
// //     }
// // };

int main(int argc, char *argv[]) {

    size_t n = 20'000;
    size_t m = 20'000;
    
    if (argc > 1) m = atoi(argv[1]);
    if (argc > 2) n = atoi(argv[2]);

    auto a = std::make_unique<double[]>(m * n);
    auto b = std::make_unique<double[]>(n);
    auto c = std::make_unique<double[]>(m);

    for (size_t i = 0; i < m; i++) {
        size_t curr = i * n;
        for (size_t j = 0; j < n; j++) {
            a[curr + j] = i + j;
        }
    }
    for (size_t j = 0; j < n; j++) b[j] = j;


    const auto start = std::chrono::steady_clock::now();
    #pragma omp parallel
    {
        size_t nthreads = omp_get_num_threads();
        size_t threadid = omp_get_thread_num();
        size_t items_per_thread = m / nthreads;
        size_t lb = threadid * items_per_thread;
        size_t ub = (threadid == nthreads - 1) ? (m - 1) : (lb + items_per_thread - 1);

        for (size_t i = lb; i <= ub; i++) {
            c[i] = 0.0;
            for (size_t j = 0; j < n; j++) {
                c[i] += a[i*n + j] * b[j];
            }
        }
    }
    const auto end = std::chrono::steady_clock::now();

    std::cout << std::chrono::duration<double>(end - start).count(); << std::endl;
};