#include "action/action_layer.h"

namespace hyperfarm {

ActionLayer::ActionLayer(WorkerId id, void* hwnd, int adb_port)
    : worker_id_(id)
    , hwnd_(hwnd)
    , adb_port_(adb_port)
    , actual_resolution_(1920, 1080) {
}

ActionLayer::~ActionLayer() {
}

ActionResult ActionLayer::execute(const Action& action) {
    ActionResult result;
    
    switch (action.type) {
        case ActionType::CLICK:
            return executeClick(action.coord);
        case ActionType::SWIPE:
            return executeSwipe(action.from_coord, action.to_coord, action.duration_ms);
        case ActionType::WAIT:
            return executeWait(action.duration_ms);
        default:
            result.success = false;
            result.error_msg = "Unknown action type";
            return result;
    }
}

ActionResult ActionLayer::executeClick(const NormalizedCoord& coord) {
    // TODO: Implement click via ADB or WinAPI
    ActionResult result;
    result.success = false;
    result.error_msg = "Not implemented";
    return result;
}

ActionResult ActionLayer::executeSwipe(const NormalizedCoord& from, const NormalizedCoord& to, int duration_ms) {
    // TODO: Implement swipe via ADB
    ActionResult result;
    result.success = false;
    result.error_msg = "Not implemented";
    return result;
}

ActionResult ActionLayer::executeWait(int duration_ms) {
    // TODO: Implement wait
    ActionResult result;
    result.success = true;
    result.latency_ms = duration_ms;
    return result;
}

cv::Point ActionLayer::denormalizeCoord(const NormalizedCoord& coord) {
    return coord.toActual(actual_resolution_);
}

} // namespace hyperfarm
