@echo off
echo ========================================
echo Building and Running Coordinate Tests
echo ========================================
echo.

REM Check if build directory exists
if not exist ..\build (
    echo Error: Build directory not found!
    echo Please run build.bat from the project root first.
    pause
    exit /b 1
)

REM Navigate to build directory
cd ..\build

echo Building test executable...
cmake --build . --target test_coordinates --config Debug
if %errorlevel% neq 0 (
    echo.
    echo Build failed! Please check the error messages above.
    cd ..\tests
    pause
    exit /b %errorlevel%
)

echo.
echo ========================================
echo Running Coordinate Normalization Tests
echo ========================================
echo.

REM Run the tests
if exist bin\Debug\test_coordinates.exe (
    bin\Debug\test_coordinates.exe
) else if exist bin\test_coordinates.exe (
    bin\test_coordinates.exe
) else if exist Debug\test_coordinates.exe (
    Debug\test_coordinates.exe
) else (
    echo Error: Test executable not found!
    echo Expected locations:
    echo   - bin\Debug\test_coordinates.exe
    echo   - bin\test_coordinates.exe
    echo   - Debug\test_coordinates.exe
    cd ..\tests
    pause
    exit /b 1
)

set TEST_RESULT=%errorlevel%

echo.
echo ========================================
if %TEST_RESULT% equ 0 (
    echo All tests PASSED!
) else (
    echo Some tests FAILED!
)
echo ========================================

cd ..\tests
pause
exit /b %TEST_RESULT%
