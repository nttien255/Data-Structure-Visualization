#ifndef SYSTEM_UTILS_H
#define SYSTEM_UTILS_H

#include <string>

// Thêm tham số void* để truyền Window Handle từ Raylib sang Windows
std::string OpenWindowsFileDialog(void* windowHandle = nullptr);

#endif