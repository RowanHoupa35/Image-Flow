#include "Image.hpp"

// Disable warnings from STB implementation
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
#pragma GCC diagnostic ignored "-Wunused-variable"
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <stb/stb_image_write.h>
#pragma GCC diagnostic pop

Image::Image(int width, int height, int channels) 
    : m_width(width), m_height(height), m_channels(channels) {
    if (width <= 0 || height <= 0 || channels <= 0) {
        throw std::invalid_argument("Image dimensions must be positive");
    }
    m_pixels.resize(width * height * channels);
}

bool Image::loadFromFile(const std::string& filepath) {
    int width, height, channels;
    unsigned char* data = stbi_load(filepath.c_str(), &width, &height, &channels, 0);
    
    if (!data) return false;
    
    m_width = width;
    m_height = height;
    m_channels = channels;
    m_pixels.assign(data, data + width * height * channels);
    
    stbi_image_free(data);
    return true;
}

bool Image::saveToFile(const std::string& filepath) const {
    std::string ext = filepath.substr(filepath.find_last_of(".") + 1);
    
    if (ext == "png") {
        return stbi_write_png(filepath.c_str(), m_width, m_height, m_channels, 
                             m_pixels.data(), m_width * m_channels);
    } else if (ext == "jpg" || ext == "jpeg") {
        return stbi_write_jpg(filepath.c_str(), m_width, m_height, m_channels, 
                             m_pixels.data(), 90); // 90% quality
    } else if (ext == "bmp") {
        return stbi_write_bmp(filepath.c_str(), m_width, m_height, m_channels, 
                             m_pixels.data());
    }
    
    return false;
}

uint8_t& Image::at(int x, int y, int channel) {
    if (x < 0 || x >= m_width || y < 0 || y >= m_height || channel < 0 || channel >= m_channels) {
        throw std::out_of_range("Image::at: index out of range");
    }
    return m_pixels[(y * m_width + x) * m_channels + channel];
}

const uint8_t& Image::at(int x, int y, int channel) const {
    // Reuse non-const version to avoid code duplication
    return const_cast<Image*>(this)->at(x, y, channel);
}

void Image::stbiWriteFunc(void* context, void* data, int size) {
    (void)context;
    (void)data;
    (void)size;
}