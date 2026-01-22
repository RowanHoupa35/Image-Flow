#ifndef BRIGHTNESS_FILTER_HPP
#define BRIGHTNESS_FILTER_HPP

#include "../Filter.hpp"
#include <memory>

class BrightnessFilter : public Filter {
public:
    BrightnessFilter(float factor = 1.0f) : brightnessFactor(factor) {}
    
    void apply(const Image& input, Image& output) override;
    std::string getName() const override { 
        return "Brightness (" + std::to_string(brightnessFactor) + ")"; 
    }
    std::unique_ptr<Filter> clone() const override {
        return std::make_unique<BrightnessFilter>(*this);
    }
    
    // Getter/Setter for UI control
    float getBrightness() const { return brightnessFactor; }
    void setBrightness(float factor) { brightnessFactor = factor; }
    
private:
    float brightnessFactor = 1.0f; // 1.0 = no change, <1.0 = darker, >1.0 = brighter
};

#endif