# Quick Start - Running Coordinate Normalization Tests

## Prerequisites Check

Before running tests, ensure you have:
- ✅ CMake installed
- ✅ Google Test installed (via vcpkg or manual installation)
- ✅ OpenCV installed
- ✅ Project built successfully (run `build.bat` from project root)

## Run Tests (3 Easy Steps)

### Step 1: Navigate to tests directory
```bash
cd tests
```

### Step 2: Run the test script
```bash
build_and_run_tests.bat
```

### Step 3: Check results
- ✅ **All tests PASSED!** - You're good to go!
- ❌ **Some tests FAILED!** - Check the output for details

---

## What Gets Tested?

✅ **25+ test cases** covering:
- Coordinate normalization to 1920x1080 standard
- Denormalization to various resolutions (HD, QHD, 4K, mobile)
- Action point storage and retrieval
- Bounding box consistency
- Search region processing
- Edge cases and precision

✅ **All Requirements 7.1-7.5** validated

---

## Troubleshooting

### "Build directory not found"
**Solution**: Run `build.bat` from project root first
```bash
cd ..
build.bat
cd tests
build_and_run_tests.bat
```

### "Google Test not found"
**Solution**: Install Google Test via vcpkg
```bash
vcpkg install gtest:x64-windows
```

### "Test executable not found"
**Solution**: Build the test target manually
```bash
cd ..\build
cmake --build . --target test_coordinates --config Debug
```

---

## Alternative: Manual Test Run

If the batch script doesn't work, run manually:

```bash
# From project root
cd build

# Build tests
cmake --build . --target test_coordinates --config Debug

# Run tests
bin\Debug\test_coordinates.exe

# Or use CTest
ctest -R CoordinateNormalizationTests --output-on-failure
```

---

## Expected Output (Success)

```
[==========] Running 25 tests from 1 test suite.
[----------] 25 tests from CoordinateNormalizationTest
[ RUN      ] CoordinateNormalizationTest.NormalizeToStandardResolution
[       OK ] CoordinateNormalizationTest.NormalizeToStandardResolution
...
[  PASSED  ] 25 tests.
```

---

## Need More Info?

- 📖 Full documentation: `tests/README.md`
- 📋 Implementation details: `tests/TEST_IMPLEMENTATION_SUMMARY.md`
- 🔧 Test source code: `tests/test_coordinates.cpp`

---

**Ready to proceed?** Once all tests pass, you can move on to the next task in the implementation plan!

