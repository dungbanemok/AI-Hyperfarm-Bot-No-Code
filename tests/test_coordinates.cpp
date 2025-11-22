#include <gtest/gtest.h>
#include "core/coordinates.h"
#include <cmath>

using namespace hyperfarm;

// Helper function to compare floating point values with tolerance
bool floatEqual(float a, float b, float epsilon = 0.001f) {
    return std::abs(a - b) < epsilon;
}

// Test fixture for coordinate normalization tests
class CoordinateNormalizationTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Common test resolutions
        standard_size = cv::Size(1920, 1080);
        hd_size = cv::Size(1280, 720);
        fhd_size = cv::Size(1920, 1080);
        qhd_size = cv::Size(2560, 1440);
        uhd_size = cv::Size(3840, 2160);
        mobile_size = cv::Size(1080, 1920);  // Portrait
        tablet_size = cv::Size(1024, 768);
    }

    cv::Size standard_size;
    cv::Size hd_size;
    cv::Size fhd_size;
    cv::Size qhd_size;
    cv::Size uhd_size;
    cv::Size mobile_size;
    cv::Size tablet_size;
};

// ============================================================================
// Requirement 7.1: WHEN hệ thống nhận tọa độ đầu vào THEN hệ thống SHALL 
// chuẩn hóa về độ phân giải chuẩn 1920x1080 bằng floating point
// ============================================================================

TEST_F(CoordinateNormalizationTest, NormalizeToStandardResolution) {
    // Test normalization from standard resolution (should be identity)
    NormalizedCoord norm = CoordinateNormalizer::normalize(960, 540, 1920, 1080);
    EXPECT_TRUE(floatEqual(norm.x, 960.0f));
    EXPECT_TRUE(floatEqual(norm.y, 540.0f));
}

TEST_F(CoordinateNormalizationTest, NormalizeFromHDResolution) {
    // Test normalization from 1280x720 (HD)
    // Center point (640, 360) should map to (960, 540) in normalized space
    NormalizedCoord norm = CoordinateNormalizer::normalize(640, 360, 1280, 720);
    EXPECT_TRUE(floatEqual(norm.x, 960.0f));
    EXPECT_TRUE(floatEqual(norm.y, 540.0f));
}

TEST_F(CoordinateNormalizationTest, NormalizeFromQHDResolution) {
    // Test normalization from 2560x1440 (QHD)
    // Center point (1280, 720) should map to (960, 540) in normalized space
    NormalizedCoord norm = CoordinateNormalizer::normalize(1280, 720, 2560, 1440);
    EXPECT_TRUE(floatEqual(norm.x, 960.0f));
    EXPECT_TRUE(floatEqual(norm.y, 540.0f));
}

TEST_F(CoordinateNormalizationTest, NormalizeFromUHDResolution) {
    // Test normalization from 3840x2160 (4K UHD)
    // Center point (1920, 1080) should map to (960, 540) in normalized space
    NormalizedCoord norm = CoordinateNormalizer::normalize(1920, 1080, 3840, 2160);
    EXPECT_TRUE(floatEqual(norm.x, 960.0f));
    EXPECT_TRUE(floatEqual(norm.y, 540.0f));
}

TEST_F(CoordinateNormalizationTest, NormalizeUsesFloatingPoint) {
    // Verify that normalization uses floating point precision
    // Test with coordinates that would lose precision with integer math
    NormalizedCoord norm = CoordinateNormalizer::normalize(333, 222, 1000, 600);
    
    // Expected: (333/1000) * 1920 = 639.36, (222/600) * 1080 = 399.6
    EXPECT_TRUE(floatEqual(norm.x, 639.36f, 0.01f));
    EXPECT_TRUE(floatEqual(norm.y, 399.6f, 0.01f));
    
    // Verify it's not truncated to integer
    EXPECT_FALSE(floatEqual(norm.x, 639.0f, 0.01f));
    EXPECT_FALSE(floatEqual(norm.y, 399.0f, 0.01f));
}

// ============================================================================
// Requirement 7.2: WHEN hệ thống gửi lệnh Click THEN hệ thống SHALL 
// chuyển đổi tọa độ chuẩn hóa về độ phân giải thực tế của Worker
// ============================================================================

