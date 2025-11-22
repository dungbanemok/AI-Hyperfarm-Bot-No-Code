# Design Document

## Overview

AI Hyperfarm Bot là một hệ thống tự động hóa farming đa Worker được xây dựng hoàn toàn bằng C/C++ với kiến trúc No-Code. Hệ thống cho phép người dùng cuối tạo và chỉnh sửa logic bot thông qua giao diện đồ họa mà không cần viết code. Thiết kế tập trung vào ba trụ cột chính:

1. **Độ tin cậy cao (99.9% uptime)**: Kiến trúc fault-tolerant với cơ chế cô lập lỗi và recovery
2. **Hiệu suất cao**: Tối ưu hóa capture, AI inference và multi-threading để đạt >10 FPS/Worker
3. **No-Code**: Giao diện trực quan cho phép tạo Template và Flow mà không cần lập trình

Hệ thống sử dụng kiến trúc Manager-Worker với các module được phân tách rõ ràng: Capture Layer, Perception Layer, Decision Layer và Action Layer.

## Architecture

### High-Level Architecture

```
┌─────────────────────────────────────────────────────────────────┐
│                         Manager Process                          │
│  ┌────────────┐  ┌──────────────┐  ┌─────────────────────────┐ │
│  │ Manager UI │  │ Flow Builder │  │  Template Creator       │ │
│  │  (Qt GUI)  │  │  (Qt Graph)  │  │    (Qt Graphics)        │ │
│  └─────┬──────┘  └──────┬───────┘  └───────────┬─────────────┘ │
│        │                │                       │                │
│  ┌─────▼────────────────▼───────────────────────▼─────────────┐ │
│  │              Manager Core (C++)                             │ │
│  │  - Worker Registry & Lifecycle Management                   │ │
│  │  - EmulatorWindowMapper (ADB Port ↔ HWND)                  │ │
│  │  - Flow Executor (JSON Graph → FSM)                        │ │
│  │  - Checkpoint Manager                                       │ │
│  └─────────────────────────┬───────────────────────────────────┘ │
└────────────────────────────┼─────────────────────────────────────┘
                             │
        ┌────────────────────┼────────────────────┐
        │                    │                    │
┌───────▼────────┐  ┌────────▼────────┐  ┌───────▼────────┐
│  Worker Thread │  │  Worker Thread  │  │  Worker Thread │
│   (Worker 1)   │  │   (Worker 2)    │  │   (Worker N)   │
│                │  │                 │  │                │
│ ┌────────────┐ │  │ ┌────────────┐  │  │ ┌────────────┐ │
│ │  Capture   │ │  │ │  Capture   │  │  │ │  Capture   │ │
│ │   Layer    │ │  │ │   Layer    │  │  │ │   Layer    │ │
│ └─────┬──────┘ │  │ └─────┬──────┘  │  │ └─────┬──────┘ │
│       │        │  │       │         │  │       │        │
│ ┌─────▼──────┐ │  │ ┌─────▼──────┐  │  │ ┌─────▼──────┐ │
│ │ Perception │ │  │ │ Perception │  │  │ │ Perception │ │
│ │   Layer    │ │  │ │   Layer    │  │  │ │   Layer    │ │
│ └─────┬──────┘ │  │ └─────┬──────┘  │  │ └─────┬──────┘ │
│       │        │  │       │         │  │       │        │
│ ┌─────▼──────┐ │  │ ┌─────▼──────┐  │  │ ┌─────▼──────┐ │
│ │  Decision  │ │  │ │  Decision  │  │  │ │  Decision  │ │
│ │   Layer    │ │  │ │   Layer    │  │  │ │   Layer    │ │
│ └─────┬──────┘ │  │ └─────┬──────┘  │  │ └─────┬──────┘ │
│       │        │  │       │         │  │       │        │
│ ┌─────▼──────┐ │  │ ┌─────▼──────┐  │  │ ┌─────▼──────┐ │
│ │   Action   │ │  │ │   Action   │  │  │ │   Action   │ │
│ │   Layer    │ │  │ │   Layer    │  │  │ │   Layer    │ │
│ └────────────┘ │  │ └────────────┘  │  │ └────────────┘ │
└────────────────┘  └─────────────────┘  └────────────────┘
         │                   │                    │
         └───────────────────┼────────────────────┘
                             │
                    ┌────────▼─────────┐
                    │  Shared Services │
                    │                  │
                    │ ┌──────────────┐ │
                    │ │ AI Inference │ │
                    │ │    Engine    │ │
                    │ │ (ONNXRuntime)│ │
                    │ └──────────────┘ │
                    │                  │
                    │ ┌──────────────┐ │
                    │ │   Template   │ │
                    │ │   Matcher    │ │
                    │ │   (OpenCV)   │ │
                    │ └──────────────┘ │
                    └──────────────────┘
```

