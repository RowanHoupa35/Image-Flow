/**
 * @file BoxBlurFilter.hpp
 * @brief Box blur (averaging) filter with OpenMP parallelization
 *
 * Applies a box blur effect by averaging pixel values within a square kernel.
 * Each output pixel is the mean of all pixels within the specified radius.
 *
 * Algorithm: For each pixel, compute average of (2*radius+1)^2 neighbors
 * Complexity: O(width * height * radius^2)
 *
 * Implementation uses OpenMP with collapse(2) and dynamic scheduling for
 * efficient 2D parallelization across all CPU cores.
 *
 * This is a parameterized filter with adjustable blur radius (1-10).
 *
 * @see BoxBlurFilterGPU for SYCL GPU implementation
 * @see Filter.hpp for the base class interface
 *
 * @author Rowan HOUPA
 * @date January 2026
 */

#ifndef BOXBLUR_FILTER_HPP
#define BOXBLUR_FILTER_HPP

#include "../Filter.hpp"
#include <memory>
#include <chrono>

class BoxBlurFilter : public Filter {
public:
    BoxBlurFilter(int radius = 1) : kernelRadius(radius) {}
    
    void apply(const Image& input, Image& output) override;
    std::string getName() const override {
        return "Box Blur (radius=" + std::to_string(kernelRadius) + ")";
    }
    std::unique_ptr<Filter> clone() const override {
        return std::make_unique<BoxBlurFilter>(*this);
    }
    
    int getRadius() const { return kernelRadius; }
    void setRadius(int radius) {
        kernelRadius = std::max(1, std::min(radius, 10));
    }
    
    bool supportsGPU() const override { return true; }
    double getLastExecutionTime() const override { return lastExecutionTime; }
    
private:
    int kernelRadius = 1;
    double lastExecutionTime = 0.0;
};

#endif