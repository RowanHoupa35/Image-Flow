/**
 * @file test_filters.cpp
 * @brief Unit tests for individual filter implementations
 *
 * Tests each filter in isolation by applying it to a synthetic gradient
 * image and saving the output for visual verification.
 *
 * @details
 * Test Image:
 * - Size: 400x300 pixels, RGB
 * - Pattern: Red gradient horizontal, green gradient vertical, blue constant
 *
 * Filters Tested:
 * 1. InvertFilter: Creates photographic negative
 * 2. BrightnessFilter: 50% brightness increase (factor 1.5)
 * 3. BoxBlurFilter: Blur with radius 3
 * 4. Filter chain: Invert followed by blur
 *
 * Output Files:
 * - test_inverted.png: Inversion result
 * - test_brightened.png: Brightness result
 * - test_blurred.png: Blur result
 * - test_chain.png: Combined filter result
 *
 * @note Visual inspection required - no automated pass/fail
 * @author ENSTA Paris - IN204 Project
 * @date January 2026
 */

#include "Image.hpp"
#include "filters/InvertFilter.hpp"
#include "filters/BrightnessFilter.hpp"
#include "filters/BoxBlurFilter.hpp"
#include <iostream>

int main() {
    // Create test image
    Image img(400, 300, 3);
    for (int y = 0; y < img.getHeight(); ++y) {
        for (int x = 0; x < img.getWidth(); ++x) {
            img.at(x, y, 0) = static_cast<uint8_t>(x * 255 / img.getWidth());
            img.at(x, y, 1) = static_cast<uint8_t>(y * 255 / img.getHeight());
            img.at(x, y, 2) = 150;
        }
    }
    
    // Test InvertFilter
    InvertFilter invert;
    Image inverted;
    invert.apply(img, inverted);
    inverted.saveToFile("test_inverted.png");
    std::cout << "Created inverted.png" << std::endl;
    
    // Test BrightnessFilter
    BrightnessFilter brighter(1.5f); // 50% brighter
    Image brightened;
    brighter.apply(img, brightened);
    brightened.saveToFile("test_brightened.png");
    std::cout << "Created brightened.png" << std::endl;
    
    // Test BoxBlurFilter
    BoxBlurFilter blur(3); // Radius 3
    Image blurred;
    blur.apply(img, blurred);
    blurred.saveToFile("test_blurred.png");
    std::cout << "Created blurred.png" << std::endl;
    
    // Test filter chain
    Image final = img;
    invert.apply(final, final);  // Invert
    blur.apply(final, final);    // Then blur
    final.saveToFile("test_chain.png");
    std::cout << "Created chain.png (invert → blur)" << std::endl;
    
    return 0;
}
