#include "filters/GrayscaleFilterGPU.hpp"
#include "filters/GrayscaleFilter.hpp"
#include <iostream>

void GrayscaleFilterGPU::apply(const Image& input, Image& output) {
    output = Image(input.getWidth(), input.getHeight(), 1);
    
    auto start = std::chrono::high_resolution_clock::now();
    
    try {
        // Créer une queue SYCL (GPU)
        sycl::queue q(sycl::gpu_selector_v);
        
        std::cout << " Exécution GPU sur: " 
                  << q.get_device().get_info<sycl::info::device::name>() 
                  << std::endl;
        
        int width = input.getWidth();
        int height = input.getHeight();
        int totalPixels = width * height;
        
        // Buffers SYCL
        sycl::buffer<uint8_t, 1> bufIn(input.data(), sycl::range<1>(input.size()));
        sycl::buffer<uint8_t, 1> bufOut(output.data(), sycl::range<1>(output.size()));
        
        // Kernel de conversion en niveaux de gris
        q.submit([&](sycl::handler& h) {
            auto accIn = bufIn.get_access<sycl::access::mode::read>(h);
            auto accOut = bufOut.get_access<sycl::access::mode::write>(h);
            
            h.parallel_for(sycl::range<1>(totalPixels), [=](sycl::id<1> idx) {
                int i = idx[0];
                int srcIdx = i * 3; // RGB
                
                uint8_t r = accIn[srcIdx];
                uint8_t g = accIn[srcIdx + 1];
                uint8_t b = accIn[srcIdx + 2];
                
                // Formule de luminance
                uint8_t gray = static_cast<uint8_t>(
                    0.299f * r + 0.587f * g + 0.114f * b
                );
                
                accOut[i] = gray;
            });
        }).wait();
        
        auto end = std::chrono::high_resolution_clock::now();
        lastExecutionTime = std::chrono::duration<double, std::milli>(end - start).count();
        
        std::cout << "GPU terminé en " << lastExecutionTime << " ms" << std::endl;
        
    } catch (sycl::exception const& e) {
        std::cerr << "SYCL exception: " << e.what() << std::endl;
        std::cerr << "↩Fallback sur CPU..." << std::endl;
        
        // Fallback CPU
        GrayscaleFilter cpuFallback;
        cpuFallback.apply(input, output);
        
        auto end = std::chrono::high_resolution_clock::now();
        lastExecutionTime = std::chrono::duration<double, std::milli>(end - start).count();
    }
}