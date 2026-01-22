#include "filters/BoxBlurFilterGPU.hpp"
#include "filters/BoxBlurFilter.hpp"
#include <iostream>
#include <cstring>

void BoxBlurFilterGPU::apply(const Image& input, Image& output) {
    output = Image(input.getWidth(), input.getHeight(), input.getChannels());
    
    auto start = std::chrono::high_resolution_clock::now();
    
    try {
        sycl::queue q(sycl::gpu_selector_v);
        
        std::cout << "🖥️  BoxBlur GPU sur: " 
                  << q.get_device().get_info<sycl::info::device::name>() 
                  << std::endl;
        
        const int width = input.getWidth();
        const int height = input.getHeight();
        const int channels = input.getChannels();
        const int radius = blurRadius;
        
        // Copie temporaire pour éviter les problèmes de concurrence
        std::vector<uint8_t> inputData(input.data(), input.data() + input.size());
        std::vector<uint8_t> outputData(output.size());
        
        {
            sycl::buffer<uint8_t, 1> bufIn(inputData.data(), sycl::range<1>(inputData.size()));
            sycl::buffer<uint8_t, 1> bufOut(outputData.data(), sycl::range<1>(outputData.size()));
            
            q.submit([&](sycl::handler& h) {
                auto accIn = bufIn.get_access<sycl::access::mode::read>(h);
                auto accOut = bufOut.get_access<sycl::access::mode::write>(h);
                
                // Kernel ultra-simplifié : un thread par pixel
                h.parallel_for(sycl::range<1>(width * height), [=](sycl::id<1> idx) {
                    const int i = idx[0];
                    const int y = i / width;
                    const int x = i % width;
                    
                    // Traiter chaque canal séquentiellement dans le thread
                    for (int c = 0; c < channels; c++) {
                        int sum = 0;
                        int count = 0;
                        
                        // Convolution simplifiée
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
        
        // Copier le résultat
        std::memcpy(output.data(), outputData.data(), outputData.size());
        
        auto end = std::chrono::high_resolution_clock::now();
        lastExecutionTime = std::chrono::duration<double, std::milli>(end - start).count();
        
        std::cout << "✅ GPU Blur terminé en " << lastExecutionTime << " ms" << std::endl;
        
    } catch (sycl::exception const& e) {
        std::cerr << "❌ SYCL exception: " << e.what() << std::endl;
        std::cerr << "↩️  Fallback sur CPU..." << std::endl;
        
        BoxBlurFilter cpuFallback(blurRadius);
        cpuFallback.apply(input, output);
        
        auto end = std::chrono::high_resolution_clock::now();
        lastExecutionTime = std::chrono::duration<double, std::milli>(end - start).count();
    }
}