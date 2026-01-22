#include "filters/BrightnessFilter.hpp"
#include <algorithm> // for std::clamp

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
                
                // Clamp to [0, 255]
                pixel = std::clamp(pixel, 0.0f, 255.0f);
                
                output.at(x, y, c) = static_cast<uint8_t>(pixel);
            }
        }
    }
}