### Key Architectural Decisions

1. **Manager-Worker Pattern**: Manager process điều phối N Worker threads, mỗi Worker quản lý một emulator instance
2. **Layered Processing Pipeline**: Mỗi Worker có 4 layer xử lý tuần tự: Capture → Perception → Decision → Action
3. **Shared AI Engine**: Tất cả Worker chia sẻ một AI Inference Engine duy nhất với mutex-protected queue để tối ưu GPU
4. **No-Code via JSON**: Flow logic và Template metadata được lưu dưới dạng JSON, được load và execute runtime
5. **Dual Capture Strategy**: Window Capture (primary) + ADB Atomic Chain (fallback) để đảm bảo reliability

## Components and Interfaces

### 1. Manager Core

**Responsibilities:**
- Quản lý lifecycle của tất cả Worker threads
- Ánh xạ và duy trì mapping giữa ADB Port ↔ HWND
- Load và parse JSON Graph thành FSM
- Quản lý checkpoint và recovery
- Thu thập và báo cáo metrics

**Key Classes:**

```cpp
class WorkerManager {
public:
    void initialize(const Config& config);
    WorkerId createWorker(const EmulatorInfo& emulator);
    void startWorker(WorkerId id, const FlowGraph& flow);
    void stopWorker(WorkerId id);
    void restartWorker(WorkerId id);
    WorkerStatus getWorkerStatus(WorkerId id);
    std::vector<WorkerMetrics> getAllMetrics();
    
private:
    std::map<WorkerId, std::unique_ptr<WorkerThread>> workers_;
    EmulatorWindowMapper mapper_;
    CheckpointManager checkpoint_mgr_;
    std::mutex workers_mutex_;
};

class EmulatorWindowMapper {
public:
    bool mapEmulator(const EmulatorInfo& info);
    HWND getHWND(WorkerId id);
    int getADBPort(WorkerId id);
    bool verifyMapping(WorkerId id);
    float getMappingSuccessRate();
    
private:
    struct Mapping {
        WorkerId worker_id;
        std::string emulator_internal_id;
        HWND hwnd;
        int adb_port;
        bool verified;
    };
    std::map<WorkerId, Mapping> mappings_;
    std::mutex mapping_mutex_;
};

class FlowExecutor {
public:
    void loadFlow(const std::string& json_path);
    FSM buildFSM(const FlowGraph& graph);
    NodeId getNextNode(NodeId current, const DecisionResult& result);
    
private:
    FlowGraph graph_;
    std::map<NodeId, std::unique_ptr<Node>> nodes_;
};
```

### 2. Worker Thread

**Responsibilities:**
- Thực thi flow logic cho một emulator cụ thể
- Điều phối 4 layer xử lý: Capture → Perception → Decision → Action
- Duy trì FSM state và current node
- Báo cáo metrics và status về Manager

**Key Classes:**

```cpp
class WorkerThread {
public:
    WorkerThread(WorkerId id, const EmulatorInfo& emulator);
    void run(const FlowGraph& flow);
    void stop();
    void pause();
    void resume();
    WorkerStatus getStatus();
    WorkerMetrics getMetrics();
    
private:
    void processLoop();
    void saveCheckpoint();
    void loadCheckpoint();
    
    WorkerId id_;
    EmulatorInfo emulator_;
    FSM fsm_;
    NodeId current_node_;
    
    std::unique_ptr<CaptureLayer> capture_;
    std::unique_ptr<PerceptionLayer> perception_;
    std::unique_ptr<DecisionLayer> decision_;
    std::unique_ptr<ActionLayer> action_;
    
    std::atomic<bool> running_;
    std::atomic<bool> paused_;
    FPSCounter fps_counter_;
};

struct WorkerStatus {
    WorkerId id;
    FSMState state;
    NodeId current_node;
    float fps;
    bool is_running;
    bool is_paused;
    std::string last_error;
};

struct WorkerMetrics {
    WorkerId id;
    float capture_failure_rate;
    float avg_inference_time_ms;
    float avg_action_latency_ms;
    int total_frames_processed;
    int total_errors;
};
```

