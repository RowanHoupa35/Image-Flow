#include <iostream>
#include <iomanip>
#include "core/Image.hpp"
#include "core/filters/GrayscaleFilter.hpp"
#include "core/filters/GrayscaleFilterGPU.hpp"
#include "core/filters/InvertFilter.hpp"
#include <chrono>

void testGPUvsCPU() {
    std::cout << "\n=== TEST PARALLÉLISATION GPU vs CPU ===\n";
    
    // Créer une image de test (grande pour le benchmark)
    Image testImage(2048, 2048, 3);
    for (int y = 0; y < testImage.getHeight(); ++y) {
        for (int x = 0; x < testImage.getWidth(); ++x) {
            testImage.at(x, y, 0) = (x * y) % 256;
            testImage.at(x, y, 1) = (x + y) % 256;
            testImage.at(x, y, 2) = (x - y + 256) % 256;
        }
    }
    
    std::cout << "Image de test: " << testImage.getWidth() 
              << "x" << testImage.getHeight() 
              << " (" << testImage.size() / 1024 / 1024 << " MB)\n";
    
    // Test CPU
    GrayscaleFilter cpuFilter;
    Image cpuResult;
    
    auto cpuStart = std::chrono::high_resolution_clock::now();
    cpuFilter.apply(testImage, cpuResult);
    auto cpuEnd = std::chrono::high_resolution_clock::now();
    double cpuTime = std::chrono::duration<double, std::milli>(
        cpuEnd - cpuStart
    ).count();
    
    // Test GPU
    GrayscaleFilterGPU gpuFilter;
    Image gpuResult;
    
    auto gpuStart = std::chrono::high_resolution_clock::now();
    gpuFilter.apply(testImage, gpuResult);
    auto gpuEnd = std::chrono::high_resolution_clock::now();
    double gpuTime = std::chrono::duration<double, std::milli>(
        gpuEnd - gpuStart
    ).count();
    
    // Résultats
    std::cout << "\n=== RÉSULTATS ===\n";
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "CPU (séquentiel): " << cpuTime << " ms\n";
    std::cout << "GPU (SYCL): " << gpuTime << " ms\n";
    std::cout << "Speedup: " << cpuTime / gpuTime << "x\n";
    
    // Vérifier que les résultats sont identiques
    bool resultsMatch = true;
    int errors = 0;
    for (int i = 0; i < std::min(cpuResult.size(), gpuResult.size()); ++i) {
        if (std::abs(cpuResult.data()[i] - gpuResult.data()[i]) > 1) {
            resultsMatch = false;
            errors++;
            if (errors < 5) {
                std::cout << "Mismatch à l'index " << i 
                          << ": CPU=" << (int)cpuResult.data()[i]
                          << ", GPU=" << (int)gpuResult.data()[i] << std::endl;
            }
        }
    }
    
    if (resultsMatch) {
        std::cout << "✓ Résultats CPU/GPU identiques\n";
    } else {
        std::cout << "✗ " << errors << " différences trouvées\n";
    }
}

void testMultiThread() {
    std::cout << "\n=== TEST MULTI-CŒURS ===\n";
    
    Image img(1024, 1024, 3);
    InvertFilter filter;
    Image result;
    
    // Test avec différents nombres de threads
    for (int threads : {1, 2, 4, 8}) {
        omp_set_num_threads(threads);
        
        auto start = std::chrono::high_resolution_clock::now();
        filter.apply(img, result);
        auto end = std::chrono::high_resolution_clock::now();
        
        double time = std::chrono::duration<double, std::milli>(
            end - start
        ).count();
        
        std::cout << threads << " thread(s): " << time << " ms\n";
    }
}

int main() {
    std::cout << "=== BENCHMARK PARALLÉLISATION ===\n";
    
    testMultiThread();
    testGPUvsCPU();
    
    return 0;
}