TEST_F(CoordinateNormalizationTest, DenormalizeToStandardResolution) {
    // Test denormalization to standard resolution
    NormalizedCoord norm(960.0f, 540.0f);
    cv::Point actual = CoordinateNormalizer::denormalize(norm, 1920, 1080);
    EXPECT_EQ(actual.x, 960);
    EXPECT_EQ(actual.y, 540);
}

TEST_F(CoordinateNormalizationTest, DenormalizeToHDResolution) {
    // Test denormalization to 1280x720 (HD)
    // Normalized (960, 540) should map to (640, 360) in HD
    NormalizedCoord norm(960.0f, 540.0f);
    cv::Point actual = CoordinateNormalizer::denormalize(norm, 1280, 720);
    EXPECT_EQ(actual.x, 640);
    EXPECT_EQ(actual.y, 360);
}

TEST_F(CoordinateNormalizationTest, DenormalizeToQHDResolution) {
    // Test denormalization to 2560x1440 (QHD)
    // Normalized (960, 540) should map to (1280, 720) in QHD
    NormalizedCoord norm(960.0f, 540.0f);
    cv::Point actual = CoordinateNormalizer::denormalize(norm, 2560, 1440);
    EXPECT_EQ(actual.x, 1280);
    EXPECT_EQ(actual.y, 720);
}

TEST_F(CoordinateNormalizationTest, DenormalizeToUHDResolution) {
    // Test denormalization to 3840x2160 (4K UHD)
    // Normalized (960, 540) should map to (1920, 1080) in UHD
    NormalizedCoord norm(960.0f, 540.0f);
    cv::Point actual = CoordinateNormalizer::denormalize(norm, 3840, 2160);
    EXPECT_EQ(actual.x, 1920);
    EXPECT_EQ(actual.y, 1080);
}

TEST_F(CoordinateNormalizationTest, DenormalizeToMobileResolution) {
    // Test denormalization to mobile portrait resolution (1080x1920)
    NormalizedCoord norm(960.0f, 540.0f);
    cv::Point actual = CoordinateNormalizer::denormalize(norm, 1080, 1920);
    // Expected: (960/1920) * 1080 = 540, (540/1080) * 1920 = 960
    EXPECT_EQ(actual.x, 540);
    EXPECT_EQ(actual.y, 960);
}

// ============================================================================
// Requirement 7.3: WHEN hệ thống lưu Action Point THEN hệ thống SHALL 
// lưu tọa độ dưới dạng chuẩn hóa trong JSON Metadata
// ============================================================================

TEST_F(CoordinateNormalizationTest, NormalizedCoordFromActual) {
    // Test static factory method for creating normalized coordinates
    NormalizedCoord norm = NormalizedCoord::fromActual(640, 360, hd_size);
    EXPECT_TRUE(floatEqual(norm.x, 960.0f));
    EXPECT_TRUE(floatEqual(norm.y, 540.0f));
}

TEST_F(CoordinateNormalizationTest, NormalizedCoordToActual) {
    // Test conversion back to actual coordinates
    NormalizedCoord norm(960.0f, 540.0f);
    cv::Point actual = norm.toActual(hd_size);
    EXPECT_EQ(actual.x, 640);
    EXPECT_EQ(actual.y, 360);
}

TEST_F(CoordinateNormalizationTest, ActionPointStorageFormat) {
    // Verify that action points can be stored and retrieved correctly
    // Simulate storing an action point from HD resolution
    cv::Point original_hd(640, 360);
    NormalizedCoord stored = NormalizedCoord::fromActual(original_hd.x, original_hd.y, hd_size);
    
    // Verify stored values are in normalized space
    EXPECT_TRUE(floatEqual(stored.x, 960.0f));
    EXPECT_TRUE(floatEqual(stored.y, 540.0f));
    
    // Simulate retrieving and using on QHD resolution
    cv::Point retrieved_qhd = stored.toActual(qhd_size);
    EXPECT_EQ(retrieved_qhd.x, 1280);
    EXPECT_EQ(retrieved_qhd.y, 720);
}

// ============================================================================
// Requirement 7.4: WHEN hệ thống trả về Bounding Box THEN hệ thống SHALL 
// đảm bảo tọa độ đã được chuẩn hóa
// ============================================================================

