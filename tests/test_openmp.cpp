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