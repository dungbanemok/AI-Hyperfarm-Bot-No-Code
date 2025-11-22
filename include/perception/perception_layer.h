#pragma once

#include "core/layer_interface.h"
#include "core/worker_interface.h"
#include <memory>

namespace hyperfarm {

// Forward declarations
class AIInferenceEngine;
class TemplateMatcher;

class PerceptionLayer : public IPerceptionLayer {
public:
    PerceptionLayer(WorkerId id);
    virtual ~PerceptionLayer();
    
    void setAIEngine(std::shared_ptr<AIInferenceEngine> engine);
    void setTemplateMatcher(std::shared_ptr<TemplateMatcher> matcher);
    
    PerceptionResult detectAI(const cv::Mat& image, const std::string& target_class) override;
    PerceptionResult matchTemplate(const cv::Mat& image, const std::string& template_id) override;
    
private:
    WorkerId worker_id_;
    std::shared_ptr<AIInferenceEngine> ai_engine_;
    std::shared_ptr<TemplateMatcher> template_matcher_;
};

} // namespace hyperfarm
