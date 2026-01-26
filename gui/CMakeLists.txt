find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(GUI_SOURCES
    src/main.cpp
    src/MainWindow.cpp
)

set(GUI_HEADERS
    include/MainWindow.hpp
)

qt6_add_executable(ImageFlowGUI 
    ${GUI_SOURCES}
    ${GUI_HEADERS}
)


set_property(SOURCE include/MainWindow.hpp PROPERTY SKIP_AUTOMOC OFF)

# Liaisons
target_link_libraries(ImageFlowGUI 
    PRIVATE 
        Qt6::Core
        Qt6::Widgets
        CoreLib
)

target_include_directories(ImageFlowGUI 
    PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../core/include
)

set_target_properties(ImageFlowGUI PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    AUTOMOC ON
    AUTOUIC ON
)
