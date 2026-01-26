/**
 * @file GrayscaleFilter.cpp
 * @brief CPU implementation of grayscale conversion using OpenMP
 *
 * Converts RGB images to single-channel grayscale using the luminance
 * formula derived from human visual perception (ITU-R BT.601 standard).
 *
 * @details
 * Algorithm: Gray = 0.299*R + 0.587*G + 0.114*B
 * - Green has highest weight (human eyes most sensitive to green)
 * - Red has medium weight
 * - Blue has lowest weight
 *
 * Parallelization Strategy:
 * - Uses OpenMP #pragma omp parallel for
 * - Dynamic scheduling for load balancing across threads
 * - Each row processed independently (no data dependencies)
 * - Thread count determined by OMP_NUM_THREADS environment variable
 *
 * Performance:
 * - Execution time measured and stored in lastExecutionTime
 * - Typical speedup: 4-8x on 8-core CPU vs single-threaded
 *
 * @see GrayscaleFilterGPU.cpp for GPU version
 * @author Rowan HOUPA
 * @date January 2026
 */

#include "filters/GrayscaleFilter.hpp"

void GrayscaleFilter::apply(const Image& input, Image& output) {
    auto start = std::chrono::high_resolution_clock::now();

    output = Image(input.getWidth(), input.getHeight(), 1);

    int width = input.getWidth();
    int height = input.getHeight();

    #pragma omp parallel for schedule(dynamic)
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            uint8_t r = input.at(x, y, 0);
            uint8_t g = input.at(x, y, 1);
            uint8_t b = input.at(x, y, 2);

            uint8_t gray = static_cast<uint8_t>(0.299f * r + 0.587f * g + 0.114f * b);
            output.at(x, y, 0) = gray;
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    lastExecutionTime = std::chrono::duration<double, std::milli>(end - start).count();
}