/**
 * @file BoxBlurFilter.cpp
 * @brief Box blur (averaging) filter implementation using OpenMP
 *
 * Applies a box blur effect by averaging each pixel with its neighbors
 * within a specified radius. Creates a smoothing/softening effect.
 *
 * @details
 * Algorithm:
 * - For each pixel, compute the average of all pixels in a square neighborhood
 * - Neighborhood size: (2*radius + 1) × (2*radius + 1)
 * - Edge pixels use only available neighbors (no padding)
 *
 * Parallelization Strategy:
 * - Uses OpenMP collapse(2) to parallelize X and Y loops together
 * - Dynamic scheduling handles varying workloads at image edges
 * - Each output pixel computed independently (no race conditions)
 *
 * Complexity: O(width × height × radius²)
 * - Larger radius = more neighbors to average = slower
 * - Consider separable box filter for O(width × height × radius)
 *
 * Performance:
 * - Execution time measured for benchmarking
 * - Typical radius: 1-5 for subtle blur, 10+ for strong blur
 *
 * @see BoxBlurFilterGPU.cpp for GPU-accelerated version
 * @author Rowan HOUPA
 * @date January 2026
 */

#include "filters/BoxBlurFilter.hpp"
#include <algorithm>
#include <vector>

void BoxBlurFilter::apply(const Image& input, Image& output) {
    auto start = std::chrono::high_resolution_clock::now();
    
    output = Image(input.getWidth(), input.getHeight(), input.getChannels());
    
    int width = input.getWidth();
    int height = input.getHeight();
    int channels = input.getChannels();
    
    #pragma omp parallel for collapse(2) schedule(dynamic)
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            for (int c = 0; c < channels; ++c) {
                float sum = 0.0f;
                int count = 0;
                
                for (int ky = -kernelRadius; ky <= kernelRadius; ++ky) {
                    int ny = y + ky;
                    if (ny < 0 || ny >= height) continue;
                    
                    for (int kx = -kernelRadius; kx <= kernelRadius; ++kx) {
                        int nx = x + kx;
                        if (nx < 0 || nx >= width) continue;
                        
                        sum += static_cast<float>(input.at(nx, ny, c));
                        count++;
                    }
                }
                
                float avg = sum / count;
                output.at(x, y, c) = static_cast<uint8_t>(
                    std::clamp(avg, 0.0f, 255.0f)
                );
            }
        }
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    lastExecutionTime = std::chrono::duration<double, std::milli>(end - start).count();
}
