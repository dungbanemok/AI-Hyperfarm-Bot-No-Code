#include "perception/perception_layer.h"

namespace hyperfarm {

PerceptionLayer::PerceptionLayer(WorkerId id)
    : worker_id_(id) {
}

PerceptionLayer::~PerceptionLayer() {
}

void PerceptionLayer::setAIEngine(std::shared_ptr<AIInferenceEngine> engine) {
    ai_engine_ = engine;
}

void PerceptionLayer::setTemplateMatcher(std::shared_ptr<TemplateMatcher> matcher) {
    template_matcher_ = matcher;
}

PerceptionResult PerceptionLayer::detectAI(const cv::Mat& image, const std::string& target_class) {
    // TODO: Implement AI detection
    PerceptionResult result;
    result.found = false;
    return result;
}

PerceptionResult PerceptionLayer::matchTemplate(const cv::Mat& image, const std::string& template_id) {
    // TODO: Implement template matching
    PerceptionResult result;
    result.found = false;
    return result;
}

} // namespace hyperfarm
