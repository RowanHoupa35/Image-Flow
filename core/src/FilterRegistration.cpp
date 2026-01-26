/**
 * @file FilterRegistration.cpp
 * @brief Central registration point for all image filters
 *
 * This file registers all available filters with the FilterFactory singleton.
 * Using static initialization,filters are registered before main() executes,
 * making them immediately available to both GUI and CLI interfaces.
 *
 * @details
 * Plugin Architecture:
 * To add a new filter to ImageFlow:
 * 1. Create your filter class inheriting from Filter (in filters/ directory)
 * 2. Include the header here
 * 3. Add a factory.registerFilter<YourFilter>(...) call
 * 4. Rebuild - the GUI will automatically display the new filter!
 *
 * Registration Methods:
 * - registerFilter<T>(): Basic CPU-only filter
 * - registerFilterWithGPU<CPU, GPU>(): Filter with CPU and GPU variants
 * - registerParameterizedFilter<T>(): Filter with constructor parameters
 * - registerParameterizedFilterWithGPU<CPU, GPU>(): Parameterized with GPU variant
 *
 * Static Initialization:
 * The FilterRegistrar struct uses the "static initialization" idiom to
 * ensure registerAllFilters() runs before main(). This is a common C++
 * pattern for plugin/module registration.
 *
 * @see FilterFactory.hpp for the factory implementation
 * @author Rowan HOUPA
 * @date January 2026
 */

#include "FilterFactory.hpp"
#include "filters/GrayscaleFilter.hpp"
#include "filters/GrayscaleFilterGPU.hpp"
#include "filters/InvertFilter.hpp"
#include "filters/BrightnessFilter.hpp"
#include "filters/BoxBlurFilter.hpp"
#include "filters/BoxBlurFilterGPU.hpp"
#include "filters/SepiaFilter.hpp"
#include <iostream>

/**
 * @brief Registers all available filters with the FilterFactory.
 *
 * To add a new filter:
 * 1. Create your filter class inheriting from Filter
 * 2. Add a registration line below
 * 3. Rebuild - that's it! The GUI will automatically show it.
 *
 * No need to modify the GUI code!
 */
void registerAllFilters() {
    std::cout << "========== REGISTERING FILTERS ==========" << std::endl;
    auto& factory = FilterFactory::instance();

    // Grayscale filter
    factory.registerFilterWithGPU<GrayscaleFilter, GrayscaleFilterGPU>(
        "grayscale",
        "Niveaux de Gris",
        "Convertit l'image en niveaux de gris"
    );

    // Invert filter
    factory.registerFilter<InvertFilter>(
        "invert",
        "Inverser",
        "Inverse les couleurs de l'image"
    );

    // Brightness filter
    factory.registerParameterizedFilter<BrightnessFilter>(
        "brightness",
        "Luminosité",
        "Ajuste la luminosité de l'image",
        []() { return std::make_unique<BrightnessFilter>(1.0f); }
    );

    // Box Blur filter
    factory.registerParameterizedFilterWithGPU<BoxBlurFilter, BoxBlurFilterGPU>(
        "boxblur",
        "Flou",
        "Applique un flou à l'image",
        []() { return std::make_unique<BoxBlurFilter>(2); },
        []() { return std::make_unique<BoxBlurFilterGPU>(2); }
    );

    // Sepia filter
    factory.registerFilter<SepiaFilter>(
        "sepia",
        "Ton Sépia",
        "Applique un effet ton sépia vintage"
    );

    std::cout << "Total filters registered: " << factory.getFilterIds().size() << std::endl;
    std::cout << "========== REGISTRATION COMPLETE ==========" << std::endl;
}
namespace {
    struct FilterRegistrar {
        FilterRegistrar() {
            registerAllFilters();
        }
    };
    
    static FilterRegistrar registrar;
}
