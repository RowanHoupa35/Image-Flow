cmake_minimum_required(VERSION 3.20)

# Force Intel compiler AVANT project()
set(CMAKE_CXX_COMPILER icpx CACHE STRING "C++ compiler" FORCE)

project(ImageFlow VERSION 0.1.0 LANGUAGES CXX)

# C++ Standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# SYCL flag
add_compile_options(-fsycl)

# Trouver Qt6
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

# Ajouter les sous-projets
add_subdirectory(core)
add_subdirectory(gui)

# Benchmark executable
add_executable(benchmark benchmark.cpp)
target_link_libraries(benchmark PRIVATE CoreLib)
target_include_directories(benchmark PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/core/include)
target_compile_options(benchmark PRIVATE -qopenmp)
target_link_options(benchmark PRIVATE -fsycl -qopenmp)

# CLI executable
add_executable(imageflow_cli imageflow_cli.cpp)
target_link_libraries(imageflow_cli PRIVATE CoreLib)
target_include_directories(imageflow_cli PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/core/include)
target_compile_options(imageflow_cli PRIVATE -qopenmp)
target_link_options(imageflow_cli PRIVATE -fsycl -qopenmp) 