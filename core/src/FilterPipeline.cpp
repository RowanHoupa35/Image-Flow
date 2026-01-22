#include "FilterPipeline.hpp"
#include "filters/GrayscaleFilter.hpp"
#include "filters/InvertFilter.hpp"
#include "filters/BrightnessFilter.hpp"
#include "filters/BoxBlurFilter.hpp"

#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <memory>

// === Constructor & Assignment Operators ===

FilterPipeline::FilterPipeline(const FilterPipeline& other) {
    // Deep copy all filters
    for (const auto& filter : other.filters) {
        filters.push_back(filter->clone());
    }
}

FilterPipeline& FilterPipeline::operator=(const FilterPipeline& other) {
    if (this != &other) {
        filters.clear();
        for (const auto& filter : other.filters) {
            filters.push_back(filter->clone());
        }
    }
    return *this;
}

// === Filter Management ===

void FilterPipeline::addFilter(std::unique_ptr<Filter> filter) {
    if (!filter) {
        throw std::invalid_argument("FilterPipeline::addFilter: null filter");
    }
    filters.push_back(std::move(filter));
}

void FilterPipeline::insertFilter(size_t index, std::unique_ptr<Filter> filter) {
    if (index > filters.size()) {
        throw std::out_of_range("FilterPipeline::insertFilter: index out of range");
    }
    if (!filter) {
        throw std::invalid_argument("FilterPipeline::insertFilter: null filter");
    }
    filters.insert(filters.begin() + index, std::move(filter));
}

void FilterPipeline::removeFilter(size_t index) {
    if (index >= filters.size()) {
        throw std::out_of_range("FilterPipeline::removeFilter: index out of range");
    }
    filters.erase(filters.begin() + index);
}

void FilterPipeline::moveFilterUp(size_t index) {
    if (index == 0 || index >= filters.size()) return;
    
    std::swap(filters[index], filters[index - 1]);
}

void FilterPipeline::moveFilterDown(size_t index) {
    if (index >= filters.size() - 1) return;
    
    std::swap(filters[index], filters[index + 1]);
}

void FilterPipeline::clear() {
    filters.clear();
}

// === Core Processing ===

Image FilterPipeline::apply(const Image& input) const {
    if (filters.empty()) {
        return input; // No filters, return copy of input
    }
    
    Image result = input; // Copy constructor
    Image temp; // Reusable temporary
    
    for (const auto& filter : filters) {
        temp = std::move(result);    // Move result to temp
        filter->apply(temp, result); // Process: temp -> result
    }
    
    return result;
}

Image FilterPipeline::apply(Image&& input) const {
    if (filters.empty()) {
        return std::move(input); // No filters, return input (moved)
    }
    
    Image result = std::move(input); // Move input to result
    Image temp;
    
    for (const auto& filter : filters) {
        temp = std::move(result);    // Move result to temp
        filter->apply(temp, result); // Process: temp -> result
    }
    
    return result;
}

// === Accessors ===

const Filter* FilterPipeline::getFilter(size_t index) const {
    if (index >= filters.size()) {
        throw std::out_of_range("FilterPipeline::getFilter: index out of range");
    }
    return filters[index].get();
}

Filter* FilterPipeline::getFilter(size_t index) {
    // Reuse const version to avoid code duplication
    return const_cast<Filter*>(static_cast<const FilterPipeline*>(this)->getFilter(index));
}

// === Serialization ===

std::string FilterPipeline::toString() const {
    std::ostringstream oss;
    oss << "FilterPipeline[" << filters.size() << "]:\n";
    
    for (size_t i = 0; i < filters.size(); ++i) {
        oss << "  " << (i + 1) << ". " << filters[i]->getName() << "\n";
    }
    
    return oss.str();
}

std::string FilterPipeline::getDescription() const {
    if (filters.empty()) {
        return "Empty pipeline";
    }
    
    std::ostringstream oss;
    oss << filters.size() << " filter(s): ";
    
    for (size_t i = 0; i < filters.size(); ++i) {
        if (i > 0) oss << " → ";
        oss << filters[i]->getName();
    }
    
    return oss.str();
}

// === File I/O (Simple JSON-like format) ===

bool FilterPipeline::saveToFile(const std::string& filepath) const {
    std::ofstream file(filepath);
    if (!file.is_open()) {
        return false;
    }
    
    file << "{\n";
    file << "  \"pipeline\": [\n";
    
    for (size_t i = 0; i < filters.size(); ++i) {
        file << "    {\n";
        file << "      \"name\": \"" << filters[i]->getName() << "\",\n";
        
        // Safe way to get type name
        const Filter& filterRef = *filters[i];
        file << "      \"type\": \"" << typeid(filterRef).name() << "\"\n";
        
        file << "    }";
        if (i < filters.size() - 1) file << ",";
        file << "\n";
    }
    
    file << "  ]\n";
    file << "}\n";
    
    return file.good();
}

bool FilterPipeline::loadFromFile(const std::string& filepath) {
    // Simplified implementation - in reality you'd parse JSON
    // and create appropriate filter instances
    std::ifstream file(filepath);
    if (!file.is_open()) {
        return false;
    }
    
    // For now, just clear and add default filters as example
    // You would implement proper JSON parsing here
    filters.clear();
    
    // Example: Add some default filters
    filters.push_back(std::make_unique<GrayscaleFilter>());
    filters.push_back(std::make_unique<BrightnessFilter>(1.2f));
    
    return true;
}

// === Helper ===

std::unique_ptr<Filter> FilterPipeline::cloneFilter(const Filter* filter) const {
    if (!filter) return nullptr;
    return filter->clone();
}