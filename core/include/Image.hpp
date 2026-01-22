#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <cstdint>
#include <vector>
#include <string>
#include <stdexcept>
#include <sycl/sycl.hpp> // For SYCL buffer compatibility

class Image {
public:
    // Constructors
    Image() = default;
    Image(int width, int height, int channels = 3);
    
    // Load/Save using STB
    bool loadFromFile(const std::string& filepath);
    bool saveToFile(const std::string& filepath) const;
    
    // Accessors
    int getWidth() const { return m_width; }
    int getHeight() const { return m_height; }
    int getChannels() const { return m_channels; }
    size_t size() const { return m_pixels.size(); }
    
    // Raw pixel data access (for CPU filters)
    uint8_t* data() { return m_pixels.data(); }
    const uint8_t* data() const { return m_pixels.data(); }
    
    // Pixel access with bounds checking
    uint8_t& at(int x, int y, int channel);
    const uint8_t& at(int x, int y, int channel) const;
    
    // SYCL buffer creation (for GPU filters)
    sycl::buffer<uint8_t, 1> createSyclBuffer() {
        return sycl::buffer<uint8_t, 1>(m_pixels.data(), sycl::range<1>(m_pixels.size()));
    }
    
    // Create an empty image with same dimensions
    Image createEmptyLike() const {
        return Image(m_width, m_height, m_channels);
    }

private:
    int m_width = 0;
    int m_height = 0;
    int m_channels = 3; // Default: RGB
    std::vector<uint8_t> m_pixels;
    
    // STB callbacks must use C linkage
    static void stbiWriteFunc(void* context, void* data, int size);
};

#endif // IMAGE_HPP