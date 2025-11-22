# Requirements Document

## Introduction

AI Hyperfarm Bot là một hệ thống tự động hóa farming đa Worker được xây dựng bằng C/C++ với khả năng No-Code, cho phép người dùng cuối tạo và chỉnh sửa logic bot mà không cần viết code. Hệ thống phải đạt độ ổn định 99.9% uptime khi vận hành trên 20+ Worker đồng thời, với khả năng chụp ảnh, nhận diện AI và điều khiển giả lập Android thông qua ADB và WinAPI.

## Glossary

- **Worker**: Một instance giả lập Android (emulator) được quản lý bởi hệ thống Bot
- **Manager**: Module trung tâm điều phối và giám sát tất cả Worker
- **FSM (Finite State Machine)**: Máy trạng thái hữu hạn điều khiển logic hành vi của Bot
- **Node**: Một đơn vị logic trong Flow Builder đại diện cho Action hoặc Decision
- **Template**: Mẫu hình ảnh được sử dụng để nhận diện đối tượng trên màn hình
- **Search Region**: Vùng tìm kiếm được định nghĩa trên màn hình để nhận diện Template
- **Action Point**: Điểm tọa độ được định nghĩa để thực hiện thao tác Click
- **HWND**: Handle của cửa sổ Windows (Window Handle)
- **ADB (Android Debug Bridge)**: Công cụ dòng lệnh để giao tiếp với thiết bị Android
- **Device Mismatch**: Lỗi xảy ra khi hệ thống gửi lệnh đến sai Worker
- **ADB Routing Error**: Lỗi định tuyến lệnh ADB đến sai port/device
- **Capture Failure Rate**: Tỷ lệ phần trăm lỗi khi chụp ảnh màn hình
- **ONNXRuntime**: Framework suy luận AI cho mô hình ONNX
- **YOLOv8**: Mô hình nhận diện đối tượng thời gian thực
- **NMS (Non-Maximum Suppression)**: Thuật toán loại bỏ bounding box trùng lặp
- **Bounding Box**: Hình chữ nhật bao quanh đối tượng được nhận diện
- **Normalized Coordinates**: Tọa độ được chuẩn hóa theo độ phân giải chuẩn 1920x1080
- **Window Capture**: Phương pháp chụp ảnh trực tiếp từ cửa sổ Windows qua WinAPI
- **ADB Atomic Chain**: Chuỗi lệnh ADB được thực thi nguyên tử làm phương án dự phòng
- **Inference**: Quá trình suy luận của mô hình AI
- **FPS (Frames Per Second)**: Số khung hình xử lý được mỗi giây
- **JSON Graph**: Biểu đồ logic Bot được lưu dưới dạng JSON
- **JSON Metadata**: Dữ liệu mô tả Template và Action được lưu dưới dạng JSON
- **MVP (Minimum Viable Product)**: Sản phẩm khả thi tối thiểu
- **Qt Graphics View Framework**: Framework đồ họa của Qt để xây dựng Node Editor
- **CUDA/TensorRT**: Công nghệ tăng tốc GPU của NVIDIA

## Requirements

### Requirement 1

**User Story:** Là một người dùng cuối, tôi muốn tạo logic Bot mà không cần viết code, để tôi có thể tùy chỉnh hành vi farming theo nhu cầu của mình

#### Acceptance Criteria

1. WHEN người dùng mở Template Creator THEN hệ thống SHALL hiển thị giao diện cho phép vẽ Search Region và đặt Action Point trên ảnh chụp màn hình
2. WHEN người dùng định nghĩa một Template THEN hệ thống SHALL lưu tất cả thông tin (Search Region, Action Point, hình ảnh mẫu) dưới dạng JSON Metadata
3. WHEN người dùng mở Flow Builder THEN hệ thống SHALL hiển thị Node Editor cho phép kéo thả và kết nối các Node
4. WHEN người dùng tạo một Flow THEN hệ thống SHALL lưu toàn bộ logic dưới dạng JSON Graph mà không yêu cầu code
5. WHEN người dùng chỉnh sửa Flow THEN hệ thống SHALL cho phép thêm, xóa, sửa Node và kết nối mà không cần biên dịch lại

