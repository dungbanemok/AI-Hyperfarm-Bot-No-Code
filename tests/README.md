# Unit Tests for AI Hyperfarm Bot

## Overview

This directory contains unit tests for the AI Hyperfarm Bot project. The tests are written using Google Test framework.

## Test Coverage

### Coordinate Normalization Tests (`test_coordinates.cpp`)

Comprehensive tests for the coordinate normalization system covering all requirements:

#### Requirement 7.1: Normalization to Standard Resolution
- `NormalizeToStandardResolution`: Tests identity transformation for 1920x1080
- `NormalizeFromHDResolution`: Tests normalization from 1280x720
- `NormalizeFromQHDResolution`: Tests normalization from 2560x1440
- `NormalizeFromUHDResolution`: Tests normalization from 3840x2160
- `NormalizeUsesFloatingPoint`: Verifies floating-point precision is maintained

#### Requirement 7.2: Denormalization to Actual Resolution
- `DenormalizeToStandardResolution`: Tests denormalization to 1920x1080
- `DenormalizeToHDResolution`: Tests denormalization to 1280x720
- `DenormalizeToQHDResolution`: Tests denormalization to 2560x1440
- `DenormalizeToUHDResolution`: Tests denormalization to 3840x2160
- `DenormalizeToMobileResolution`: Tests denormalization to portrait orientation

#### Requirement 7.3: Action Point Storage
- `NormalizedCoordFromActual`: Tests static factory method
- `NormalizedCoordToActual`: Tests conversion back to actual coordinates
- `ActionPointStorageFormat`: Verifies action points can be stored and retrieved correctly

#### Requirement 7.4: Bounding Box Normalization
- `NormalizedRectFromActual`: Tests rectangle normalization
- `NormalizedRectToActual`: Tests rectangle denormalization
- `BoundingBoxConsistency`: Verifies bounding boxes maintain consistency

#### Requirement 7.5: Search Region Processing
- `SearchRegionNormalization`: Tests search region normalization
- `SearchRegionAcrossResolutions`: Verifies proportions are maintained across resolutions

#### Additional Edge Cases
- `CornerCoordinates`: Tests corner coordinates (0,0) and (max,max)
- `RoundTripConsistency`: Tests round-trip conversion maintains consistency
- `NonStandardAspectRatio`: Tests with non-standard aspect ratios
- `PrecisionMaintenance`: Tests precision through multiple conversions
- `RectangleWidthHeightCalculation`: Verifies rectangle dimensions
- `MultipleResolutionConsistency`: Tests proportional results across resolutions

## Building and Running Tests

### Prerequisites

1. **Google Test**: Install Google Test framework
   ```bash
   vcpkg install gtest:x64-windows
   ```

2. **OpenCV**: Required for cv::Point and cv::Size types
   ```bash
   vcpkg install opencv4:x64-windows
   ```

### Build Tests

```bash
# From project root
mkdir build
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=[path to vcpkg]/scripts/buildsystems/vcpkg.cmake
cmake --build . --config Debug
```

### Run Tests

```bash
# Run all tests
cd build
ctest --output-on-failure

# Or run specific test executable
./bin/test_coordinates
```

### Run Tests with Verbose Output

```bash
./bin/test_coordinates --gtest_verbose
```

### Run Specific Test Cases

```bash
# Run only normalization tests
./bin/test_coordinates --gtest_filter=*Normalize*

# Run only denormalization tests
./bin/test_coordinates --gtest_filter=*Denormalize*
```

## Test Resolutions

The tests use the following common resolutions:

- **HD**: 1280x720
- **Full HD**: 1920x1080 (standard/normalized)
- **QHD**: 2560x1440
- **4K UHD**: 3840x2160
- **Mobile Portrait**: 1080x1920
- **Tablet**: 1024x768

## Expected Results

All tests should pass with the current implementation. The tests verify:

1. ✅ Coordinates are normalized to 1920x1080 standard using floating-point precision
2. ✅ Coordinates can be denormalized to any target resolution
3. ✅ Action points can be stored in normalized format and retrieved correctly
4. ✅ Bounding boxes maintain consistency across resolutions
5. ✅ Search regions use normalized coordinates for all calculations
6. ✅ Round-trip conversions maintain precision (within rounding tolerance)
7. ✅ Edge cases (corners, non-standard aspect ratios) are handled correctly

## Troubleshooting

### Google Test Not Found

If CMake can't find Google Test:
```bash
set GTEST_ROOT=C:/path/to/googletest
```

Or install via vcpkg:
```bash
vcpkg install gtest
```

### OpenCV Not Found

Ensure OpenCV is installed and OpenCV_DIR is set:
```bash
set OpenCV_DIR=C:/opencv/build
```

### Tests Fail to Link

Make sure the hyperfarm_core library is built before running tests:
```bash
cmake --build . --target hyperfarm_core
cmake --build . --target test_coordinates
```

## Adding New Tests

To add new test cases:

1. Add test functions to `test_coordinates.cpp` using the `TEST_F` macro
2. Use the `CoordinateNormalizationTest` fixture for setup
3. Follow the naming convention: `TestCategory_SpecificBehavior`
4. Add comments referencing the requirement being tested

Example:
```cpp
TEST_F(CoordinateNormalizationTest, NewTestCase) {
    // Test implementation
    NormalizedCoord norm = CoordinateNormalizer::normalize(x, y, w, h);
    EXPECT_TRUE(floatEqual(norm.x, expected_x));
}
```

## Continuous Integration

These tests should be run as part of the CI/CD pipeline to ensure coordinate normalization remains correct across code changes.

