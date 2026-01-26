/**
 * @file benchmark.cpp
 * @brief Performance benchmark comparing CPU (OpenMP) vs GPU (SYCL)
 *
 * Runs standardized tests to measure and compare execution times between
 * CPU and GPU implementations of the same filters. Useful for evaluating
 * hardware-specific performance characteristics.
 *
 * @details
 * Test Configuration:
 * - Image size: 2000x1500 pixels (RGB) = ~9 MB
 * - Test image: Synthetic gradient pattern (no I/O overhead)
 * - Filters tested: Grayscale, Box Blur (radius=3)
 *
 * Measurements:
 * - Execution time per filter (milliseconds)
 * - Speedup ratio (CPU time / GPU time)
 *
 * Expected Results:
 * - Discrete GPU: 10-100x speedup for large images
 * - Integrated GPU: May be slower due to memory transfer overhead
 * - CPU: Consistent baseline, benefits from OpenMP multi-threading
 *
 * Usage Notes:
 * - Run multiple times for consistent results (first run may be slower)
 * - GPU performance depends on driver, device, and memory bandwidth
 * - Small images may show CPU faster due to GPU setup overhead
 *
 * @see GrayscaleFilter, GrayscaleFilterGPU for grayscale implementations
 * @see BoxBlurFilter, BoxBlurFilterGPU for blur implementations
 * @author Rowan HOUPA
 * @date January 2026
 */

#include <iostream>
#include <iomanip>
#include "Image.hpp"
#include "filters/GrayscaleFilter.hpp"
#include "filters/GrayscaleFilterGPU.hpp"
#include "filters/BoxBlurFilter.hpp"
#include "filters/BoxBlurFilterGPU.hpp"

void printHeader() {
    std::cout << "\n╔═══════════════════════════════════════════════════════════════╗\n";
    std::cout << "║        IMAGEFLOW - BENCHMARK CPU vs GPU (SYCL)               ║\n";
    std::cout << "╚═══════════════════════════════════════════════════════════════╝\n\n";
}

void benchmark(const std::string& name, Filter& filter, const Image& img) {
    Image result;
    filter.apply(img, result);
    
    std::cout << std::setw(30) << std::left << name 
              << ": " << std::setw(10) << std::right 
              << std::fixed << std::setprecision(2) 
              << filter.getLastExecutionTime() << " ms\n";
}

int main() {
    printHeader();
    
    std::cout << "Création d'une image de test (2000x1500)...\n";
    Image testImg(2000, 1500, 3);
    
    for (int y = 0; y < testImg.getHeight(); ++y) {
        for (int x = 0; x < testImg.getWidth(); ++x) {
            testImg.at(x, y, 0) = (x * 255) / testImg.getWidth();
            testImg.at(x, y, 1) = (y * 255) / testImg.getHeight();
            testImg.at(x, y, 2) = 128;
        }
    }
    
    std::cout << "Image créée: " << testImg.getWidth() << "x" 
              << testImg.getHeight() << " (" << testImg.size() / 1024 / 1024 
              << " MB)\n\n";
    
    std::cout << " Test 1: GRAYSCALE\n";
    std::cout << std::string(50, '-') << "\n";
    
    GrayscaleFilter gsCPU;
    GrayscaleFilterGPU gsGPU;
    
    benchmark("CPU (séquentiel)", gsCPU, testImg);
    benchmark("GPU (SYCL parallèle)", gsGPU, testImg);
    
    double speedup1 = gsCPU.getLastExecutionTime() / gsGPU.getLastExecutionTime();
    std::cout << "Speedup GPU: " << std::setprecision(2) << speedup1 << "x\n\n";
    
    std::cout << " Test 2: BOX BLUR (radius=3)\n";
    std::cout << std::string(50, '-') << "\n";
    
    BoxBlurFilter blurCPU(3);
    BoxBlurFilterGPU blurGPU(3);
    
    benchmark("CPU (OpenMP)", blurCPU, testImg);
    benchmark("GPU (SYCL parallèle)", blurGPU, testImg);
    
    double speedup2 = blurCPU.getLastExecutionTime() / blurGPU.getLastExecutionTime();
    std::cout << " Speedup GPU: " << std::setprecision(2) << speedup2 << "x\n\n";
    
    std::cout << "╔═══════════════════════════════════════════════════════════════╗\n";
    std::cout << "║                       RÉSUMÉ                                  ║\n";
    std::cout << "╠═══════════════════════════════════════════════════════════════╣\n";
    std::cout << "║ Grayscale Speedup GPU: " << std::setw(10) << speedup1 << "x                     ║\n";
    std::cout << "║ Blur Speedup GPU:      " << std::setw(10) << speedup2 << "x                     ║\n";
    std::cout << "╚═══════════════════════════════════════════════════════════════╝\n";
    
    return 0;
}