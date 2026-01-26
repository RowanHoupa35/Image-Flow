/**
 * @file BoxBlurFilterGPU.cpp
 * @brief GPU-accelerated box blur implementation using SYCL
 *
 * Performs box blur (averaging) on GPU using Intel oneAPI SYCL.
 * Each pixel is computed by a separate GPU work-item, enabling massive
 * parallelism for the convolution operation.
 *
 * @details
 * SYCL Implementation:
 * - One work-item per pixel (width * height total)
 * - Each work-item computes the average of its (2*radius+1)² neighborhood
 * - Uses temporary vectors to avoid buffer synchronization issues
 * - Final result copied back to output image
 *
 * Memory Management:
 * - Input/output copied to intermediate vectors before GPU transfer
 * - SYCL buffers manage CPU-GPU memory automatically
 * - Uses scope-based buffer destruction for synchronization
 *
 * Error Handling:
 * - Catches sycl::exception for GPU failures
 * - Falls back to CPU BoxBlurFilter if GPU unavailable
 * - Always completes processing regardless of GPU status
 *
 * Performance Notes:
 * - Compute-bound due to O(radius²) work per pixel
 * - Benefits from discrete GPUs more than grayscale filter
 * - Memory transfer overhead may dominate for small images
 *
 * @see BoxBlurFilter.cpp for CPU version
 * @author Rowan HOUPA
 * @date January 2026
 */

#include "filters/BoxBlurFilterGPU.hpp"
#include "filters/BoxBlurFilter.hpp"
#include <iostream>
#include <cstring>

void BoxBlurFilterGPU::apply(const Image& input, Image& output) {
    output = Image(input.getWidth(), input.getHeight(), input.getChannels());
    
    auto start = std::chrono::high_resolution_clock::now();
    
    try {
        sycl::queue q(sycl::gpu_selector_v);
        
        std::cout << "BoxBlur GPU sur: " 
                  << q.get_device().get_info<sycl::info::device::name>() 
                  << std::endl;
        
        const int width = input.getWidth();
        const int height = input.getHeight();
        const int channels = input.getChannels();
        const int radius = blurRadius;
        
        std::vector<uint8_t> inputData(input.data(), input.data() + input.size());
        std::vector<uint8_t> outputData(output.size());
        
        {
            sycl::buffer<uint8_t, 1> bufIn(inputData.data(), sycl::range<1>(inputData.size()));
            sycl::buffer<uint8_t, 1> bufOut(outputData.data(), sycl::range<1>(outputData.size()));
            
            q.submit([&](sycl::handler& h) {
                auto accIn = bufIn.get_access<sycl::access::mode::read>(h);
                auto accOut = bufOut.get_access<sycl::access::mode::write>(h);
                
                h.parallel_for(sycl::range<1>(width * height), [=](sycl::id<1> idx) {
                    const int i = idx[0];
                    const int y = i / width;
                    const int x = i % width;
                    
                    for (int c = 0; c < channels; c++) {
                        int sum = 0;
                        int count = 0;
                        
                        int yStart = (y - radius < 0) ? 0 : y - radius;
                        int yEnd = (y + radius >= height) ? height - 1 : y + radius;
                        int xStart = (x - radius < 0) ? 0 : x - radius;
                        int xEnd = (x + radius >= width) ? width - 1 : x + radius;
                        
                        for (int ny = yStart; ny <= yEnd; ny++) {
                            for (int nx = xStart; nx <= xEnd; nx++) {
                                int srcIdx = (ny * width + nx) * channels + c;
                                sum += accIn[srcIdx];
                                count++;
                            }
                        }
                        
                        int avg = sum / count;
                        int dstIdx = i * channels + c;
                        accOut[dstIdx] = (avg > 255) ? 255 : ((avg < 0) ? 0 : avg);
                    }
                });
            }).wait();
        }
        
        std::memcpy(output.data(), outputData.data(), outputData.size());
        
        auto end = std::chrono::high_resolution_clock::now();
        lastExecutionTime = std::chrono::duration<double, std::milli>(end - start).count();
        
        std::cout << "GPU Blur terminé en " << lastExecutionTime << " ms" << std::endl;
        
    } catch (sycl::exception const& e) {
        std::cerr << "SYCL exception: " << e.what() << std::endl;
        std::cerr << "↩Fallback sur CPU..." << std::endl;
        
        BoxBlurFilter cpuFallback(blurRadius);
        cpuFallback.apply(input, output);
        
        auto end = std::chrono::high_resolution_clock::now();
        lastExecutionTime = std::chrono::duration<double, std::milli>(end - start).count();
    }
}