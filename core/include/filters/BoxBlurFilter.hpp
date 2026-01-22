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