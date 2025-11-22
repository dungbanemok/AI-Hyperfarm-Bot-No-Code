# Implementation Plan

- [x] 1. Thiết lập cấu trúc dự án và core interfaces




  - Tạo cấu trúc thư mục cho project (src/core, src/capture, src/perception, src/action, src/ui, include/, models/, templates/)
  - Định nghĩa các interface và base classes chính (Worker, Layer interfaces)
  - Thiết lập CMake build system với dependencies (Qt, OpenCV, ONNXRuntime, Boost)
  - Tạo file config.json mẫu với các tham số cấu hình
  - _Requirements: 18.4_

- [x] 2. Implement hệ thống chuẩn hóa tọa độ





  - Implement struct NormalizedCoord với các phương thức fromActual() và toActual()
  - Implement struct NormalizedRect với các phương thức chuyển đổi
  - Implement class CoordinateNormalizer với các hàm normalize() và denormalize()
  - _Requirements: 7.1, 7.2, 7.3, 7.4, 7.5_

- [x] 2.1 Viết unit tests cho coordinate normalization






  - Viết unit tests cho các phép chuyển đổi tọa độ với nhiều độ phân giải khác nhau
  - _Requirements: 7.1, 7.2, 7.3, 7.4, 7.5_

- [ ] 3. Xây dựng EmulatorWindowMapper để fix lỗi Device Mismatch
  - Implement class EmulatorWindowMapper với phương thức mapEmulator()
  - Implement logic ánh xạ ADB Port ↔ HWND dựa trên emulator internal ID
  - Implement phương thức verifyMapping() để xác minh mapping
  - Implement getMappingSuccessRate() để thu thập metrics
  - _Requirements: 3.1, 3.2, 3.3, 12.2_

- [ ]* 3.1 Viết unit tests cho EmulatorWindowMapper
  - Viết unit tests cho mapping logic với mock emulator data
  - _Requirements: 3.1, 3.2, 3.3, 12.2_

- [ ] 4. Implement Capture Layer với Layered Capture Architecture
  - Implement class CaptureLayer với phương thức capture()
  - Implement windowCapture() sử dụng WinAPI (BitBlt hoặc PrintWindow)
  - Implement adbCapture() sử dụng ADB screencap command
  - Implement logic fallback: thử Window Capture trước, nếu fail thì dùng ADB
  - Implement normalizeResolution() để resize ảnh về 1920x1080
  - Implement BufferPool<cv::Mat> để tái sử dụng memory
  - Implement getFailureRate() để thu thập Capture Failure Rate metrics
  - _Requirements: 4.1, 4.2, 4.3, 4.4, 3.5, 12.1_

- [ ]* 4.1 Viết unit tests cho Capture Layer
  - Viết unit tests cho từng phương thức capture
  - _Requirements: 4.1, 4.2, 4.3, 4.4, 3.5, 12.1_

- [ ] 5. Implement AI Inference Engine
  - Implement class AIInferenceEngine với ONNXRuntime
  - Implement loadModel() để load YOLOv8 ONNX model với TensorRT/CUDA support
  - Implement preprocessImage() để chuẩn bị input tensor
  - Implement postprocess() để parse output tensor thành Detection objects
  - Implement applyNMS() để loại bỏ bounding box trùng lặp
  - Implement inference_mutex_ để đảm bảo thread-safe khi nhiều Worker gọi đồng thời
  - Chuẩn hóa tọa độ bounding box về 1920x1080 trong postprocess()
  - _Requirements: 5.1, 5.2, 5.3, 5.4, 5.5_

- [ ]* 5.1 Viết unit tests cho AI Inference Engine
  - Viết unit tests với mock ONNX model
  - _Requirements: 5.1, 5.2, 5.3, 5.4, 5.5_

- [ ] 6. Implement Template Matcher
  - Implement class TemplateMatcher sử dụng OpenCV
  - Implement match() với cv::matchTemplate() và cv::TM_CCOEFF_NORMED
  - Implement extractSearchRegion() để crop ảnh theo Search Region
  - Implement convertToNormalized() để chuẩn hóa tọa độ kết quả
  - Implement match_mutex_ để đảm bảo thread-safe
  - _Requirements: 6.1, 6.2, 6.3, 6.4, 6.5_

- [ ]* 6.1 Viết unit tests cho Template Matcher
  - Viết unit tests với template images mẫu
  - _Requirements: 6.1, 6.2, 6.3, 6.4, 6.5_

- [ ] 7. Implement Perception Layer
  - Implement class PerceptionLayer
  - Implement detectAI() để gọi AIInferenceEngine và parse kết quả
  - Implement matchTemplate() để gọi TemplateMatcher và parse kết quả
  - _Requirements: 5.1, 6.1_

- [ ]* 7.1 Viết unit tests cho Perception Layer
  - Viết unit tests với mock AI Engine và Template Matcher
  - _Requirements: 5.1, 6.1_

