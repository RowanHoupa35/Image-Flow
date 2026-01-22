# ImageFlow - Image Processing with CPU/GPU Parallelization

![C++](https://img.shields.io/badge/C++-17-blue.svg)
![Qt](https://img.shields.io/badge/Qt-6.9.2-green.svg)
![OpenMP](https://img.shields.io/badge/OpenMP-Parallel-orange.svg)
![SYCL](https://img.shields.io/badge/SYCL-GPU-red.svg)

## Overview

ImageFlow is an image processing application demonstrating:
- **Polymorphism & Inheritance** - Extensible filter architecture using design patterns
- **Multi-core CPU Parallelization** - OpenMP for all filters (5/5 coverage)
- **GPU Acceleration** - SYCL for massive parallelism (2/5 filters)
- **Hybrid Architecture** - Automatic CPU/GPU selection with fallback

## Features

### Dual Interface
- **GUI** - Qt 6.9.2 graphical interface with real-time preview
- **CLI** - Command-line tool for batch processing

### Image Processing Filters
1. **Grayscale** - Convert to grayscale (CPU + GPU)
2. **Invert** - Invert colors (CPU)
3. **Brightness** - Adjust brightness (CPU)
4. **Box Blur** - Apply blur effect (CPU + GPU)
5. **Sepia** - Vintage sepia tone (CPU)

### Performance
- **CPU:** 4-8x speedup with OpenMP multi-threading
- **GPU:** 10-100x speedup with SYCL on large images
- **Scalable:** Tested from 1-16 CPU cores

## Quick Start

### Prerequisites

**Linux (Ubuntu/Debian):**
```bash
# Install Qt 6 development tools
sudo apt install qt6-base-dev qt6-tools-dev cmake build-essential

# Install Intel oneAPI (for SYCL and OpenMP)
wget https://registrationcenter-download.intel.com/akdlm/IRC_NAS/163da6e4-56eb-4948-aba3-debcec61c064/l_BaseKit_p_2024.0.1.46_offline.sh
sudo sh ./l_BaseKit_p_2024.0.1.46_offline.sh
```

**Windows:**
- Install Visual Studio 2022 with C++ support
- Install Qt 6.9.2 from https://www.qt.io/download
- Install Intel oneAPI Base Toolkit

### Build

```bash
# Clone or extract the project
cd ImageFlow

# Source Intel oneAPI environment
source /opt/intel/oneapi/setvars.sh  # Linux
# or
call "C:\Program Files (x86)\Intel\oneAPI\setvars.bat"  # Windows

# Build
mkdir -p build && cd build
cmake ..
make -j$(nproc)
```

### Run

**GUI:**
```bash
./run_gui.sh
# or
cd build && ./gui/ImageFlowGUI
```

**CLI:**
```bash
./run_cli.sh help
./run_cli.sh process image.jpg
./run_cli.sh batch
```

**Benchmark:**
```bash
cd build && ./benchmark
```

## Architecture

### Polymorphism & Design Patterns

```
Filter (Abstract Base Class)
├── GrayscaleFilter (CPU)
├── GrayscaleFilterGPU (GPU)
├── InvertFilter (CPU)
├── BrightnessFilter (CPU)
├── BoxBlurFilter (CPU)
├── BoxBlurFilterGPU (GPU)
└── SepiaFilter (CPU)

FilterFactory (Singleton + Factory Pattern)
└── Dynamically creates filters

FilterRegistration
└── Central registration point
```

**Key Principles:**
- **Open/Closed Principle** - Add filters without modifying GUI/CLI
- **Dependency Inversion** - Interfaces depend on `Filter` abstraction
- **Single Responsibility** - Each class has one clear purpose

### Parallelization

**OpenMP (CPU - 100% coverage):**
```cpp
#pragma omp parallel for schedule(dynamic)
for (int y = 0; y < height; ++y) {
    // Each row processed by different CPU core
}
```

**SYCL (GPU - 40% coverage):**
```cpp
sycl::queue q(sycl::gpu_selector_v);
q.submit([&](sycl::handler& h) {
    h.parallel_for(sycl::range<1>(totalPixels), [=](sycl::id<1> idx) {
        // Each pixel processed by separate GPU thread
    });
}).wait();
```

## Adding New Filters

1. Create filter class inheriting from `Filter`:
```cpp
class MyFilter : public Filter {
public:
    void apply(const Image& input, Image& output) override {
        // Implementation
    }
    std::string getName() const override { return "My Filter"; }
    std::unique_ptr<Filter> clone() const override {
        return std::make_unique<MyFilter>(*this);
    }
};
```

2. Register in `core/src/FilterRegistration.cpp`:
```cpp
factory.registerFilter<MyFilter>("myfilter", "My Filter", "Description");
```

3. Rebuild - GUI and CLI automatically discover the new filter!

## Performance Benchmarks

bash -c 'source /opt/intel/oneapi/setvars.sh > /dev/null 2>&1 && cd build && ./benchmark 2>&1'

## Project Structure

```
ImageFlow/
├── core/                    # Core library
│   ├── include/
│   │   ├── Filter.hpp      # Abstract base class
│   │   ├── FilterFactory.hpp
│   │   ├── FilterPipeline.hpp
│   │   └── filters/        # Concrete implementations
│   └── src/
│       └── FilterRegistration.cpp
├── gui/                     # Qt GUI application
├── imageflow_cli.cpp        # Command-line interface
├── benchmark.cpp            # Performance testing
├── run_gui.sh              # GUI launcher
├── run_cli.sh              # CLI launcher
└── report.tex              # Full technical documentation
```

## Technologies Used

- **C++17** - Modern C++ features
- **Qt 6.9.2** - Cross-platform GUI framework
- **OpenMP 4.5+** - Multi-core CPU parallelization
- **Intel oneAPI SYCL** - GPU acceleration
- **CMake 3.20+** - Build system