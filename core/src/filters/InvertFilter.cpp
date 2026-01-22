// Dans core/src/filters/InvertFilter.cpp
#include "filters/InvertFilter.hpp"
#include <omp.h>
#include <iostream>

void InvertFilter::apply(const Image& input, Image& output) {
    output = Image(input.getWidth(), input.getHeight(), input.getChannels());
    
    int totalPixels = input.getWidth() * input.getHeight() * input.getChannels();
    
    // Configuration OpenMP dynamique
    int numThreads = omp_get_max_threads();
    omp_set_num_threads(numThreads);
    
    #pragma omp parallel for schedule(dynamic, 256)
    for (int i = 0; i < totalPixels; ++i) {
        output.data()[i] = 255 - input.data()[i];
    }
    
    // Debug info
    #ifdef DEBUG
    #pragma omp single
    std::cout << "[CPU] InvertFilter - Threads: " << numThreads 
              << ", Pixels: " << totalPixels << std::endl;
    #endif
}