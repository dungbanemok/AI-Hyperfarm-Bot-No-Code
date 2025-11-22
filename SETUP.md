# AI Hyperfarm Bot - Setup Guide

## Project Structure Overview

The project has been set up with the following structure:

```
AI-Hyperfarm-Bot/
├── .kiro/specs/ai-hyperfarm-bot/    # Specification documents
│   ├── requirements.md               # Requirements document
│   ├── design.md                     # Design document
│   └── tasks.md                      # Implementation tasks
├── src/                              # Source files
│   ├── core/                         # Core functionality
│   │   ├── config.cpp               # Configuration management
│   │   └── coordinates.cpp          # Coordinate normalization
│   ├── capture/                      # Screen capture layer
│   │   └── capture_layer.cpp
│   ├── perception/                   # AI and template matching
│   │   └── perception_layer.cpp
│   ├── action/                       # Action execution
│   │   └── action_layer.cpp
│   ├── ui/                          # Qt UI components
│   │   └── manager_ui.cpp
│   └── main.cpp                     # Application entry point
├── include/                          # Header files
│   ├── core/                        # Core interfaces
│   │   ├── types.h                  # Type definitions
│   │   ├── coordinates.h            # Coordinate system
│   │   ├── config.h                 # Configuration structures
│   │   ├── worker_interface.h       # Worker interface
│   │   └── layer_interface.h        # Layer interfaces
│   ├── capture/
│   │   └── capture_layer.h
│   ├── perception/
│   │   └── perception_layer.h
│   ├── action/
│   │   └── action_layer.h
│   └── ui/
│       └── manager_ui.h
├── models/                           # AI models directory
│   └── README.md                    # Model setup instructions
├── templates/                        # Template images
│   └── README.md                    # Template format guide
├── config.json                       # Configuration file
├── CMakeLists.txt                   # CMake build configuration
├── build.bat                        # Windows build script
├── .gitignore                       # Git ignore rules
└── README.md                        # Project documentation

```

## Core Interfaces Implemented

### 1. Type Definitions (`include/core/types.h`)
- `WorkerId`: Worker identifier type
- `NodeId`: Flow node identifier
- Enums: `FSMState`, `CaptureMethod`, `ActionType`, `NodeType`

### 2. Coordinate System (`include/core/coordinates.h`)
- `NormalizedCoord`: Normalized coordinates (1920x1080 standard)
- `NormalizedRect`: Normalized rectangle
- `CoordinateNormalizer`: Conversion utilities

### 3. Worker Interface (`include/core/worker_interface.h`)
- `IWorker`: Base worker interface
- `WorkerStatus`: Worker status structure
- `WorkerMetrics`: Performance metrics
- `EmulatorInfo`: Emulator configuration

### 4. Layer Interfaces (`include/core/layer_interface.h`)
- `ICaptureLayer`: Screen capture interface
- `IPerceptionLayer`: Object detection interface
- `IDecisionLayer`: Decision making interface
- `IActionLayer`: Action execution interface

### 5. Configuration (`include/core/config.h`)
- `Config`: Main configuration class
- Configuration structures for all subsystems

## Dependencies Required

### 1. Qt 5 (5.12 or later)
**Purpose**: GUI framework for Manager UI, Template Creator, and Flow Builder

**Installation**:
- Windows: Download from https://www.qt.io/download
- Install Qt 5.12+ with MSVC compiler support

### 2. OpenCV (4.x)
**Purpose**: Image processing and template matching

**Installation**:
- Download from https://opencv.org/releases/
- Or use vcpkg: `vcpkg install opencv4`

### 3. Boost (1.70 or later)
**Purpose**: Threading and system utilities

**Installation**:
- Download from https://www.boost.org/
- Or use vcpkg: `vcpkg install boost`

### 4. ONNXRuntime (Optional)
**Purpose**: AI inference engine

**Installation**:
- Download from https://github.com/microsoft/onnxruntime/releases
- Extract and note the include/lib paths

## Building the Project

### Step 1: Install Dependencies

Using vcpkg (recommended):
```bash
vcpkg install opencv4:x64-windows
vcpkg install boost:x64-windows
vcpkg install qt5:x64-windows
```

### Step 2: Configure CMake

```bash
mkdir build
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=[path to vcpkg]/scripts/buildsystems/vcpkg.cmake
```

If using ONNXRuntime:
```bash
cmake .. -DONNXRUNTIME_INCLUDE_DIR=C:/path/to/onnxruntime/include -DONNXRUNTIME_LIB_DIR=C:/path/to/onnxruntime/lib
```

### Step 3: Build

Using the build script:
```bash
build.bat
```

Or manually:
```bash
cd build
cmake --build . --config Release
```

### Step 4: Run

```bash
cd build/bin
hyperfarm_bot.exe
```

## Configuration

Edit `config.json` to configure the system:

```json
{
  "manager": {
    "max_workers": 50,
    "checkpoint_interval_sec": 30,
    "metrics_report_interval_sec": 60
  },
  "capture": {
    "prefer_window_capture": true,
    "adb_fallback_enabled": true,
    "buffer_pool_size": 10,
    "target_fps": 10
  },
  "ai": {
    "model_path": "models/yolov8n.onnx",
    "use_tensorrt": true,
    "use_cuda": true,
    "nms_iou_threshold": 0.45,
    "confidence_threshold": 0.5
  }
}
```

## Next Steps

The project structure and core interfaces are now set up. The next tasks are:

1. **Task 2**: Implement coordinate normalization system
2. **Task 3**: Build EmulatorWindowMapper
3. **Task 4**: Implement Capture Layer
4. **Task 5**: Implement AI Inference Engine
5. And so on...

Refer to `.kiro/specs/ai-hyperfarm-bot/tasks.md` for the complete implementation plan.

## Architecture Overview

The system uses a layered architecture:

```
Manager Process
    ├── Worker Thread 1
    │   ├── Capture Layer
    │   ├── Perception Layer
    │   ├── Decision Layer
    │   └── Action Layer
    ├── Worker Thread 2
    │   └── ...
    └── Worker Thread N
        └── ...

Shared Services
    ├── AI Inference Engine
    └── Template Matcher
```

Each Worker runs independently, processing frames through the four layers in sequence.

## Troubleshooting

### CMake can't find Qt
Set Qt5_DIR environment variable:
```bash
set Qt5_DIR=C:/Qt/5.15.2/msvc2019_64/lib/cmake/Qt5
```

### CMake can't find OpenCV
Set OpenCV_DIR environment variable:
```bash
set OpenCV_DIR=C:/opencv/build
```

### Build errors with Boost
Ensure Boost is properly installed and BOOST_ROOT is set:
```bash
set BOOST_ROOT=C:/boost_1_78_0
```

## Support

For issues or questions, refer to:
- Requirements: `.kiro/specs/ai-hyperfarm-bot/requirements.md`
- Design: `.kiro/specs/ai-hyperfarm-bot/design.md`
- Tasks: `.kiro/specs/ai-hyperfarm-bot/tasks.md`
