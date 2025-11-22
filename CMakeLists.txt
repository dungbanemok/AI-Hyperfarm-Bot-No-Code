cmake_minimum_required(VERSION 3.15)
project(AIHyperfarmBot VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Find required packages
find_package(Qt5 REQUIRED COMPONENTS Core Widgets Gui)
find_package(OpenCV REQUIRED)
find_package(Boost REQUIRED COMPONENTS system thread)

# ONNXRuntime - you may need to set ONNXRUNTIME_ROOT_DIR
# find_package(onnxruntime REQUIRED)
# For now, we'll add it as an optional dependency
set(ONNXRUNTIME_INCLUDE_DIR "" CACHE PATH "Path to ONNXRuntime include directory")
set(ONNXRUNTIME_LIB_DIR "" CACHE PATH "Path to ONNXRuntime library directory")

# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/core
    ${CMAKE_SOURCE_DIR}/include/capture
    ${CMAKE_SOURCE_DIR}/include/perception
    ${CMAKE_SOURCE_DIR}/include/action
    ${CMAKE_SOURCE_DIR}/include/ui
    ${OpenCV_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
    ${ONNXRUNTIME_INCLUDE_DIR}
)

# Core library
file(GLOB_RECURSE CORE_SOURCES 
    "src/core/*.cpp"
)

file(GLOB_RECURSE CORE_HEADERS 
    "include/core/*.h"
)

add_library(hyperfarm_core STATIC
    ${CORE_SOURCES}
    ${CORE_HEADERS}
)

target_link_libraries(hyperfarm_core
    ${OpenCV_LIBS}
    ${Boost_LIBRARIES}
)

# Capture library
file(GLOB_RECURSE CAPTURE_SOURCES 
    "src/capture/*.cpp"
)

file(GLOB_RECURSE CAPTURE_HEADERS 
    "include/capture/*.h"
)

add_library(hyperfarm_capture STATIC
    ${CAPTURE_SOURCES}
    ${CAPTURE_HEADERS}
)

target_link_libraries(hyperfarm_capture
    hyperfarm_core
    ${OpenCV_LIBS}
)

# Perception library
file(GLOB_RECURSE PERCEPTION_SOURCES 
    "src/perception/*.cpp"
)

file(GLOB_RECURSE PERCEPTION_HEADERS 
    "include/perception/*.h"
)

add_library(hyperfarm_perception STATIC
    ${PERCEPTION_SOURCES}
    ${PERCEPTION_HEADERS}
)

target_link_libraries(hyperfarm_perception
    hyperfarm_core
    ${OpenCV_LIBS}
)

# Action library
file(GLOB_RECURSE ACTION_SOURCES 
    "src/action/*.cpp"
)

file(GLOB_RECURSE ACTION_HEADERS 
    "include/action/*.h"
)

add_library(hyperfarm_action STATIC
    ${ACTION_SOURCES}
    ${ACTION_HEADERS}
)

target_link_libraries(hyperfarm_action
    hyperfarm_core
)

# UI library
file(GLOB_RECURSE UI_SOURCES 
    "src/ui/*.cpp"
)

file(GLOB_RECURSE UI_HEADERS 
    "include/ui/*.h"
)

# Enable Qt MOC for UI files
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_library(hyperfarm_ui STATIC
    ${UI_SOURCES}
    ${UI_HEADERS}
)

target_link_libraries(hyperfarm_ui
    hyperfarm_core
    Qt5::Core
    Qt5::Widgets
    Qt5::Gui
)

# Main executable
add_executable(hyperfarm_bot
    src/main.cpp
)

target_link_libraries(hyperfarm_bot
    hyperfarm_core
    hyperfarm_capture
    hyperfarm_perception
    hyperfarm_action
    hyperfarm_ui
    ${OpenCV_LIBS}
    ${Boost_LIBRARIES}
    Qt5::Core
    Qt5::Widgets
    Qt5::Gui
)

# Installation rules
install(TARGETS hyperfarm_bot
    RUNTIME DESTINATION bin
)

install(DIRECTORY models/ DESTINATION models)
install(DIRECTORY templates/ DESTINATION templates)
install(FILES config.json DESTINATION .)

# Enable testing
enable_testing()

# Add tests subdirectory if it exists
if(EXISTS ${CMAKE_SOURCE_DIR}/tests)
    add_subdirectory(tests)
endif()
