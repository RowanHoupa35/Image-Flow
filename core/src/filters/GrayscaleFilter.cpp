#include "filters/GrayscaleFilter.hpp"

void GrayscaleFilter::apply(const Image& input, Image& output) {
    auto start = std::chrono::high_resolution_clock::now();

    output = Image(input.getWidth(), input.getHeight(), 1);

    int width = input.getWidth();
    int height = input.getHeight();

    // OpenMP parallelization: Each row processed by a different thread
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