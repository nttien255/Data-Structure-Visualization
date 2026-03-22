#ifndef INPUTBOX_H
#define INPUTBOX_H

#include "raylib.h"
#include "Core/Theme.h"
#include <string>

class InputBox {
private:
    Rectangle bounds;
    std::string text;
    std::string placeholder;
    bool isActive;
    int framesCounter;
    int maxLength;
    bool numbersOnly;

public:
    InputBox(float x, float y, float width, float height, 
             std::string placeholderText = "", int maxLen = 20, 
             bool onlyNumbers = false);

    void SetBounds(float x, float y, float w, float h);
    void Update();
    void Draw(Theme theme, Font font);

    std::string GetText() const;
    void SetText(const std::string& newText);
    void Clear();
    bool IsActive() const { return isActive; }
};

#endif