### Requirement 2

**User Story:** Là một quản trị viên hệ thống, tôi muốn hệ thống đạt độ ổn định 99.9% uptime khi vận hành 20+ Worker, để đảm bảo hoạt động farming liên tục

#### Acceptance Criteria

1. WHEN hệ thống vận hành 20 Worker đồng thời THEN hệ thống SHALL duy trì hoạt động ổn định với uptime tối thiểu 99.9%
2. WHEN một Worker gặp lỗi THEN hệ thống SHALL cô lập lỗi và tiếp tục vận hành các Worker còn lại
3. WHEN hệ thống khởi động lại THEN hệ thống SHALL phục hồi trạng thái của tất cả Worker từ checkpoint cuối cùng
4. WHEN hệ thống phát hiện Worker không phản hồi THEN hệ thống SHALL tự động khởi động lại Worker đó
5. WHEN hệ thống ghi log THEN hệ thống SHALL ghi lại tất cả sự kiện quan trọng với timestamp và Worker ID

### Requirement 3

**User Story:** Là một kỹ sư hệ thống, tôi muốn loại bỏ hoàn toàn lỗi Device Mismatch và ADB Routing, để đảm bảo mỗi lệnh luôn được gửi đến đúng Worker

#### Acceptance Criteria

1. WHEN hệ thống ánh xạ Worker THEN hệ thống SHALL tạo mapping đáng tin cậy giữa ADB Port và HWND dựa trên ID nội bộ của giả lập
2. WHEN hệ thống gửi lệnh đến Worker THEN hệ thống SHALL xác minh Worker ID trước khi thực thi lệnh
3. WHEN hệ thống chụp ảnh từ Worker THEN hệ thống SHALL đảm bảo ảnh chụp được khớp với Worker ID yêu cầu
4. WHEN hệ thống phát hiện Device Mismatch THEN hệ thống SHALL ghi log chi tiết và thử lại với cơ chế xác minh bổ sung
5. WHEN Capture Failure Rate được đo THEN hệ thống SHALL đạt tỷ lệ lỗi chụp ảnh ≤ 1%

### Requirement 4

**User Story:** Là một kỹ sư hệ thống, tôi muốn hệ thống chụp ảnh màn hình nhanh và đáng tin cậy, để đảm bảo AI có dữ liệu đầu vào chính xác

#### Acceptance Criteria

1. WHEN hệ thống chụp ảnh THEN hệ thống SHALL ưu tiên sử dụng Window Capture qua WinAPI
2. WHEN Window Capture thất bại THEN hệ thống SHALL tự động chuyển sang ADB Atomic Chain làm phương án dự phòng
3. WHEN hệ thống thực hiện Window Capture THEN hệ thống SHALL sử dụng EmulatorWindowMapper để ánh xạ HWND chính xác
4. WHEN hệ thống chụp ảnh từ nhiều Worker THEN hệ thống SHALL đảm bảo không có xung đột tài nguyên giữa các luồng
5. WHEN hệ thống đo FPS THEN hệ thống SHALL đạt tốc độ xử lý tối thiểu 10 FPS cho mỗi Worker

### Requirement 5

**User Story:** Là một kỹ sư AI, tôi muốn hệ thống suy luận AI nhanh và chính xác, để nhận diện đối tượng trên màn hình trong thời gian thực

#### Acceptance Criteria

1. WHEN hệ thống khởi động THEN hệ thống SHALL load mô hình YOLOv8 một lần duy nhất vào bộ nhớ
2. WHEN hệ thống thực hiện inference THEN hệ thống SHALL sử dụng ONNXRuntime với CUDA hoặc TensorRT để tăng tốc GPU
3. WHEN hệ thống nhận diện đối tượng THEN hệ thống SHALL áp dụng NMS để loại bỏ bounding box trùng lặp
4. WHEN hệ thống trả về kết quả THEN hệ thống SHALL chuẩn hóa tọa độ bounding box về độ phân giải chuẩn 1920x1080
5. WHEN nhiều Worker yêu cầu inference đồng thời THEN hệ thống SHALL xử lý tuần tự với cơ chế queue và mutex

