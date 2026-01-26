/**
 * @file InvertFilter.cpp
 * @brief Color inversion filter implementation using OpenMP
 *
 * Inverts all color channels to create a photographic negative effect.
 * Each pixel value V is transformed to (255 - V).
 *
 * @details
 * Algorithm: output[i] = 255 - input[i] for all pixels
 * - Simple subtraction from maximum value
 * - Works on all channels (RGB, RGBA, grayscale)
 * - No color space conversion needed
 *
 * Parallelization Strategy:
 * - Flattened 1D loop over all pixel components
 * - Dynamic scheduling with 256-pixel chunks for load balancing
 * - Thread count auto-detected via omp_get_max_threads()
 * - No synchronization needed (embarrassingly parallel)
 *
 * Performance:
 * - Memory-bound operation (simple arithmetic)
 * - Scales well with core count
 * - Debug mode prints thread count and pixel count
 *
 * @author Rowan HOUPA
 * @date January 2026
 */

#include "filters/InvertFilter.hpp"
#include <omp.h>
#include <iostream>

void InvertFilter::apply(const Image& input, Image& output) {
    output = Image(input.getWidth(), input.getHeight(), input.getChannels());
    
    int totalPixels = input.getWidth() * input.getHeight() * input.getChannels();
    
    int numThreads = omp_get_max_threads();
    omp_set_num_threads(numThreads);
    
    #pragma omp parallel for schedule(dynamic, 256)
    for (int i = 0; i < totalPixels; ++i) {
        output.data()[i] = 255 - input.data()[i];
    }
    
    #ifdef DEBUG
    #pragma omp single
    std::cout << "[CPU] InvertFilter - Threads: " << numThreads 
              << ", Pixels: " << totalPixels << std::endl;
    #endif
}