### 3. Capture Layer

**Responsibilities:**
- Chụp ảnh màn hình từ emulator window
- Ưu tiên Window Capture, fallback sang ADB nếu thất bại
- Chuẩn hóa ảnh về độ phân giải 1920x1080
- Quản lý buffer pool để tái sử dụng memory

**Key Classes:**

```cpp
class CaptureLayer {
public:
    CaptureLayer(WorkerId id, HWND hwnd, int adb_port);
    CaptureResult capture();
    float getFailureRate();
    
private:
    CaptureResult windowCapture();
    CaptureResult adbCapture();
    cv::Mat normalizeResolution(const cv::Mat& img);
    
    WorkerId worker_id_;
    HWND hwnd_;
    int adb_port_;
    BufferPool<cv::Mat> buffer_pool_;
    
    int capture_attempts_;
    int capture_failures_;
};

struct CaptureResult {
    bool success;
    cv::Mat image;  // Always 1920x1080 if success
    CaptureMethod method;  // WINDOW_CAPTURE or ADB_CAPTURE
    double timestamp_ms;
    std::string error_msg;
};

class BufferPool<T> {
public:
    BufferPool(size_t pool_size);
    T* acquire();
    void release(T* buffer);
    
private:
    std::vector<std::unique_ptr<T>> pool_;
    std::queue<T*> available_;
    std::mutex mutex_;
};
```

### 4. Perception Layer

**Responsibilities:**
- Nhận diện đối tượng trên ảnh bằng AI hoặc Template Matching
- Chuẩn hóa tọa độ kết quả về 1920x1080
- Áp dụng NMS cho AI detection
- Cache kết quả để tối ưu performance

**Key Classes:**

```cpp
class PerceptionLayer {
public:
    PerceptionLayer(WorkerId id);
    void setAIEngine(std::shared_ptr<AIInferenceEngine> engine);
    void setTemplateMatcher(std::shared_ptr<TemplateMatcher> matcher);
    
    PerceptionResult detectAI(const cv::Mat& image, const std::string& target_class);
    PerceptionResult matchTemplate(const cv::Mat& image, const Template& tmpl);
    
private:
    WorkerId worker_id_;
    std::shared_ptr<AIInferenceEngine> ai_engine_;
    std::shared_ptr<TemplateMatcher> template_matcher_;
};

struct PerceptionResult {
    bool found;
    std::vector<BoundingBox> detections;  // Normalized coordinates
    float confidence;
    double processing_time_ms;
};

struct BoundingBox {
    NormalizedCoord top_left;
    NormalizedCoord bottom_right;
    std::string class_name;
    float confidence;
};
```

### 5. AI Inference Engine (Shared Service)

**Responsibilities:**
- Load và quản lý mô hình YOLOv8 ONNX
- Thực hiện inference với CUDA/TensorRT acceleration
- Xử lý queue request từ nhiều Worker với mutex protection
- Áp dụng NMS và chuẩn hóa tọa độ

**Key Classes:**

```cpp
class AIInferenceEngine {
public:
    AIInferenceEngine();
    void loadModel(const std::string& onnx_path, bool use_tensorrt = true);
    std::vector<Detection> infer(const cv::Mat& image);
    
private:
    void preprocessImage(const cv::Mat& image, float* input_tensor);
    std::vector<Detection> postprocess(const float* output_tensor);
    std::vector<Detection> applyNMS(const std::vector<Detection>& detections, float iou_threshold);
    
    std::unique_ptr<Ort::Session> session_;
    std::unique_ptr<Ort::Env> env_;
    std::mutex inference_mutex_;
    
    // Model metadata
    std::vector<int64_t> input_shape_;
    std::vector<std::string> class_names_;
};

struct Detection {
    BoundingBox bbox;  // Already normalized
    std::string class_name;
    float confidence;
};
```

### 6. Template Matcher (Shared Service)

**Responsibilities:**
- Thực hiện template matching bằng OpenCV
- Tìm kiếm template trong Search Region được định nghĩa
- Trả về tọa độ chuẩn hóa của match

**Key Classes:**

