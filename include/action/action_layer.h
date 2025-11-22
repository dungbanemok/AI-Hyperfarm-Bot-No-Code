#pragma once

#include "core/layer_interface.h"
#include "core/worker_interface.h"
#include <opencv2/core.hpp>

namespace hyperfarm {

class ActionLayer : public IActionLayer {
public:
    ActionLayer(WorkerId id, void* hwnd, int adb_port);
    virtual ~ActionLayer();
    
    ActionResult execute(const Action& action) override;
    
private:
    ActionResult executeClick(const NormalizedCoord& coord);
    ActionResult executeSwipe(const NormalizedCoord& from, const NormalizedCoord& to, int duration_ms);
    ActionResult executeWait(int duration_ms);
    
    cv::Point denormalizeCoord(const NormalizedCoord& coord);
    
    WorkerId worker_id_;
    void* hwnd_;
    int adb_port_;
    cv::Size actual_resolution_;
};

} // namespace hyperfarm
