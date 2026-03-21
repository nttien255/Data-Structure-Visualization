#ifndef INPUTBOX_H
#define INPUTBOX_H

#include "raylib.h"
#include "Core/Theme.h"
#include <string>

class InputBox {
private:
    Rectangle bounds;
    std::string text;
    std::string label;
    bool isActive;       
    int framesCounter;   
    int maxLength; // <--- Thêm giới hạn ký tự tối đa

public:
    // Thêm tham số maxLen (mặc định là 10)
    InputBox(float x, float y, float width, float height, std::string labelText, int maxLen = 10);
    
    void Update();
    void Draw(Theme theme, Font font);
    
    std::string GetText();
    void Clear();
};

#endif