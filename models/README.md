# Models Directory

This directory contains AI models for object detection.

## Required Models

### YOLOv8 ONNX Model

The system requires a YOLOv8 model in ONNX format:
- **Filename**: `yolov8n.onnx` (or as configured in `config.json`)
- **Format**: ONNX
- **Input**: 640x640 RGB image
- **Output**: Detection results with bounding boxes and class labels

## Obtaining Models

### Option 1: Export from Ultralytics

```python
from ultralytics import YOLO

# Load a pretrained model
model = YOLO('yolov8n.pt')

# Export to ONNX
model.export(format='onnx')
```

### Option 2: Download Pre-exported

Download pre-exported ONNX models from:
- Ultralytics official repository
- ONNX Model Zoo

## Model Configuration

Edit `config.json` to configure the model:

```json
{
  "ai": {
    "model_path": "models/yolov8n.onnx",
    "use_tensorrt": true,
    "use_cuda": true,
    "nms_iou_threshold": 0.45,
    "confidence_threshold": 0.5
  }
}
```

## GPU Acceleration

For optimal performance:
- **CUDA**: Requires NVIDIA GPU and CUDA toolkit
- **TensorRT**: Requires TensorRT installation for additional optimization

## Custom Models

You can use custom-trained YOLOv8 models:
1. Train your model with Ultralytics
2. Export to ONNX format
3. Place in this directory
4. Update `config.json` with the model path
