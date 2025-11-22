# AI Hyperfarm Bot

A high-performance, multi-worker automation system for Android emulators with No-Code capabilities.

## Project Structure

```
AI-Hyperfarm-Bot/
├── src/                    # Source files
│   ├── core/              # Core functionality (coordinates, config, etc.)
│   ├── capture/           # Screen capture layer
│   ├── perception/        # AI inference and template matching
│   ├── action/            # Action execution (ADB, WinAPI)
│   ├── ui/                # Qt-based UI components
│   └── main.cpp           # Application entry point
├── include/               # Header files
│   ├── core/              # Core interfaces and types
│   ├── capture/           # Capture layer interfaces
│   ├── perception/        # Perception layer interfaces
│   ├── action/            # Action layer interfaces
│   └── ui/                # UI component headers
├── models/                # AI models (YOLOv8 ONNX)
├── templates/             # Template images and metadata
├── config.json            # Configuration file
├── CMakeLists.txt         # CMake build configuration
└── README.md              # This file

```

## Dependencies

- **Qt 5**: GUI framework for Manager UI, Template Creator, and Flow Builder
- **OpenCV**: Image processing and template matching
- **ONNXRuntime**: AI inference engine (with CUDA/TensorRT support)
- **Boost**: Threading and system utilities

## Building

### Prerequisites

1. Install Qt 5 (5.12 or later)
2. Install OpenCV (4.x)
3. Install Boost (1.70 or later)
4. Install ONNXRuntime (optional, for AI features)

### Build Instructions

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

### CMake Options

- `ONNXRUNTIME_INCLUDE_DIR`: Path to ONNXRuntime include directory
- `ONNXRUNTIME_LIB_DIR`: Path to ONNXRuntime library directory

Example:
```bash
cmake .. -DONNXRUNTIME_INCLUDE_DIR=/path/to/onnxruntime/include -DONNXRUNTIME_LIB_DIR=/path/to/onnxruntime/lib
```

## Configuration

Edit `config.json` to configure the system:

- **manager**: Worker management settings
- **capture**: Screen capture settings
- **ai**: AI inference settings
- **template_matching**: Template matching settings
- **action**: Action execution settings

## Architecture

The system uses a layered architecture:

1. **Capture Layer**: Captures screenshots from emulator windows
2. **Perception Layer**: Detects objects using AI or template matching
3. **Decision Layer**: Evaluates results and determines next action
4. **Action Layer**: Executes actions (click, swipe, wait)

Each Worker runs these layers in a loop, managed by the central Manager process.

## Core Interfaces

- `IWorker`: Worker thread interface
- `ICaptureLayer`: Screen capture interface
- `IPerceptionLayer`: Object detection interface
- `IDecisionLayer`: Decision making interface
- `IActionLayer`: Action execution interface

## Coordinate System

All coordinates are normalized to 1920x1080 standard resolution:
- Templates and actions use normalized coordinates
- Automatic conversion to actual emulator resolution
- Ensures consistency across different screen sizes

## License

[Add your license here]

## Contributing

[Add contribution guidelines here]
