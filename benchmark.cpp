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
    
    // Créer image de test
    std::cout << "Création d'une image de test (2000x1500)...\n";
    Image testImg(2000, 1500, 3);
    
    // Remplir avec un gradient
    for (int y = 0; y < testImg.getHeight(); ++y) {
        for (int x = 0; x < testImg.getWidth(); ++x) {
            testImg.at(x, y, 0) = (x * 255) / testImg.getWidth();
            testImg.at(x, y, 1) = (y * 255) / testImg.getHeight();
            testImg.at(x, y, 2) = 128;
        }
    }
    
    std::cout << "✓ Image créée: " << testImg.getWidth() << "x" 
              << testImg.getHeight() << " (" << testImg.size() / 1024 / 1024 
              << " MB)\n\n";
    
    // Benchmark Grayscale
    std::cout << " Test 1: GRAYSCALE\n";
    std::cout << std::string(50, '-') << "\n";
    
    GrayscaleFilter gsCPU;
    GrayscaleFilterGPU gsGPU;
    
    benchmark("CPU (séquentiel)", gsCPU, testImg);
    benchmark("GPU (SYCL parallèle)", gsGPU, testImg);
    
    double speedup1 = gsCPU.getLastExecutionTime() / gsGPU.getLastExecutionTime();
    std::cout << "Speedup GPU: " << std::setprecision(2) << speedup1 << "x\n\n";
    
    // Benchmark Blur
    std::cout << " Test 2: BOX BLUR (radius=3)\n";
    std::cout << std::string(50, '-') << "\n";
    
    BoxBlurFilter blurCPU(3);
    BoxBlurFilterGPU blurGPU(3);
    
    benchmark("CPU (OpenMP)", blurCPU, testImg);
    benchmark("GPU (SYCL parallèle)", blurGPU, testImg);
    
    double speedup2 = blurCPU.getLastExecutionTime() / blurGPU.getLastExecutionTime();
    std::cout << " Speedup GPU: " << std::setprecision(2) << speedup2 << "x\n\n";
    
    // Résumé
    std::cout << "╔═══════════════════════════════════════════════════════════════╗\n";
    std::cout << "║                       RÉSUMÉ                                  ║\n";
    std::cout << "╠═══════════════════════════════════════════════════════════════╣\n";
    std::cout << "║ Grayscale Speedup GPU: " << std::setw(10) << speedup1 << "x                     ║\n";
    std::cout << "║ Blur Speedup GPU:      " << std::setw(10) << speedup2 << "x                     ║\n";
    std::cout << "╚═══════════════════════════════════════════════════════════════╝\n";
    
    return 0;
}