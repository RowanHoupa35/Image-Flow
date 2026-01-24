/**
 * @file test_openmp.cpp
 * @brief OpenMP parallelization verification and speedup test
 *
 * Simple vector addition benchmark to verify OpenMP is working correctly
 * and measure the speedup from multi-threading.
 *
 * @details
 * Test: Vector addition c[i] = a[i] + b[i] for 10 million elements
 *
 * Measurements:
 * - Sequential execution time (single-threaded)
 * - Parallel execution time (OpenMP multi-threaded)
 * - Speedup ratio (sequential / parallel)
 *
 * Expected Results:
 * - On 4-core CPU: ~3-4x speedup
 * - On 8-core CPU: ~6-8x speedup
 * - Speedup may be limited by memory bandwidth
 *
 * Verification:
 * - Prints number of available threads (OMP_NUM_THREADS)
 * - Compares times to confirm parallelization is active
 *
 * @note If speedup is ~1x, check OpenMP compilation flags (-fopenmp)
 * @author ENSTA Paris - IN204 Project
 * @date January 2026
 */

#include <iostream>
#include <vector>
#include <chrono>
#include <omp.h>

int main() {
    const int N = 10000000;
    std::vector<double> a(N, 1.5);
    std::vector<double> b(N, 2.5);
    std::vector<double> c(N, 0.0);
    
    // Version séquentielle
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) {
        c[i] = a[i] + b[i];
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto seq_time = std::chrono::duration<double>(end - start).count();
    
    std::fill(c.begin(), c.end(), 0.0);
    
    // Version parallèle
    start = std::chrono::high_resolution_clock::now();
    #pragma omp parallel for
    for (int i = 0; i < N; ++i) {
        c[i] = a[i] + b[i];
    }
    end = std::chrono::high_resolution_clock::now();
    auto par_time = std::chrono::duration<double>(end - start).count();
    
    std::cout << "Threads disponibles: " << omp_get_max_threads() << std::endl;
    std::cout << "Temps séquentiel: " << seq_time * 1000 << " ms" << std::endl;
    std::cout << "Temps parallèle: " << par_time * 1000 << " ms" << std::endl;
    std::cout << "Speedup: " << seq_time / par_time << "x" << std::endl;
    
    return 0;
}