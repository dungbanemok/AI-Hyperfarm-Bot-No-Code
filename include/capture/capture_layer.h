#pragma once

#include "core/layer_interface.h"
#include "core/worker_interface.h"
#include <memory>

namespace hyperfarm {

// Forward declarations
class BufferPool;

class CaptureLayer : public ICaptureLayer {
public:
    CaptureLayer(WorkerId id, void* hwnd, int adb_port);
    virtual ~CaptureLayer();
    
    CaptureResult capture() override;
    float getFailureRate() const override;
    
private:
    CaptureResult windowCapture();
    CaptureResult adbCapture();
    cv::Mat normalizeResolution(const cv::Mat& img);
    
    WorkerId worker_id_;
    void* hwnd_;
    int adb_port_;
    
    int capture_attempts_;
    int capture_failures_;
};

} // namespace hyperfarm
