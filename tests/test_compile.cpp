/**
 * @file test_compile.cpp
 * @brief Compilation test for stb_image header-only library
 *
 * Minimal test to verify that stb_image.h is found and compiles
 * correctly. Prints the library version to confirm successful include.
 *
 * @details
 * Purpose: Verify external dependency (stb_image) is accessible
 *
 * stb_image Library:
 * - Single-header public domain image loading library
 * - Written by Sean Barrett (nothings.org)
 * - Supports PNG, JPG, BMP, GIF, TGA, PSD, HDR, PIC
 *
 * Expected Output:
 * "STB test: <version number>"
 *
 * @note This is a compile-time test, not a runtime functionality test
 * @see Image.cpp for actual stb_image usage
 * @author ENSTA Paris - IN204 Project
 * @date January 2026
 */

#include <iostream>
#include "external/stb_image.h"

int main() {
    std::cout << "STB test: " << STB_IMAGE_VERSION << std::endl;
    return 0;
}