- [ ] 8. Implement Action Layer
  - Implement class ActionLayer
  - Implement executeClick() để gửi lệnh click qua ADB hoặc WinAPI
  - Implement executeSwipe() để gửi lệnh swipe qua ADB
  - Implement executeWait() để delay execution
  - Implement denormalizeCoord() để chuyển tọa độ chuẩn về tọa độ thực tế
  - Implement ADBConnectionPool để tái sử dụng ADB connections
  - Implement retry logic với exponential backoff cho ADB commands
  - _Requirements: 7.2, 14.4, 14.5, 16.4_

- [ ]* 8.1 Viết unit tests cho Action Layer
  - Viết unit tests với mock ADB connection
  - _Requirements: 7.2, 14.4, 14.5, 16.4_

- [ ] 9. Implement Decision Layer và Flow Executor
  - Implement class FlowExecutor để load và parse JSON Graph
  - Implement buildFSM() để chuyển JSON Graph thành FSM
  - Implement getNextNode() để quyết định node tiếp theo dựa trên decision result
  - Implement class DecisionLayer để evaluate perception result
  - Implement logic cho các loại node: template_match, ai_detection, action, wait, loop
  - _Requirements: 1.4, 15.1, 15.2, 15.3, 15.4, 15.5_

- [ ]* 9.1 Viết unit tests cho Decision Layer và Flow Executor
  - Viết unit tests với mock flow graphs
  - _Requirements: 1.4, 15.1, 15.2, 15.3, 15.4, 15.5_

- [ ] 10. Implement Worker Thread
  - Implement class WorkerThread với std::thread
  - Implement processLoop() để thực thi flow: Capture → Perception → Decision → Action
  - Implement FPSCounter để đo và báo cáo FPS
  - Integrate tất cả các layer: CaptureLayer, PerceptionLayer, DecisionLayer, ActionLayer
  - Implement stop(), pause(), resume() methods
  - Implement getStatus() và getMetrics() để báo cáo về Manager
  - _Requirements: 2.2, 4.5, 9.5_

- [ ]* 10.1 Viết integration tests cho Worker Thread
  - Viết integration tests cho worker loop
  - _Requirements: 2.2, 4.5, 9.5_

- [ ] 11. Implement Checkpoint Manager
  - Implement class CheckpointManager
  - Implement saveCheckpoint() để lưu FSM state, current node và context vào JSON
  - Implement loadCheckpoint() để restore state từ JSON
  - Implement atomic write để tránh corruption
  - Integrate checkpoint vào WorkerThread: save sau mỗi node, load khi khởi động
  - _Requirements: 13.1, 13.2, 13.3, 13.4, 13.5_

- [ ]* 11.1 Viết unit tests cho Checkpoint Manager
  - Viết unit tests cho save/load logic
  - _Requirements: 13.1, 13.2, 13.3, 13.4, 13.5_

- [ ] 12. Implement Worker Manager
  - Implement class WorkerManager
  - Implement createWorker() để tạo WorkerThread mới
  - Implement startWorker(), stopWorker(), restartWorker()
  - Implement getWorkerStatus() và getAllMetrics()
  - Integrate EmulatorWindowMapper để ánh xạ emulator khi tạo worker
  - Implement workers_mutex_ để đảm bảo thread-safe
  - Implement error isolation: nếu một worker crash, không ảnh hưởng worker khác
  - _Requirements: 2.1, 2.2, 2.4, 14.2_

- [ ]* 12.1 Viết integration tests cho Worker Manager
  - Viết integration tests cho worker lifecycle
  - _Requirements: 2.1, 2.2, 2.4, 14.2_

- [ ] 13. Implement Manager UI (Qt)
  - Thiết kế UI layout 3 cột (Left: Worker list, Center: Status table, Right: Details)
  - Implement class ManagerUI kế thừa QMainWindow
  - Implement QTableView để hiển thị Worker status (FSM State, Current Node, FPS)
  - Implement QTimer để cập nhật UI mỗi 500ms
  - Implement slots cho start/stop/pause worker buttons
  - Connect UI với WorkerManager để lấy status và metrics
  - _Requirements: 9.1, 9.2, 9.3, 9.4, 9.5_

- [ ] 14. Implement Template Creator (Qt)
  - Thiết kế UI cho Template Creator
  - Implement class TemplateCreator kế thừa QWidget
  - Implement loadScreenshot() để hiển thị ảnh chụp màn hình
  - Implement mouse event handlers để vẽ Search Region (drag rectangle)
  - Implement mouse click handler để đặt Action Point
  - Implement saveTemplate() để serialize thành JSON Metadata và lưu template image
  - Implement loadTemplate() để deserialize và hiển thị lại
  - _Requirements: 1.1, 1.2, 10.1, 10.2, 10.3, 10.4, 10.5_

