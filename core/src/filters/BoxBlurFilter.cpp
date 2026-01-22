#include "filters/BoxBlurFilter.hpp"
#include <algorithm>
#include <vector>

void BoxBlurFilter::apply(const Image& input, Image& output) {
    auto start = std::chrono::high_resolution_clock::now();
    
    output = Image(input.getWidth(), input.getHeight(), input.getChannels());
    
    int width = input.getWidth();
    int height = input.getHeight();
    int channels = input.getChannels();
    
    // Version CPU avec OpenMP
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