/**
 * @file SepiaFilter.hpp
 * @brief Sepia tone vintage photo effect filter with OpenMP parallelization
 *
 * This filter applies a warm, brownish sepia tone to images, simulating the
 * look of antique photographs. Uses standard sepia transformation coefficients
 * derived from photographic color science.
 *
 * @details
 * - Parallelization: OpenMP with dynamic scheduling for load balancing
 * - Formula (per Microsoft/Adobe standards):
 *     newR = 0.393*R + 0.769*G + 0.189*B
 *     newG = 0.349*R + 0.686*G + 0.168*B
 *     newB = 0.272*R + 0.534*G + 0.131*B
 * - Values are clamped to [0, 255] to prevent overflow
 * - Grayscale images are passed through unchanged
 *
 * @note This is also a DEMONSTRATION filter showing the plugin architecture:
 *       just create the file, register it in FilterRegistration.cpp, rebuild!
 * @see GrayscaleFilter, BrightnessFilter for other color transformation filters
 * @author Rowan HOUPA
 * @date January 2026
 */

#ifndef SEPIA_FILTER_HPP
#define SEPIA_FILTER_HPP

#include "../Filter.hpp"

/**
 * @class SepiaFilter
 * @brief Applies a sepia tone effect to the image using OpenMP parallelization.
 */
class SepiaFilter : public Filter {
public:
    void apply(const Image& input, Image& output) override {
        // Create output with same dimensions
        output = Image(input.getWidth(), input.getHeight(), input.getChannels());

        int width = input.getWidth();
        int height = input.getHeight();
        int channels = input.getChannels();

        // Apply sepia tone transformation with OpenMP parallelization
        #pragma omp parallel for schedule(dynamic)
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                if (channels >= 3) {
                    int r = input.at(x, y, 0);
                    int g = input.at(x, y, 1);
                    int b = input.at(x, y, 2);

                    // Sepia tone transformation matrix
                    int newR = static_cast<int>(0.393 * r + 0.769 * g + 0.189 * b);
                    int newG = static_cast<int>(0.349 * r + 0.686 * g + 0.168 * b);
                    int newB = static_cast<int>(0.272 * r + 0.534 * g + 0.131 * b);

                    // Clamp values
                    output.at(x, y, 0) = std::min(255, newR);
                    output.at(x, y, 1) = std::min(255, newG);
                    output.at(x, y, 2) = std::min(255, newB);
                } else {
                    // Grayscale image - copy as is
                    output.at(x, y, 0) = input.at(x, y, 0);
                }
            }
        }
    }

    std::string getName() const override {
        return "Sepia Tone";
    }

    std::unique_ptr<Filter> clone() const override {
        return std::make_unique<SepiaFilter>(*this);
    }
};

#endif // SEPIA_FILTER_HPP