### Requirement 6

**User Story:** Là một kỹ sư hệ thống, tôi muốn hệ thống hỗ trợ Template Matching, để nhận diện đối tượng khi AI không khả dụng hoặc không phù hợp

#### Acceptance Criteria

1. WHEN người dùng định nghĩa Template THEN hệ thống SHALL lưu hình ảnh mẫu và Search Region
2. WHEN hệ thống thực hiện Template Matching THEN hệ thống SHALL sử dụng OpenCV C++ để tìm kiếm mẫu trong Search Region
3. WHEN Template được tìm thấy THEN hệ thống SHALL trả về tọa độ được chuẩn hóa về độ phân giải 1920x1080
4. WHEN Template không được tìm thấy THEN hệ thống SHALL trả về kết quả null và ghi log
5. WHEN hệ thống so sánh Template THEN hệ thống SHALL sử dụng ngưỡng confidence có thể cấu hình

### Requirement 7

**User Story:** Là một kỹ sư hệ thống, tôi muốn tất cả tọa độ được chuẩn hóa, để đảm bảo tính nhất quán trên các độ phân giải khác nhau

#### Acceptance Criteria

1. WHEN hệ thống nhận tọa độ đầu vào THEN hệ thống SHALL chuẩn hóa về độ phân giải chuẩn 1920x1080 bằng floating point
2. WHEN hệ thống gửi lệnh Click THEN hệ thống SHALL chuyển đổi tọa độ chuẩn hóa về độ phân giải thực tế của Worker
3. WHEN hệ thống lưu Action Point THEN hệ thống SHALL lưu tọa độ dưới dạng chuẩn hóa trong JSON Metadata
4. WHEN hệ thống trả về Bounding Box THEN hệ thống SHALL đảm bảo tọa độ đã được chuẩn hóa
5. WHEN hệ thống xử lý Search Region THEN hệ thống SHALL sử dụng tọa độ chuẩn hóa cho tất cả phép tính

### Requirement 8

**User Story:** Là một kỹ sư hệ thống, tôi muốn hệ thống sử dụng kiến trúc đa luồng hiệu quả, để xử lý nhiều Worker đồng thời mà không bị nghẽn

#### Acceptance Criteria

1. WHEN hệ thống khởi động THEN hệ thống SHALL tạo kiến trúc Manager-Worker với std::thread hoặc Boost.Asio
2. WHEN nhiều Worker truy cập tài nguyên chung THEN hệ thống SHALL sử dụng Mutex và Semaphore để đồng bộ hóa
3. WHEN Worker thực hiện inference THEN hệ thống SHALL sử dụng queue và mutex để đảm bảo truy cập tuần tự vào AI Engine
4. WHEN hệ thống gửi lệnh ADB THEN hệ thống SHALL đảm bảo không có race condition giữa các luồng
5. WHEN hệ thống quản lý socket THEN hệ thống SHALL sử dụng cơ chế đồng bộ để tránh xung đột

### Requirement 9

**User Story:** Là một người dùng, tôi muốn giao diện Manager UI hiển thị trạng thái của tất cả Worker theo thời gian thực, để giám sát hoạt động của hệ thống

#### Acceptance Criteria

1. WHEN người dùng mở Manager UI THEN hệ thống SHALL hiển thị giao diện 3 cột (Left/Center/Right)
2. WHEN hệ thống cập nhật trạng thái Worker THEN hệ thống SHALL hiển thị trong QTableView ở cột trung tâm với các thông tin FSM State, Current Node và FPS
3. WHEN Worker thay đổi trạng thái THEN hệ thống SHALL cập nhật UI trong thời gian thực không quá 500ms
4. WHEN người dùng chọn Worker THEN hệ thống SHALL hiển thị chi tiết Worker ở cột bên phải
5. WHEN hệ thống hiển thị FPS THEN hệ thống SHALL tính toán và cập nhật mỗi giây

