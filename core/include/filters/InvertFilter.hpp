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