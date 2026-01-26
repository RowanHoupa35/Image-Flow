/**
 * @file Image.hpp
 * @brief Image data structure for pixel storage and manipulation
 *
 * This file defines the Image class which serves as the fundamental data
 * structure for all image processing operations. It provides:
 * - Pixel storage using std::vector<uint8_t> (STL container)
 * - File I/O via STB library (PNG, JPG, BMP, TGA)
 * - Bounds-checked pixel access via at(x, y, channel)
 * - SYCL buffer creation for GPU processing
 *
 * Memory Layout: Pixels are stored in row-major order as [R,G,B,R,G,B,...]
 * for RGB images, or [Y,Y,Y,...] for grayscale.
 *
 * @see Filter.hpp for image transformation interface
 * @see GrayscaleFilterGPU.hpp for SYCL buffer usage example
 *
 * @author Rowan HOUPA
 * @date January 2026
 */

#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <cstdint>
#include <vector>
#include <string>
#include <stdexcept>
#include <sycl/sycl.hpp> // For SYCL buffer compatibility

class Image {
public:
    Image() = default;
    Image(int width, int height, int channels = 3);
    
    bool loadFromFile(const std::string& filepath);
    bool saveToFile(const std::string& filepath) const;
    
    int getWidth() const { return m_width; }
    int getHeight() const { return m_height; }
    int getChannels() const { return m_channels; }
    size_t size() const { return m_pixels.size(); }
    
    uint8_t* data() { return m_pixels.data(); }
    const uint8_t* data() const { return m_pixels.data(); }
    
    uint8_t& at(int x, int y, int channel);
    const uint8_t& at(int x, int y, int channel) const;
    
    sycl::buffer<uint8_t, 1> createSyclBuffer() {
        return sycl::buffer<uint8_t, 1>(m_pixels.data(), sycl::range<1>(m_pixels.size()));
    }
    
    Image createEmptyLike() const {
        return Image(m_width, m_height, m_channels);
    }

private:
    int m_width = 0;
    int m_height = 0;
    int m_channels = 3; // Default: RGB
    std::vector<uint8_t> m_pixels;
    
    static void stbiWriteFunc(void* context, void* data, int size);
};

#endif
