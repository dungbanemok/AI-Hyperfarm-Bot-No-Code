#pragma once

#include <opencv2/core.hpp>

namespace hyperfarm {

// Normalized coordinate system (1920x1080 standard)
struct NormalizedCoord {
    float x;  // 0.0 to 1920.0
    float y;  // 0.0 to 1080.0
    
    NormalizedCoord() : x(0.0f), y(0.0f) {}
    NormalizedCoord(float x_, float y_) : x(x_), y(y_) {}
    
    static NormalizedCoord fromActual(int actual_x, int actual_y, const cv::Size& actual_size);
    cv::Point toActual(const cv::Size& actual_size) const;
};

struct NormalizedRect {
    NormalizedCoord top_left;
    NormalizedCoord bottom_right;
    
    NormalizedRect() = default;
    NormalizedRect(const NormalizedCoord& tl, const NormalizedCoord& br)
        : top_left(tl), bottom_right(br) {}
    
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

} // namespace hyperfarm
