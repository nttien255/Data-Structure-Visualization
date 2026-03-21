cmake_minimum_required(VERSION 3.14)
project(DataStructureVisualizer VERSION 1.0)

# Set C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Tự động tìm tất cả các file .cpp trong thư mục src và các thư mục con
file(GLOB_RECURSE ALL_SOURCES "src/*.cpp")

# 2. Khai báo file thực thi
add_executable(${PROJECT_NAME} ${ALL_SOURCES})

# 3. Trỏ CMake tới thư mục 'include' của riêng project
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)

# =========================================================================
# 4. TÍCH HỢP RAYLIB TỪ GIT SUBMODULE
# (Yêu cầu phải chạy lệnh: git submodule add https://github.com/raysan5/raylib.git raylib)
# =========================================================================
add_subdirectory(raylib)

# 5. Link thư viện (raylib từ submodule và comdlg32 cho Windows File Dialog)
target_link_libraries(${PROJECT_NAME} PRIVATE raylib comdlg32)

# 6. Tự động copy thư mục assets vào thư mục build khi chạy
file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})