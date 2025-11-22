#pragma once

#include <string>
#include <cstdint>

namespace hyperfarm {

// Type aliases
using WorkerId = uint32_t;
using NodeId = std::string;

// Enums
enum class FSMState {
    IDLE,
    RUNNING,
    PAUSED,
    ERROR,
    STOPPED
};

enum class CaptureMethod {
    WINDOW_CAPTURE,
    ADB_CAPTURE
};

enum class ActionType {
    CLICK,
    SWIPE,
    WAIT
};

enum class NodeType {
    TEMPLATE_MATCH,
    AI_DETECTION,
    ACTION,
    WAIT,
    LOOP,
    DECISION
};

} // namespace hyperfarm
