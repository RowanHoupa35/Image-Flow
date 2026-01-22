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
        std::string name;           // Display name (e.g., "Grayscale")
        std::string description;    // Short description
        bool hasGPUVersion;         // Does this filter have GPU support?
        bool hasParameters;         // Does this filter have adjustable parameters?
        std::function<std::unique_ptr<Filter>()> createCPU;  // CPU version creator
        std::function<std::unique_ptr<Filter>()> createGPU;  // GPU version creator (optional)
    };

    /**
     * @brief Get the singleton instance
     */
    static FilterFactory& instance() {
        static FilterFactory factory;
        return factory;
    }

    /**
     * @brief Register a filter (CPU only)
     */
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

    /**
     * @brief Register a filter with CPU and GPU versions
     */
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

    /**
     * @brief Register a parameterized filter (CPU only)
     */
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

    /**
     * @brief Register a parameterized filter with GPU version
     */
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

    /**
     * @brief Create a filter instance by ID
     */
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

    /**
     * @brief Get all registered filter IDs
     */
    std::vector<std::string> getFilterIds() const {
        std::vector<std::string> ids;
        for (const auto& pair : filters) {
            ids.push_back(pair.first);
        }
        return ids;
    }

    /**
     * @brief Get filter metadata
     */
    const FilterInfo* getFilterInfo(const std::string& id) const {
        auto it = filters.find(id);
        if (it == filters.end()) {
            return nullptr;
        }
        return &it->second;
    }

    /**
     * @brief Check if a filter exists
     */
    bool hasFilter(const std::string& id) const {
        return filters.find(id) != filters.end();
    }

private:
    FilterFactory() = default;
    std::map<std::string, FilterInfo> filters;
};

#endif // FILTER_FACTORY_HPP
