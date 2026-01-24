/**
 * @file Filter.hpp
 * @brief Abstract base class for all image filters (Polymorphism core)
 *
 * This file defines the Filter interface that all concrete filters must implement.
 * It is the foundation of the polymorphic architecture, enabling:
 * - Runtime filter selection via virtual dispatch
 * - Easy addition of new filters without modifying existing code (Open/Closed Principle)
 * - Uniform treatment of all filters through the base class pointer
 *
 * Key methods:
 * - apply(): Pure virtual method for image processing (must be overridden)
 * - getName(): Returns filter display name for UI
 * - clone(): Prototype pattern for filter duplication
 * - supportsGPU(): Query GPU acceleration availability
 *
 * @see FilterFactory for dynamic filter creation
 * @see FilterPipeline for chaining multiple filters
 *
 * @author Rowan HOUPA
 * @date January 2026
 */

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