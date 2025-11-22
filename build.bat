@echo off
echo Building AI Hyperfarm Bot...

if not exist build mkdir build
cd build

cmake ..
if %errorlevel% neq 0 (
    echo CMake configuration failed!
    cd ..
    exit /b %errorlevel%
)

cmake --build . --config Release
if %errorlevel% neq 0 (
    echo Build failed!
    cd ..
    exit /b %errorlevel%
)

echo Build completed successfully!
cd ..