TEST_F(CoordinateNormalizationTest, NormalizedRectFromActual) {
    // Test rectangle normalization from HD resolution
    cv::Rect actual_rect(320, 180, 640, 360);  // HD rectangle
    NormalizedRect norm_rect = NormalizedRect::fromActual(actual_rect, hd_size);
    
    // Top-left should be (480, 270) in normalized space
    EXPECT_TRUE(floatEqual(norm_rect.top_left.x, 480.0f));
    EXPECT_TRUE(floatEqual(norm_rect.top_left.y, 270.0f));
    
    // Bottom-right should be (1440, 810) in normalized space
    EXPECT_TRUE(floatEqual(norm_rect.bottom_right.x, 1440.0f));
    EXPECT_TRUE(floatEqual(norm_rect.bottom_right.y, 810.0f));
}

TEST_F(CoordinateNormalizationTest, NormalizedRectToActual) {
    // Test rectangle denormalization to QHD resolution
    NormalizedRect norm_rect(NormalizedCoord(480.0f, 270.0f), 
                             NormalizedCoord(1440.0f, 810.0f));
    cv::Rect actual_rect = norm_rect.toActual(qhd_size);
    
    // Should map to (640, 360, 1280, 720) in QHD
    EXPECT_EQ(actual_rect.x, 640);
    EXPECT_EQ(actual_rect.y, 360);
    EXPECT_EQ(actual_rect.width, 1280);
    EXPECT_EQ(actual_rect.height, 720);
}

TEST_F(CoordinateNormalizationTest, BoundingBoxConsistency) {
    // Test that bounding boxes maintain consistency across resolutions
    cv::Rect original_hd(100, 100, 200, 150);
    
    // Normalize from HD
    NormalizedRect norm = NormalizedRect::fromActual(original_hd, hd_size);
    
    // Denormalize back to HD - should get same rectangle
    cv::Rect recovered_hd = norm.toActual(hd_size);
    EXPECT_EQ(recovered_hd.x, original_hd.x);
    EXPECT_EQ(recovered_hd.y, original_hd.y);
    EXPECT_EQ(recovered_hd.width, original_hd.width);
    EXPECT_EQ(recovered_hd.height, original_hd.height);
}

// ============================================================================
// Requirement 7.5: WHEN hệ thống xử lý Search Region THEN hệ thống SHALL 
// sử dụng tọa độ chuẩn hóa cho tất cả phép tính
// ============================================================================

TEST_F(CoordinateNormalizationTest, SearchRegionNormalization) {
    // Test search region normalization from various resolutions
    cv::Rect search_region_hd(400, 300, 480, 120);
    NormalizedRect norm_region = NormalizedRect::fromActual(search_region_hd, hd_size);
    
    // Verify normalized coordinates
    EXPECT_TRUE(floatEqual(norm_region.top_left.x, 600.0f));
    EXPECT_TRUE(floatEqual(norm_region.top_left.y, 450.0f));
    EXPECT_TRUE(floatEqual(norm_region.bottom_right.x, 1320.0f));
    EXPECT_TRUE(floatEqual(norm_region.bottom_right.y, 630.0f));
}

TEST_F(CoordinateNormalizationTest, SearchRegionAcrossResolutions) {
    // Define search region in HD
    cv::Rect search_hd(320, 180, 640, 360);
    NormalizedRect norm_region = NormalizedRect::fromActual(search_hd, hd_size);
    
    // Apply to different resolutions
    cv::Rect search_qhd = norm_region.toActual(qhd_size);
    cv::Rect search_uhd = norm_region.toActual(uhd_size);
    
    // Verify proportions are maintained
    // QHD should be 2x HD
    EXPECT_EQ(search_qhd.x, 640);
    EXPECT_EQ(search_qhd.y, 360);
    EXPECT_EQ(search_qhd.width, 1280);
    EXPECT_EQ(search_qhd.height, 720);
    
    // UHD should be 4x HD
    EXPECT_EQ(search_uhd.x, 1280);
    EXPECT_EQ(search_uhd.y, 720);
    EXPECT_EQ(search_uhd.width, 2560);
    EXPECT_EQ(search_uhd.height, 1440);
}

// ============================================================================
// Additional Edge Cases and Comprehensive Tests
// ============================================================================

