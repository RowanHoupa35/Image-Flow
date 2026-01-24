/**
 * @file GrayscaleFilter.hpp
 * @brief CPU-based grayscale conversion filter using OpenMP
 *
 * Converts a color image to grayscale using the luminosity method:
 *   Gray = 0.299*R + 0.587*G + 0.114*B
 *
 * This formula weights colors according to human perception of brightness.
 * The implementation uses OpenMP for multi-core CPU parallelization with
 * dynamic scheduling for optimal load balancing.
 *
 * @see GrayscaleFilterGPU for SYCL GPU implementation
 * @see Filter.hpp for the base class interface
 *
 * @author Rowan HOUPA
 * @date January 2026
 */

#ifndef GRAYSCALE_FILTER_HPP
#define GRAYSCALE_FILTER_HPP

#include "../Filter.hpp"
#include <chrono>

class GrayscaleFilter : public Filter {
public:
    void apply(const Image& input, Image& output) override;
    std::string getName() const override { return "Grayscale"; }
    std::unique_ptr<Filter> clone() const override {
        return std::make_unique<GrayscaleFilter>(*this);
    }
    double getLastExecutionTime() const override { return lastExecutionTime; }
    
private:
    double lastExecutionTime = 0.0;
};

#endif