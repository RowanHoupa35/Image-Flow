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