- [ ] 15. Implement Flow Builder (Qt)
  - Thiết kế UI cho Flow Builder với Qt Graphics View Framework
  - Implement class FlowBuilder kế thừa QWidget
  - Implement class FlowNode kế thừa QGraphicsItem
  - Implement drag-and-drop để thêm node vào canvas
  - Implement node connection logic (draw edges giữa các node)
  - Implement node configuration dialog cho mỗi loại node
  - Implement saveFlow() để serialize graph thành JSON
  - Implement loadFlow() để deserialize và hiển thị lại graph
  - _Requirements: 1.3, 1.4, 1.5, 11.1, 11.2, 11.3, 11.4, 11.5_

- [ ]* 16. Implement Import/Export functionality
  - Implement export Flow: tạo JSON file từ FlowGraph
  - Implement import Flow: validate và load JSON file vào FlowBuilder
  - Implement export Template: tạo package (zip) chứa JSON Metadata và image
  - Implement import Template: extract package và load vào TemplateCreator
  - Implement error handling cho invalid files
  - _Requirements: 17.1, 17.2, 17.3, 17.4, 17.5_

- [ ]* 16.1 Viết unit tests cho Import/Export
  - Viết unit tests cho import/export logic
  - _Requirements: 17.1, 17.2, 17.3, 17.4, 17.5_

- [ ] 17. Implement Metrics và Reporting System
  - Implement metrics collection trong các layer (Capture, Perception, Action)
  - Implement WorkerMetrics aggregation trong WorkerManager
  - Implement calculateCaptureFailureRate() và calculateMappingSuccessRate()
  - Implement metrics export: JSON và CSV format
  - _Requirements: 12.1, 12.2, 12.3, 12.4, 12.5_

- [ ]* 17.1 Implement metrics visualization
  - Implement metrics visualization trong Manager UI (charts)
  - _Requirements: 12.1, 12.2, 12.3, 12.4, 12.5_

- [ ]* 17.2 Viết unit tests cho Metrics System
  - Viết unit tests cho metrics calculation
  - _Requirements: 12.1, 12.2, 12.3, 12.4, 12.5_

- [ ] 18. Implement Error Handling và Recovery
  - Implement exception handling trong tất cả các layer
  - Implement error logging với timestamp và worker ID
  - Implement worker isolation: catch exception trong WorkerThread, không crash Manager
  - Implement auto-restart logic cho worker không phản hồi
  - Implement graceful shutdown: cleanup resources khi stop worker
  - _Requirements: 14.1, 14.2, 14.3, 14.4, 14.5, 2.4_

- [ ]* 18.1 Viết tests cho error scenarios
  - Viết tests cho error scenarios
  - _Requirements: 14.1, 14.2, 14.3, 14.4, 14.5, 2.4_

- [ ]* 19. Performance Optimization
  - Implement AI inference batching nếu có nhiều request đồng thời
  - Optimize image processing với SIMD instructions nếu CPU hỗ trợ
  - Profile và optimize critical paths để đạt <200ms latency từ capture đến action
  - Benchmark và đo FPS, latency
  - _Requirements: 16.1, 16.2, 16.3, 16.4, 16.5_

- [ ]* 20. Implement Hot-reload và Scalability features
  - Implement hot-reload cho AI model: reload model mà không stop worker
  - Implement auto-detection cho emulator mới: scan và map HWND tự động
  - Implement plugin architecture cho custom node types
  - Test scalability: chạy 20+ workers đồng thời và đo performance
  - Optimize để hỗ trợ 50 workers như yêu cầu
  - _Requirements: 18.1, 18.2, 18.3, 18.5_

- [ ] 21. MVP Integration Testing
  - Viết integration tests cho toàn bộ pipeline: Capture → Perception → Decision → Action
  - Test với emulator thật: chạy flow đơn giản và verify kết quả
  - Test multi-worker scenario: chạy 5 workers đồng thời
  - Verify Capture Failure Rate ≤ 1% như yêu cầu
  - _Requirements: 2.1, 3.5_

- [ ]* 21.1 Extended System Testing
  - Test error scenarios: emulator disconnect, ADB timeout, model load failure
  - Test với 10-20 workers đồng thời
  - Verify 99.9% uptime trong thời gian test dài (24h+)
  - _Requirements: 2.1, 3.5_

- [ ]* 22. Documentation và Deployment
  - Viết README.md với hướng dẫn build và setup
  - Viết User Guide cho Template Creator và Flow Builder
  - Viết Developer Guide cho architecture và code structure
  - Tạo example flows và templates
  - Setup CI/CD pipeline
  - Tạo installer hoặc deployment package
  - _Requirements: N/A (Documentation)_

- [ ] 23. MVP Checkpoint - Đảm bảo tất cả tests pass
  - Chạy tất cả unit tests và integration tests
  - Verify MVP requirements: auto-farm cơ bản trên 5 workers với Capture Failure Rate ≤ 1%
  - Fix các bugs phát hiện trong quá trình testing
  - Hỏi user nếu có vấn đề phát sinh
