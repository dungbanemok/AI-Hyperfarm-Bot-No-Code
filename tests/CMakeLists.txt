# Test configuration
cmake_minimum_required(VERSION 3.15)

# Find Google Test
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Test executable for coordinate normalization
add_executable(test_coordinates
    test_coordinates.cpp
)

target_link_libraries(test_coordinates
    hyperfarm_core
    ${GTEST_LIBRARIES}
    ${GTEST_MAIN_LIBRARIES}
    ${OpenCV_LIBS}
    pthread
)

# Add test to CTest
add_test(NAME CoordinateNormalizationTests COMMAND test_coordinates)