```cpp
class TemplateMatcher {
public:
    TemplateMatcher();
    MatchResult match(const cv::Mat& image, const Template& tmpl);
    
private:
    cv::Mat extractSearchRegion(const cv::Mat& image, const NormalizedRect& region);
    NormalizedCoord convertToNormalized(const cv::Point& pt, const cv::Size& img_size);
    
    std::mutex match_mutex_;
};

struct Template {
    std::string id;
    cv::Mat template_image;
    NormalizedRect search_region;
    float confidence_threshold;
};

struct MatchResult {
    bool found;
    NormalizedCoord location;
    float confidence;
};
```

### 7. Decision Layer

**Responsibilities:**
- Đánh giá kết quả từ Perception Layer
- Quyết định Node tiếp theo dựa trên Flow logic
- Cập nhật FSM state

**Key Classes:**

```cpp
class DecisionLayer {
public:
    DecisionLayer(WorkerId id, FlowExecutor* executor);
    DecisionResult evaluate(const Node& current_node, const PerceptionResult& perception);
    
private:
    WorkerId worker_id_;
    FlowExecutor* executor_;
};

struct DecisionResult {
    NodeId next_node;
    bool should_continue;
    std::map<std::string, std::string> context;  // Variables to pass to next node
};
```

### 8. Action Layer

**Responsibilities:**
- Thực thi action được định nghĩa trong Node
- Chuyển đổi tọa độ chuẩn hóa về tọa độ thực tế của emulator
- Gửi lệnh ADB hoặc WinAPI để thực hiện Click/Swipe/Wait

**Key Classes:**

```cpp
class ActionLayer {
public:
    ActionLayer(WorkerId id, HWND hwnd, int adb_port);
    ActionResult execute(const Action& action);
    
private:
    ActionResult executeClick(const NormalizedCoord& coord);
    ActionResult executeSwipe(const NormalizedCoord& from, const NormalizedCoord& to, int duration_ms);
    ActionResult executeWait(int duration_ms);
    
    cv::Point denormalizeCoord(const NormalizedCoord& coord);
    
    WorkerId worker_id_;
    HWND hwnd_;
    int adb_port_;
    cv::Size actual_resolution_;
    ADBConnectionPool adb_pool_;
};

struct Action {
    ActionType type;  // CLICK, SWIPE, WAIT
    NormalizedCoord coord;  // For CLICK
    NormalizedCoord from_coord;  // For SWIPE
    NormalizedCoord to_coord;  // For SWIPE
    int duration_ms;  // For WAIT or SWIPE
};

struct ActionResult {
    bool success;
    double latency_ms;
    std::string error_msg;
};
```

### 9. Coordinate Normalization System

**Responsibilities:**
- Chuẩn hóa tất cả tọa độ về độ phân giải chuẩn 1920x1080
- Chuyển đổi qua lại giữa normalized và actual coordinates

**Key Classes:**

```cpp
struct NormalizedCoord {
    float x;  // 0.0 to 1920.0
    float y;  // 0.0 to 1080.0
    
    static NormalizedCoord fromActual(int actual_x, int actual_y, const cv::Size& actual_size);
    cv::Point toActual(const cv::Size& actual_size) const;
};

struct NormalizedRect {
    NormalizedCoord top_left;
    NormalizedCoord bottom_right;
    
    static NormalizedRect fromActual(const cv::Rect& rect, const cv::Size& actual_size);
    cv::Rect toActual(const cv::Size& actual_size) const;
};

class CoordinateNormalizer {
public:
    static constexpr int STANDARD_WIDTH = 1920;
    static constexpr int STANDARD_HEIGHT = 1080;
    
    static NormalizedCoord normalize(int x, int y, int actual_width, int actual_height);
    static cv::Point denormalize(const NormalizedCoord& coord, int actual_width, int actual_height);
};
```

### 10. Manager UI (Qt)

**Responsibilities:**
- Hiển thị trạng thái của tất cả Worker theo thời gian thực
- Cho phép start/stop/pause Worker
- Hiển thị metrics và logs

**Key Classes:**

```cpp
class ManagerUI : public QMainWindow {
    Q_OBJECT
    
public:
    ManagerUI(WorkerManager* manager, QWidget* parent = nullptr);
    
private slots:
    void onWorkerStatusChanged(WorkerId id, const WorkerStatus& status);
    void onStartWorkerClicked();
    void onStopWorkerClicked();
    void onPauseWorkerClicked();
    
private:
    void setupUI();
    void updateWorkerTable();
    
    WorkerManager* manager_;
    QTableView* worker_table_;
    QTimer* update_timer_;
};
```

