file(GLOB_RECURSE CORE_SOURCES "src/*.cpp")
add_library(CoreLib STATIC ${CORE_SOURCES})


get_filename_component(EXTERNAL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../external" ABSOLUTE)

set(PROJECT_ROOT "${CMAKE_SOURCE_DIR}")

set(CORE_SOURCES
    src/Image.cpp
    src/FilterPipeline.cpp
    src/FilterRegistration.cpp
    src/filters/GrayscaleFilter.cpp
    src/filters/InvertFilter.cpp
    src/filters/BrightnessFilter.cpp
    src/filters/BoxBlurFilter.cpp
    src/filters/GrayscaleFilterGPU.cpp
    src/filters/BoxBlurFilterGPU.cpp
)

target_include_directories(CoreLib 
    PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${PROJECT_ROOT}/external         
    PRIVATE
        ${CMAKE_CURRENT_BINARY_DIR}       
)

find_package(OpenMP REQUIRED)
target_link_libraries(CoreLib PRIVATE OpenMP::OpenMP_CXX)

target_link_libraries(CoreLib 
    PRIVATE 
        sycl
)

target_compile_options(CoreLib PRIVATE -Wall -Wextra -O2)
