#include "core/coordinates.h"

namespace hyperfarm {

NormalizedCoord NormalizedCoord::fromActual(int actual_x, int actual_y, const cv::Size& actual_size) {
    return CoordinateNormalizer::normalize(actual_x, actual_y, actual_size.width, actual_size.height);
}

cv::Point NormalizedCoord::toActual(const cv::Size& actual_size) const {
    return CoordinateNormalizer::denormalize(*this, actual_size.width, actual_size.height);
}

NormalizedRect NormalizedRect::fromActual(const cv::Rect& rect, const cv::Size& actual_size) {
    NormalizedCoord tl = NormalizedCoord::fromActual(rect.x, rect.y, actual_size);
    NormalizedCoord br = NormalizedCoord::fromActual(rect.x + rect.width, rect.y + rect.height, actual_size);
    return NormalizedRect(tl, br);
}

cv::Rect NormalizedRect::toActual(const cv::Size& actual_size) const {
    cv::Point tl_pt = top_left.toActual(actual_size);
    cv::Point br_pt = bottom_right.toActual(actual_size);
    return cv::Rect(tl_pt.x, tl_pt.y, br_pt.x - tl_pt.x, br_pt.y - tl_pt.y);
}

NormalizedCoord CoordinateNormalizer::normalize(int x, int y, int actual_width, int actual_height) {
    float norm_x = (static_cast<float>(x) / actual_width) * STANDARD_WIDTH;
    float norm_y = (static_cast<float>(y) / actual_height) * STANDARD_HEIGHT;
    return NormalizedCoord(norm_x, norm_y);
}

cv::Point CoordinateNormalizer::denormalize(const NormalizedCoord& coord, int actual_width, int actual_height) {
    int actual_x = static_cast<int>((coord.x / STANDARD_WIDTH) * actual_width);
    int actual_y = static_cast<int>((coord.y / STANDARD_HEIGHT) * actual_height);
    return cv::Point(actual_x, actual_y);
}

} // namespace hyperfarm
