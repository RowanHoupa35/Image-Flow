/**
 * @file BrightnessFilter.cpp
 * @brief Brightness adjustment filter implementation using OpenMP
 *
 * Adjusts image brightness by multiplying all pixel values by a factor.
 * Factor > 1.0 brightens, factor < 1.0 darkens, factor = 1.0 unchanged.
 *
 * @details
 * Algorithm: output[x,y,c] = clamp(input[x,y,c] * factor, 0, 255)
 * - Linear scaling of pixel values
 * - Clamping prevents overflow/underflow
 * - Applied uniformly to all channels
 *
 * Parallelization Strategy:
 * - Uses OpenMP collapse(2) to parallelize both X and Y loops
 * - Combined iteration space divided among threads
 * - Better load distribution than parallelizing only outer loop
 *
 * Usage:
 * - factor = 0.5: 50% darker
 * - factor = 1.0: no change
 * - factor = 1.5: 50% brighter
 * - factor = 2.0: double brightness (may saturate)
 *
 * @author Rowan HOUPA
 * @date January 2026
 */

#include "filters/BrightnessFilter.hpp"
#include <algorithm>

void BrightnessFilter::apply(const Image& input, Image& output) {
    output = Image(input.getWidth(), input.getHeight(), input.getChannels());
    
    int width = input.getWidth();
    int height = input.getHeight();
    int channels = input.getChannels();
    
    #pragma omp parallel for collapse(2)
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            for (int c = 0; c < channels; ++c) {
                float pixel = static_cast<float>(input.at(x, y, c));
                pixel *= brightnessFactor;
                
                pixel = std::clamp(pixel, 0.0f, 255.0f);
                
                output.at(x, y, c) = static_cast<uint8_t>(pixel);
            }
        }
    }
}