TEST_F(CoordinateNormalizationTest, CornerCoordinates) {
    // Test corner coordinates (0,0) and (max,max)
    NormalizedCoord top_left = CoordinateNormalizer::normalize(0, 0, 1280, 720);
    EXPECT_TRUE(floatEqual(top_left.x, 0.0f));
    EXPECT_TRUE(floatEqual(top_left.y, 0.0f));
    
    NormalizedCoord bottom_right = CoordinateNormalizer::normalize(1280, 720, 1280, 720);
    EXPECT_TRUE(floatEqual(bottom_right.x, 1920.0f));
    EXPECT_TRUE(floatEqual(bottom_right.y, 1080.0f));
}

TEST_F(CoordinateNormalizationTest, RoundTripConsistency) {
    // Test round-trip conversion maintains consistency
    std::vector<cv::Size> test_sizes = {hd_size, fhd_size, qhd_size, uhd_size, tablet_size};
    
    for (const auto& size : test_sizes) {
        // Test various points
        std::vector<cv::Point> test_points = {
            cv::Point(0, 0),
            cv::Point(size.width / 2, size.height / 2),
            cv::Point(size.width - 1, size.height - 1),
            cv::Point(size.width / 4, size.height / 4)
        };
        
        for (const auto& pt : test_points) {
            NormalizedCoord norm = NormalizedCoord::fromActual(pt.x, pt.y, size);
            cv::Point recovered = norm.toActual(size);
            
            // Allow small rounding error
            EXPECT_NEAR(recovered.x, pt.x, 1);
            EXPECT_NEAR(recovered.y, pt.y, 1);
        }
    }
}

TEST_F(CoordinateNormalizationTest, NonStandardAspectRatio) {
    // Test with non-standard aspect ratios
    cv::Size custom_size(1366, 768);  // Common laptop resolution
    
    NormalizedCoord norm = CoordinateNormalizer::normalize(683, 384, 1366, 768);
    // Center point should map to center of normalized space
    EXPECT_TRUE(floatEqual(norm.x, 960.0f, 1.0f));
    EXPECT_TRUE(floatEqual(norm.y, 540.0f, 1.0f));
}

TEST_F(CoordinateNormalizationTest, PrecisionMaintenance) {
    // Test that precision is maintained through multiple conversions
    cv::Size size1(1280, 720);
    cv::Size size2(2560, 1440);
    
    cv::Point original(123, 456);
    
    // Convert: actual1 -> normalized -> actual2 -> normalized -> actual1
    NormalizedCoord norm1 = NormalizedCoord::fromActual(original.x, original.y, size1);
    cv::Point actual2 = norm1.toActual(size2);
    NormalizedCoord norm2 = NormalizedCoord::fromActual(actual2.x, actual2.y, size2);
    cv::Point recovered = norm2.toActual(size1);
    
    // Should recover original coordinates (with small rounding tolerance)
    EXPECT_NEAR(recovered.x, original.x, 2);
    EXPECT_NEAR(recovered.y, original.y, 2);
}

TEST_F(CoordinateNormalizationTest, RectangleWidthHeightCalculation) {
    // Verify that rectangle width and height are calculated correctly
    NormalizedRect norm_rect(NormalizedCoord(100.0f, 100.0f), 
                             NormalizedCoord(500.0f, 300.0f));
    
    cv::Rect actual = norm_rect.toActual(standard_size);
    
    // Width should be 500-100 = 400 in normalized, same in standard
    EXPECT_EQ(actual.width, 400);
    // Height should be 300-100 = 200 in normalized, same in standard
    EXPECT_EQ(actual.height, 200);
}

TEST_F(CoordinateNormalizationTest, MultipleResolutionConsistency) {
    // Test that the same normalized coordinate produces proportional results
    NormalizedCoord norm(960.0f, 540.0f);  // Center of normalized space
    
    cv::Point hd = norm.toActual(hd_size);
    cv::Point fhd = norm.toActual(fhd_size);
    cv::Point qhd = norm.toActual(qhd_size);
    
    // All should be at center of their respective resolutions
    EXPECT_EQ(hd.x, 640);
    EXPECT_EQ(hd.y, 360);
    EXPECT_EQ(fhd.x, 960);
    EXPECT_EQ(fhd.y, 540);
    EXPECT_EQ(qhd.x, 1280);
    EXPECT_EQ(qhd.y, 720);
}

// Main function
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
