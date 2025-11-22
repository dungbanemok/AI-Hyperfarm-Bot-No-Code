#pragma once

#include "types.h"
#include <string>
#include <atomic>

namespace hyperfarm {

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

struct EmulatorInfo {
    WorkerId worker_id;
    std::string emulator_internal_id;
    void* hwnd;  // HWND on Windows
    int adb_port;
    int actual_width;
    int actual_height;
};

// Base interface for Worker
class IWorker {
public:
    virtual ~IWorker() = default;
    
    virtual void run() = 0;
    virtual void stop() = 0;
    virtual void pause() = 0;
    virtual void resume() = 0;
    
    virtual WorkerStatus getStatus() const = 0;
    virtual WorkerMetrics getMetrics() const = 0;
};

} // namespace hyperfarm