### Requirement 10

**User Story:** Là một người dùng, tôi muốn Template Creator cho phép tôi tạo Template bằng cách vẽ và click trên ảnh, để định nghĩa vùng tìm kiếm và điểm thao tác một cách trực quan

#### Acceptance Criteria

1. WHEN người dùng mở Template Creator THEN hệ thống SHALL hiển thị ảnh chụp màn hình của Worker được chọn
2. WHEN người dùng vẽ hình chữ nhật THEN hệ thống SHALL lưu tọa độ Search Region dưới dạng chuẩn hóa
3. WHEN người dùng click để đặt Action Point THEN hệ thống SHALL lưu tọa độ dưới dạng chuẩn hóa
4. WHEN người dùng lưu Template THEN hệ thống SHALL tạo file JSON Metadata chứa tất cả thông tin
5. WHEN người dùng load Template THEN hệ thống SHALL hiển thị lại Search Region và Action Point trên ảnh

### Requirement 11

**User Story:** Là một người dùng, tôi muốn Flow Builder cho phép tôi tạo logic Bot bằng Node Editor, để xây dựng quy trình farming phức tạp mà không cần code

#### Acceptance Criteria

1. WHEN người dùng mở Flow Builder THEN hệ thống SHALL hiển thị Node Editor dựa trên Qt Graphics View Framework
2. WHEN người dùng kéo Node vào canvas THEN hệ thống SHALL tạo Node mới đại diện cho Action hoặc Decision
3. WHEN người dùng kết nối hai Node THEN hệ thống SHALL tạo edge thể hiện luồng điều khiển
4. WHEN người dùng lưu Flow THEN hệ thống SHALL serialize toàn bộ graph thành file JSON Graph
5. WHEN người dùng load Flow THEN hệ thống SHALL deserialize JSON Graph và hiển thị lại Node Editor

### Requirement 12

**User Story:** Là một quản trị viên, tôi muốn hệ thống báo cáo metrics quan trọng, để đánh giá hiệu suất và độ tin cậy

#### Acceptance Criteria

1. WHEN hệ thống vận hành THEN hệ thống SHALL thu thập Capture Failure Rate cho mỗi Worker
2. WHEN hệ thống ánh xạ HWND THEN hệ thống SHALL thu thập HWND Mapping Success Rate
3. WHEN hệ thống tạo báo cáo THEN hệ thống SHALL bao gồm Capture Failure Rate và HWND Mapping Success Rate
4. WHEN hệ thống ghi metrics THEN hệ thống SHALL lưu dữ liệu theo timestamp để phân tích xu hướng
5. WHEN người dùng yêu cầu báo cáo THEN hệ thống SHALL xuất báo cáo dưới dạng JSON hoặc CSV

### Requirement 13

**User Story:** Là một kỹ sư hệ thống, tôi muốn hệ thống hỗ trợ checkpoint và recovery, để phục hồi trạng thái sau khi khởi động lại

#### Acceptance Criteria

1. WHEN Worker hoàn thành một Node THEN hệ thống SHALL lưu checkpoint bao gồm FSM State và Current Node
2. WHEN hệ thống khởi động lại THEN hệ thống SHALL load checkpoint cuối cùng của mỗi Worker
3. WHEN checkpoint được load THEN hệ thống SHALL khôi phục Worker về đúng FSM State và Current Node
4. WHEN checkpoint bị lỗi THEN hệ thống SHALL khởi động Worker từ trạng thái mặc định và ghi log
5. WHEN hệ thống lưu checkpoint THEN hệ thống SHALL sử dụng atomic write để tránh corruption

### Requirement 14

**User Story:** Là một kỹ sư hệ thống, tôi muốn hệ thống xử lý lỗi một cách graceful, để tránh crash và mất dữ liệu

#### Acceptance Criteria

