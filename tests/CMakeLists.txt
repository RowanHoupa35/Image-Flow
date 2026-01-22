# In ImageFlow/tests/CMakeLists.txt
add_executable(test_filters test_filters.cpp)
target_link_libraries(test_filters CoreLib)
target_include_directories(test_filters PRIVATE ../core/include)