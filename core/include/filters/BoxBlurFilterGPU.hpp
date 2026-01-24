/**
 * @file BoxBlurFilterGPU.hpp
 * @brief GPU-accelerated box blur filter using SYCL
 *
 * This filter performs box blur (averaging) on the GPU using Intel oneAPI SYCL.
 * Each output pixel is computed as the average of all pixels within the
 * specified radius, running in parallel across all GPU compute units.
 *
 * @details
 * - SYCL Features: sycl::queue, sycl::buffer, parallel_for with 2D nd_range
 * - Algorithm: For each pixel, averages (2*radius+1)² neighboring pixels
 * - Boundary: Clamps to image edges (no wrap-around)
 * - Performance: Significant speedup on discrete GPUs; may be slower on
 *   integrated GPUs due to memory transfer overhead
 * - Complexity: O(width * height) kernel launches, each doing O(radius²) work
 *
 * @note Requires SYCL-compatible device. Falls back gracefully if unavailable.
 * @see BoxBlurFilter for CPU version with OpenMP
 * @author Rowan HOUPA
 * @date January 2026
 */

#ifndef BOX_BLUR_FILTER_GPU_HPP
#define BOX_BLUR_FILTER_GPU_HPP

#include "../Filter.hpp"
#include <sycl/sycl.hpp>
#include <chrono>

class BoxBlurFilterGPU : public Filter {
public:
    BoxBlurFilterGPU(int radius = 2) : blurRadius(radius) {}
    
    void apply(const Image& input, Image& output) override;
    std::string getName() const override { 
        return "BoxBlur GPU (r=" + std::to_string(blurRadius) + ")"; 
    }
    std::unique_ptr<Filter> clone() const override {
        return std::make_unique<BoxBlurFilterGPU>(*this);
    }
    bool supportsGPU() const override { return true; }
    
    int getRadius() const { return blurRadius; }
    void setRadius(int r) { blurRadius = r; }
    double getLastExecutionTime() const override { return lastExecutionTime; }  // ← override ajouté
    
private:
    int blurRadius;
    double lastExecutionTime = 0.0;
};

#endif