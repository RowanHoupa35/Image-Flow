#ifndef FILTER_HPP
#define FILTER_HPP

#include "Image.hpp"
#include <memory>
#include <string>

class Filter {
public:
    virtual ~Filter() = default;
    
    // Main processing method
    virtual void apply(const Image& input, Image& output) = 0;
    
    // For UI display
    virtual std::string getName() const = 0;
    
    // For clone functionality (Prototype pattern)
    virtual std::unique_ptr<Filter> clone() const = 0;
    
    // GPU support query
    virtual bool supportsGPU() const { return false; }
    
    // Optional GPU implementation
    virtual void applyGPU(const Image& input, Image& output) {
        // Default fallback to CPU
        apply(input, output);
    }

     virtual double getLastExecutionTime() const { return 0.0; }
};

#endif // FILTER_HPP