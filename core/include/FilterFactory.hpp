/**
 * @file FilterFactory.hpp
 * @brief Factory and Registry pattern for dynamic filter creation
 *
 * This file implements the Singleton, Factory, and Registry design patterns
 * to enable dynamic filter instantiation without hardcoding dependencies.
 * It is a key component of the extensible architecture, allowing new filters
 * to be added without modifying GUI or CLI code.
 *
 * Design Patterns:
 * - Singleton: Single global instance via FilterFactory::instance()
 * - Factory: Creates filter objects without exposing instantiation logic
 * - Registry: Maintains a map of all available filters with metadata
 *
 * @see FilterRegistration.cpp for filter registration
 * @see Filter.hpp for the base filter interface
 *
 * @author Rowan HOUPA
 * @date January 2026
 */

#ifndef FILTER_FACTORY_HPP
#define FILTER_FACTORY_HPP

#include "Filter.hpp"
#include <memory>
#include <string>
#include <map>
#include <functional>
#include <vector>

/**
 * @class FilterFactory
 * @brief Factory for creating filters dynamically without hardcoding.
 *
 * This class implements the Factory and Registry patterns to allow
 * new filters to be added without modifying existing code.
 * Simply register a filter once, and it becomes available everywhere.
 */
class FilterFactory {
public:
    // Filter metadata for UI
    struct FilterInfo {
        std::string name;           
        std::string description;    
        bool hasGPUVersion;        
        bool hasParameters;         
        std::function<std::unique_ptr<Filter>()> createCPU; 
        std::function<std::unique_ptr<Filter>()> createGPU; 
    };

    static FilterFactory& instance() {
        static FilterFactory factory;
        return factory;
    }

    template<typename FilterType>
    void registerFilter(const std::string& id,
                       const std::string& displayName,
                       const std::string& description = "") {
        FilterInfo info;
        info.name = displayName;
        info.description = description;
        info.hasGPUVersion = false;
        info.hasParameters = false;
        info.createCPU = []() { return std::make_unique<FilterType>(); };
        info.createGPU = nullptr;

        filters[id] = info;
    }

    template<typename CPUFilterType, typename GPUFilterType>
    void registerFilterWithGPU(const std::string& id,
                               const std::string& displayName,
                               const std::string& description = "") {
        FilterInfo info;
        info.name = displayName;
        info.description = description;
        info.hasGPUVersion = true;
        info.hasParameters = false;
        info.createCPU = []() { return std::make_unique<CPUFilterType>(); };
        info.createGPU = []() { return std::make_unique<GPUFilterType>(); };

        filters[id] = info;
    }

    template<typename FilterType>
    void registerParameterizedFilter(
        const std::string& id,
        const std::string& displayName,
        const std::string& description,
        std::function<std::unique_ptr<Filter>()> creator) {

        FilterInfo info;
        info.name = displayName;
        info.description = description;
        info.hasGPUVersion = false;
        info.hasParameters = true;
        info.createCPU = creator;
        info.createGPU = nullptr;

        filters[id] = info;
    }

    template<typename CPUFilterType, typename GPUFilterType>
    void registerParameterizedFilterWithGPU(
        const std::string& id,
        const std::string& displayName,
        const std::string& description,
        std::function<std::unique_ptr<Filter>()> cpuCreator,
        std::function<std::unique_ptr<Filter>()> gpuCreator) {

        FilterInfo info;
        info.name = displayName;
        info.description = description;
        info.hasGPUVersion = true;
        info.hasParameters = true;
        info.createCPU = cpuCreator;
        info.createGPU = gpuCreator;

        filters[id] = info;
    }

    std::unique_ptr<Filter> create(const std::string& id, bool useGPU = false) const {
        auto it = filters.find(id);
        if (it == filters.end()) {
            return nullptr;
        }

        if (useGPU && it->second.createGPU) {
            return it->second.createGPU();
        }

        return it->second.createCPU();
    }

    std::vector<std::string> getFilterIds() const {
        std::vector<std::string> ids;
        for (const auto& pair : filters) {
            ids.push_back(pair.first);
        }
        return ids;
    }

    const FilterInfo* getFilterInfo(const std::string& id) const {
        auto it = filters.find(id);
        if (it == filters.end()) {
            return nullptr;
        }
        return &it->second;
    }

    bool hasFilter(const std::string& id) const {
        return filters.find(id) != filters.end();
    }

private:
    FilterFactory() = default;
    std::map<std::string, FilterInfo> filters;
};

#endif 