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
    
    double getLastExecutionTime() const override { return lastExecutionTime; }  // ← Ajout de override
    
private:
    double lastExecutionTime = 0.0;
};

#endif