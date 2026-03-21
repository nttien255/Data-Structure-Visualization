cmake_minimum_required(VERSION 3.14)
project(DataStructureVisualizer VERSION 1.0)

# Set C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. TUYỆT CHIÊU: Tự động tìm tất cả các file .cpp trong thư mục src và các thư mục con
file(GLOB_RECURSE ALL_SOURCES "src/*.cpp")

# Thêm tất cả các file source đó vào file thực thi
add_executable(${PROJECT_NAME} ${ALL_SOURCES})

# 2. Trỏ CMake tới thư mục 'include' của project
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)

# 3. Trỏ CMake tới thư mục raylib (Tải thủ công)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/raylib/include)
target_link_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/raylib/lib)

# 4. Link raylib VÀ các thư viện bắt buộc của Windows
target_link_libraries(${PROJECT_NAME} PRIVATE raylib winmm gdi32 opengl32)

# Tự động copy thư mục assets vào thư mục build khi chạy
file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})

# Tùy thuộc vào code hiện tại của bạn, hãy thêm comdlg32 vào danh sách link
target_link_libraries(${PROJECT_NAME} PRIVATE raylib comdlg32)