1. WHEN hệ thống gặp exception THEN hệ thống SHALL catch exception, ghi log chi tiết và tiếp tục vận hành
2. WHEN Worker gặp lỗi nghiêm trọng THEN hệ thống SHALL cô lập Worker đó và không ảnh hưởng đến Worker khác
3. WHEN hệ thống phát hiện memory leak THEN hệ thống SHALL ghi log cảnh báo và thực hiện cleanup
4. WHEN hệ thống mất kết nối ADB THEN hệ thống SHALL thử kết nối lại tối đa 3 lần trước khi đánh dấu Worker offline
5. WHEN hệ thống gặp lỗi I/O THEN hệ thống SHALL retry với exponential backoff

### Requirement 15

**User Story:** Là một người dùng, tôi muốn hệ thống hỗ trợ nhiều loại Node trong Flow Builder, để xây dựng logic phức tạp

#### Acceptance Criteria

1. WHEN người dùng tạo Action Node THEN hệ thống SHALL cho phép cấu hình loại action (Click, Swipe, Wait)
2. WHEN người dùng tạo Decision Node THEN hệ thống SHALL cho phép cấu hình điều kiện dựa trên kết quả nhận diện
3. WHEN người dùng tạo Loop Node THEN hệ thống SHALL cho phép cấu hình số lần lặp hoặc điều kiện dừng
4. WHEN người dùng tạo Template Match Node THEN hệ thống SHALL cho phép chọn Template và cấu hình confidence threshold
5. WHEN người dùng tạo AI Detection Node THEN hệ thống SHALL cho phép cấu hình class cần nhận diện

### Requirement 16

**User Story:** Là một kỹ sư hệ thống, tôi muốn hệ thống tối ưu hóa hiệu suất, để đạt FPS cao và độ trễ thấp

#### Acceptance Criteria

1. WHEN hệ thống chụp ảnh THEN hệ thống SHALL sử dụng buffer pool để tái sử dụng memory
2. WHEN hệ thống thực hiện inference THEN hệ thống SHALL batch nhiều request nếu có thể để tối ưu GPU
3. WHEN hệ thống xử lý ảnh THEN hệ thống SHALL sử dụng SIMD instructions nếu CPU hỗ trợ
4. WHEN hệ thống gửi lệnh ADB THEN hệ thống SHALL sử dụng connection pool để tái sử dụng socket
5. WHEN hệ thống đo latency THEN hệ thống SHALL đảm bảo độ trễ từ capture đến action không quá 200ms

### Requirement 17

**User Story:** Là một người dùng, tôi muốn hệ thống hỗ trợ import/export Flow và Template, để chia sẻ cấu hình với người khác

#### Acceptance Criteria

1. WHEN người dùng export Flow THEN hệ thống SHALL tạo file JSON Graph chứa toàn bộ định nghĩa Node và edge
2. WHEN người dùng import Flow THEN hệ thống SHALL validate JSON Graph và load vào Flow Builder
3. WHEN người dùng export Template THEN hệ thống SHALL tạo package chứa JSON Metadata và hình ảnh mẫu
4. WHEN người dùng import Template THEN hệ thống SHALL extract package và load vào Template Creator
5. WHEN file import bị lỗi THEN hệ thống SHALL hiển thị thông báo lỗi chi tiết và không crash

### Requirement 18

**User Story:** Là một kỹ sư hệ thống, tôi muốn hệ thống có khả năng mở rộng, để dễ dàng thêm Worker mới hoặc nâng cấp tính năng

#### Acceptance Criteria

1. WHEN hệ thống thêm Worker mới THEN hệ thống SHALL tự động phát hiện và ánh xạ HWND mà không cần khởi động lại
2. WHEN hệ thống nâng cấp mô hình AI THEN hệ thống SHALL cho phép hot-reload mô hình mà không dừng Worker
3. WHEN hệ thống thêm loại Node mới THEN hệ thống SHALL sử dụng plugin architecture để load Node type động
4. WHEN hệ thống cấu hình THEN hệ thống SHALL đọc từ file config JSON và áp dụng mà không cần biên dịch lại
5. WHEN hệ thống scale THEN hệ thống SHALL hỗ trợ tối thiểu 50 Worker đồng thời mà không giảm hiệu suất đáng kể
