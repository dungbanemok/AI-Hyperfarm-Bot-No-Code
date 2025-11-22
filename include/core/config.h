#pragma once

#include <string>
#include <memory>

namespace hyperfarm {

struct ManagerConfig {
    int max_workers = 50;
    int checkpoint_interval_sec = 30;
    int metrics_report_interval_sec = 60;
};

struct CaptureConfig {
    bool prefer_window_capture = true;
    bool adb_fallback_enabled = true;
    int buffer_pool_size = 10;
    int target_fps = 10;
};

struct AIConfig {
    std::string model_path = "models/yolov8n.onnx";
    bool use_tensorrt = true;
    bool use_cuda = true;
    float nms_iou_threshold = 0.45f;
    float confidence_threshold = 0.5f;
};

struct TemplateMatchingConfig {
    std::string method = "TM_CCOEFF_NORMED";
    float default_confidence_threshold = 0.8f;
};

struct ActionConfig {
    int adb_connection_pool_size = 5;
    int adb_retry_count = 3;
    int adb_retry_delay_ms = 100;
    int max_action_latency_ms = 200;
};

class Config {
public:
    ManagerConfig manager;
    CaptureConfig capture;
    AIConfig ai;
    TemplateMatchingConfig template_matching;
    ActionConfig action;
    
    static std::unique_ptr<Config> loadFromFile(const std::string& path);
    bool saveToFile(const std::string& path) const;
};

} // namespace hyperfarm
