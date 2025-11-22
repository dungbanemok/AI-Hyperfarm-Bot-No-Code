#pragma once

#include "types.h"
#include "coordinates.h"
#include <opencv2/core.hpp>
#include <string>
#include <vector>
#include <map>

namespace hyperfarm {

// Capture Layer Interface
struct CaptureResult {
    bool success;
    cv::Mat image;  // Always 1920x1080 if success
    CaptureMethod method;
    double timestamp_ms;
    std::string error_msg;
};

class ICaptureLayer {
public:
    virtual ~ICaptureLayer() = default;
    
    virtual CaptureResult capture() = 0;
    virtual float getFailureRate() const = 0;
};

// Perception Layer Interface
struct BoundingBox {
    NormalizedCoord top_left;
    NormalizedCoord bottom_right;
    std::string class_name;
    float confidence;
};

struct PerceptionResult {
    bool found;
    std::vector<BoundingBox> detections;
    float confidence;
    double processing_time_ms;
};

class IPerceptionLayer {
public:
    virtual ~IPerceptionLayer() = default;
    
    virtual PerceptionResult detectAI(const cv::Mat& image, const std::string& target_class) = 0;
    virtual PerceptionResult matchTemplate(const cv::Mat& image, const std::string& template_id) = 0;
};

// Decision Layer Interface
struct DecisionResult {
    NodeId next_node;
    bool should_continue;
    std::map<std::string, std::string> context;
};

class IDecisionLayer {
public:
    virtual ~IDecisionLayer() = default;
    
    virtual DecisionResult evaluate(const NodeId& current_node, const PerceptionResult& perception) = 0;
};

// Action Layer Interface
struct Action {
    ActionType type;
    NormalizedCoord coord;
    NormalizedCoord from_coord;
    NormalizedCoord to_coord;
    int duration_ms;
};

struct ActionResult {
    bool success;
    double latency_ms;
    std::string error_msg;
};

class IActionLayer {
public:
    virtual ~IActionLayer() = default;
    
    virtual ActionResult execute(const Action& action) = 0;
};

} // namespace hyperfarm
