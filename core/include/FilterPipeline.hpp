/**
 * @file FilterPipeline.hpp
 * @brief Filter chain management for sequential image processing
 *
 * This file defines the FilterPipeline class which manages a sequence of
 * filters to be applied to an image. It demonstrates the Composite pattern
 * and uses smart pointers (std::unique_ptr) with move semantics.
 *
 * Key Features:
 * - Add/remove/reorder filters dynamically
 * - Apply all filters sequentially with progress tracking
 * - Support for CPU/GPU processing mode selection
 * - Pipeline serialization (save/load to JSON)
 * - Performance metrics collection
 *
 * @see Filter.hpp for the base filter interface
 * @see FilterFactory.hpp for filter creation
 *
 * @author Rowan HOUPA
 * @date January 2026
 */

#ifndef FILTER_PIPELINE_HPP
#define FILTER_PIPELINE_HPP

#include "Filter.hpp"
#include "Image.hpp"
#include <vector>
#include <memory>
#include <string>
#include <chrono>
#include <sstream>
#include <iomanip>

/**
 * @class FilterPipeline
 * @brief Manages a sequence of image filters (Composite Pattern).
 * 
 * This class allows chaining multiple filters and applying them
 * sequentially to an image. It's the core of the extensible architecture.
 */
class FilterPipeline {
public:
    FilterPipeline() = default;
    
    ~FilterPipeline() = default;
    FilterPipeline(const FilterPipeline& other);
    FilterPipeline& operator=(const FilterPipeline& other);
    FilterPipeline(FilterPipeline&&) = default;
    FilterPipeline& operator=(FilterPipeline&&) = default;
    
    void addFilter(std::unique_ptr<Filter> filter);
    void insertFilter(size_t index, std::unique_ptr<Filter> filter);
    void removeFilter(size_t index);
    void moveFilterUp(size_t index);
    void moveFilterDown(size_t index);
    void clear();
    
    Image apply(const Image& input) const;
    Image apply(Image&& input) const;
    
    template<typename ProgressCallback>
    Image applyWithProgress(const Image& input, ProgressCallback callback) const;
    
    size_t size() const { return filters.size(); }
    bool empty() const { return filters.empty(); }
    
    const Filter* getFilter(size_t index) const;
    Filter* getFilter(size_t index);
    
    std::string toString() const;
    bool fromString(const std::string& config);
    
    bool saveToFile(const std::string& filepath) const;
    bool loadFromFile(const std::string& filepath);
    
    std::string getDescription() const;

    struct PipelineMetrics {
        double totalTimeMs;
        std::vector<double> filterTimes;
        std::vector<std::string> filterNames;
        bool gpuUsed;
    };
    
    PipelineMetrics applyWithMetrics(const Image& input);
    
    enum class ProcessingMode { AUTO, CPU_ONLY, GPU_PREFERRED };
    void setProcessingMode(ProcessingMode mode) { processingMode = mode; }
    
private:
    std::vector<std::unique_ptr<Filter>> filters;
    
    std::unique_ptr<Filter> cloneFilter(const Filter* filter) const;
    ProcessingMode processingMode = ProcessingMode::AUTO;
};

template<typename ProgressCallback>
Image FilterPipeline::applyWithProgress(const Image& input, ProgressCallback callback) const {
    if (filters.empty()) {
        return input;
    }
    
    Image result = input;
    float progressStep = 100.0f / filters.size();
    
    for (size_t i = 0; i < filters.size(); ++i) {
        Image temp = std::move(result);
        filters[i]->apply(temp, result);
        
        callback((i + 1) * progressStep, filters[i]->getName());
    }
    
    return result;
}

#endif
