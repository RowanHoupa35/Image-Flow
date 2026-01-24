/**
 * @file InvertFilter.hpp
 * @brief Color inversion filter (negative effect) using OpenMP
 *
 * Inverts all color values in an image by computing: NewValue = 255 - OldValue
 * This creates a photographic negative effect.
 *
 * Implementation uses OpenMP with chunk-based scheduling (256 pixels per chunk)
 * for optimal cache utilization on linear memory access patterns.
 *
 * @see Filter.hpp for the base class interface
 *
 * @author Rowan HOUPA
 * @date January 2026
 */

#ifndef INVERT_FILTER_HPP
#define INVERT_FILTER_HPP

#include "../Filter.hpp"

class InvertFilter : public Filter {
public:
    void apply(const Image& input, Image& output) override;
    std::string getName() const override { return "Invert"; }
    std::unique_ptr<Filter> clone() const override {
        return std::make_unique<InvertFilter>(*this);
    }
    
    // Simple inversion works equally well on CPU/GPU
    bool supportsGPU() const override { return true; }
};

#endif