# Create a static library from all .cpp files in the src/ directory.
file(GLOB_RECURSE CORE_SOURCES "src/*.cpp")
add_library(CoreLib STATIC ${CORE_SOURCES})


# Get the absolute path to external directory
get_filename_component(EXTERNAL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../external" ABSOLUTE)

# Get the PROJECT_SOURCE_DIR (where CMakeLists.txt is)
set(PROJECT_ROOT "${CMAKE_SOURCE_DIR}")

# In core/CMakeLists.txt, update the file list:
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

# Add BOTH source and build include paths
target_include_directories(CoreLib 
    PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${PROJECT_ROOT}/external           # <-- ABSOLUTE path to external
    PRIVATE
        ${CMAKE_CURRENT_BINARY_DIR}        # For generated files
)

# Also ensure Image.cpp uses simple includes:
# #include "stb_image.h"
# #include "stb_image_write.h"
# CoreLib uses SYCL, so it needs the '-fsycl' flag (inherited from root)
# and must link against the SYCL runtime.
find_package(OpenMP REQUIRED)
target_link_libraries(CoreLib PRIVATE OpenMP::OpenMP_CXX)

target_link_libraries(CoreLib 
    PRIVATE 
        sycl
)

# Optional but good practice: Enable warnings and optimization.
target_compile_options(CoreLib PRIVATE -Wall -Wextra -O2)