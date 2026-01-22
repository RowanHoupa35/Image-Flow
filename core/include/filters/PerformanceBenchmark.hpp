#ifndef PERFORMANCE_BENCHMARK_HPP
#define PERFORMANCE_BENCHMARK_HPP

#include "../Filter.hpp"
#include <vector>
#include <chrono>
#include <string>
#include <memory>

struct BenchmarkResult {
    std::string filterName;
    std::string implementation;
    double executionTimeMs;
    double speedup;
    int threadCount;
};

class PerformanceBenchmark {
public:
    static BenchmarkResult compareCPUvsGPU(Filter& cpuFilter, Filter& gpuFilter, const Image& testImage);
    
    static void runAllBenchmarks(const Image& testImage);
    
private:
    static Image createTestImage(int width, int height);
};

#endif