### 11. Template Creator (Qt)

**Responsibilities:**
- Cho phép người dùng vẽ Search Region trên ảnh
- Cho phép người dùng đặt Action Point
- Lưu Template dưới dạng JSON Metadata

**Key Classes:**

```cpp
class TemplateCreator : public QWidget {
    Q_OBJECT
    
public:
    TemplateCreator(QWidget* parent = nullptr);
    void loadScreenshot(const cv::Mat& screenshot);
    void saveTemplate(const std::string& path);
    
private:
    void onMousePress(QMouseEvent* event);
    void onMouseMove(QMouseEvent* event);
    void onMouseRelease(QMouseEvent* event);
    
    cv::Mat screenshot_;
    NormalizedRect search_region_;
    NormalizedCoord action_point_;
    bool drawing_region_;
};
```

### 12. Flow Builder (Qt)

**Responsibilities:**
- Cho phép người dùng tạo và chỉnh sửa Flow bằng Node Editor
- Serialize/Deserialize Flow thành JSON Graph

**Key Classes:**

```cpp
class FlowBuilder : public QWidget {
    Q_OBJECT
    
public:
    FlowBuilder(QWidget* parent = nullptr);
    void loadFlow(const std::string& json_path);
    void saveFlow(const std::string& json_path);
    
private:
    void onNodeAdded(NodeType type);
    void onNodeConnected(NodeId from, NodeId to);
    void onNodeDeleted(NodeId id);
    
    QGraphicsView* graphics_view_;
    QGraphicsScene* scene_;
    FlowGraph graph_;
};

class FlowNode : public QGraphicsItem {
public:
    FlowNode(NodeType type, NodeId id);
    void setConfig(const NodeConfig& config);
    NodeConfig getConfig() const;
    
private:
    NodeType type_;
    NodeId id_;
    NodeConfig config_;
};
```

## Data Models

### Flow Graph (JSON)

```json
{
  "flow_id": "farming_flow_001",
  "version": "1.0",
  "nodes": [
    {
      "node_id": "node_1",
      "type": "template_match",
      "config": {
        "template_id": "start_button",
        "confidence_threshold": 0.8
      },
      "next_on_success": "node_2",
      "next_on_failure": "node_1"
    },
    {
      "node_id": "node_2",
      "type": "action",
      "config": {
        "action_type": "click",
        "use_template_location": true
      },
      "next": "node_3"
    },
    {
      "node_id": "node_3",
      "type": "wait",
      "config": {
        "duration_ms": 2000
      },
      "next": "node_4"
    },
    {
      "node_id": "node_4",
      "type": "ai_detection",
      "config": {
        "target_class": "reward_icon",
        "min_confidence": 0.7
      },
      "next_on_success": "node_5",
      "next_on_failure": "node_1"
    },
    {
      "node_id": "node_5",
      "type": "action",
      "config": {
        "action_type": "click",
        "use_detection_location": true
      },
      "next": "node_1"
    }
  ],
  "start_node": "node_1"
}
```

### Template Metadata (JSON)

```json
{
  "template_id": "start_button",
  "name": "Start Button",
  "image_path": "templates/start_button.png",
  "search_region": {
    "top_left": {
      "x": 800.0,
      "y": 900.0
    },
    "bottom_right": {
      "x": 1120.0,
      "y": 1000.0
    }
  },
  "action_point": {
    "x": 960.0,
    "y": 950.0
  },
  "confidence_threshold": 0.8,
  "created_at": "2024-01-15T10:30:00Z",
  "updated_at": "2024-01-15T10:30:00Z"
}
```

### Checkpoint Data (JSON)

```json
{
  "worker_id": "worker_001",
  "timestamp": "2024-01-15T12:45:30Z",
  "fsm_state": "running",
  "current_node": "node_3",
  "context": {
    "loop_counter": 5,
    "last_detection_confidence": 0.85
  },
  "metrics": {
    "total_frames": 1523,
    "total_errors": 2,
    "capture_failure_rate": 0.003
  }
}
```

### Configuration (JSON)

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
  },
  "template_matching": {
    "method": "TM_CCOEFF_NORMED",
    "default_confidence_threshold": 0.8
  },
  "action": {
    "adb_connection_pool_size": 5,
    "adb_retry_count": 3,
    "adb_retry_delay_ms": 100,
    "max_action_latency_ms": 200
  }
}
```

