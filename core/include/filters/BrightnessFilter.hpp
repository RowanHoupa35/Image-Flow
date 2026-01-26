/**
 * @file BrightnessFilter.hpp
 * @brief Adjustable brightness filter with OpenMP parallelization
 *
 * Multiplies all pixel values by a brightness factor:
 * - factor < 1.0: Darkens the image
 * - factor = 1.0: No change
 * - factor > 1.0: Brightens the image
 *
 * Implementation uses OpenMP with collapse(2) directive to parallelize
 * both X and Y loops for maximum thread utilization.
 *
 * This is a parameterized filter demonstrating constructor-based
 * configuration in the polymorphic architecture.
 *
 * @see Filter.hpp for the base class interface
 *
 * @author Rowan HOUPA
 * @date January 2026
 */

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
    
    float getBrightness() const { return brightnessFactor; }
    void setBrightness(float factor) { brightnessFactor = factor; }
    
private:
    float brightnessFactor = 1.0f; // 1.0 = no change, <1.0 = darker, >1.0 = brighter
};

#endif