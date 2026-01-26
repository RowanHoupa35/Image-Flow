/**
 * @file GrayscaleFilterGPU.hpp
 * @brief GPU-accelerated grayscale conversion using SYCL
 *
 * SYCL implementation of grayscale conversion for massive parallelism.
 * Each pixel is processed by a separate GPU thread, enabling processing
 * of millions of pixels simultaneously.
 *
 * SYCL Features Used:
 * - sycl::queue with gpu_selector_v for GPU targeting
 * - sycl::buffer for automatic CPU<->GPU memory transfer
 * - parallel_for kernel for data-parallel execution
 * - Exception handling with automatic CPU fallback
 *
 * Performance: 10-100x faster than CPU for large images (>1920x1080)
 *
 * @see GrayscaleFilter for CPU OpenMP implementation
 * @see Filter.hpp for the base class interface
 *
 * @author Rowan HOUPA
 * @date January 2026
 */

#ifndef GRAYSCALE_FILTER_GPU_HPP
#define GRAYSCALE_FILTER_GPU_HPP

#include "../Filter.hpp"
#include <sycl/sycl.hpp>
#include <chrono>

class GrayscaleFilterGPU : public Filter {
public:
    void apply(const Image& input, Image& output) override;
    std::string getName() const override { return "Grayscale (GPU-SYCL)"; }
    std::unique_ptr<Filter> clone() const override {
        return std::make_unique<GrayscaleFilterGPU>(*this);
    }
    bool supportsGPU() const override { return true; }
    
    double getLastExecutionTime() const override { return lastExecutionTime; } 
    
private:
    double lastExecutionTime = 0.0;
};

#endif