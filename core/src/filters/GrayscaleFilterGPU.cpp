/**
 * @file GrayscaleFilterGPU.cpp
 * @brief GPU implementation of grayscale conversion using SYCL
 *
 * Converts RGB images to grayscale using parallel GPU execution via Intel
 * oneAPI SYCL. Each pixel is processed by a separate GPU work-item, enabling
 * massive parallelism on compatible hardware.
 *
 * @details
 * SYCL Components Used:
 * - sycl::queue: Command queue bound to GPU device
 * - sycl::gpu_selector_v: Automatic GPU device selection
 * - sycl::buffer: Memory containers for CPU-GPU data transfer
 * - sycl::parallel_for: Kernel launch for each pixel
 * - sycl::handler: Command group handler for kernel submission
 *
 * Algorithm: Same as CPU version (0.299*R + 0.587*G + 0.114*B)
 *
 * Error Handling:
 * - Catches sycl::exception for GPU failures
 * - Falls back to CPU (GrayscaleFilter) if GPU unavailable
 * - Ensures processing always completes even without GPU
 *
 * Performance Notes:
 * - Best on discrete GPUs (NVIDIA, AMD, Intel Arc)
 * - May be slower on integrated GPUs due to memory transfer overhead
 * - Execution time measured and stored for benchmarking
 *
 * @see GrayscaleFilter.cpp for CPU version
 * @author Rowan HOUPA
 * @date January 2026
 */

#include "filters/GrayscaleFilterGPU.hpp"
#include "filters/GrayscaleFilter.hpp"
#include <iostream>

void GrayscaleFilterGPU::apply(const Image& input, Image& output) {
    output = Image(input.getWidth(), input.getHeight(), 1);
    
    auto start = std::chrono::high_resolution_clock::now();
    
    try {
        sycl::queue q(sycl::gpu_selector_v);
        
        std::cout << " Exécution GPU sur: " 
                  << q.get_device().get_info<sycl::info::device::name>() 
                  << std::endl;
        
        int width = input.getWidth();
        int height = input.getHeight();
        int totalPixels = width * height;
        
        sycl::buffer<uint8_t, 1> bufIn(input.data(), sycl::range<1>(input.size()));
        sycl::buffer<uint8_t, 1> bufOut(output.data(), sycl::range<1>(output.size()));
        
        q.submit([&](sycl::handler& h) {
            auto accIn = bufIn.get_access<sycl::access::mode::read>(h);
            auto accOut = bufOut.get_access<sycl::access::mode::write>(h);
            
            h.parallel_for(sycl::range<1>(totalPixels), [=](sycl::id<1> idx) {
                int i = idx[0];
                int srcIdx = i * 3;
                
                uint8_t r = accIn[srcIdx];
                uint8_t g = accIn[srcIdx + 1];
                uint8_t b = accIn[srcIdx + 2];
                
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
        
        GrayscaleFilter cpuFallback;
        cpuFallback.apply(input, output);
        
        auto end = std::chrono::high_resolution_clock::now();
        lastExecutionTime = std::chrono::duration<double, std::milli>(end - start).count();
    }
}
