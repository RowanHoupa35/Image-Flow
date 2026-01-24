/**
 * @file test_gpu.cpp
 * @brief SYCL GPU availability and functionality test
 *
 * Verifies that SYCL can access a GPU device and execute a simple kernel.
 * Essential first test before running GPU-accelerated filters.
 *
 * @details
 * Test Procedure:
 * 1. Request GPU device via sycl::gpu_selector_v
 * 2. Print detected device name
 * 3. Run simple vector addition kernel: c[i] = a[i] + b[i]
 * 4. Verify result: c[0] should equal 3 (1 + 2)
 *
 * SYCL Components Tested:
 * - sycl::queue: Command queue creation
 * - sycl::gpu_selector_v: GPU device selection
 * - sycl::buffer: Memory transfer
 * - sycl::parallel_for: Kernel execution
 *
 * Possible Outcomes:
 * - "RÉUSSI": GPU works correctly with SYCL
 * - "ÉCHEC": Kernel executed but wrong result
 * - Exception: No GPU available or driver issue
 *
 * @note Requires Intel oneAPI DPC++ compiler and compatible GPU
 * @author ENSTA Paris - IN204 Project
 * @date January 2026
 */

#include <sycl/sycl.hpp>
#include <iostream>

int main() {
    try {
        sycl::queue q(sycl::gpu_selector_v);
        
        std::cout << "Device: " 
                  << q.get_device().get_info<sycl::info::device::name>() 
                  << std::endl;
        
        // Test simple : addition vectorielle
        const int N = 1000;
        std::vector<int> a(N, 1);
        std::vector<int> b(N, 2);
        std::vector<int> c(N, 0);
        
        {
            sycl::buffer<int, 1> buf_a(a.data(), sycl::range<1>(N));
            sycl::buffer<int, 1> buf_b(b.data(), sycl::range<1>(N));
            sycl::buffer<int, 1> buf_c(c.data(), sycl::range<1>(N));
            
            q.submit([&](sycl::handler& h) {
                auto acc_a = buf_a.get_access<sycl::access::mode::read>(h);
                auto acc_b = buf_b.get_access<sycl::access::mode::read>(h);
                auto acc_c = buf_c.get_access<sycl::access::mode::write>(h);
                
                h.parallel_for(sycl::range<1>(N), [=](sycl::id<1> idx) {
                    acc_c[idx] = acc_a[idx] + acc_b[idx];
                });
            }).wait();
        }
        
        std::cout << "Test SYCL GPU : " 
                  << (c[0] == 3 ? "✓ RÉUSSI" : "✗ ÉCHEC") 
                  << std::endl;
        
        return 0;
        
    } catch (sycl::exception const& e) {
        std::cerr << "SYCL exception: " << e.what() << std::endl;
        return 1;
    }
}