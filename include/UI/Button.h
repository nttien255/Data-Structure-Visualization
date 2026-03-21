#ifndef BUTTON_H
#define BUTTON_H

#include "raylib.h"
#include "Core/Theme.h"
#include <string>

enum class IconType { 
    NONE, SINGLY_LIST, HEAP, RBTREE, TRIE, MST, BFS 
};

class Button {
private:
    Rectangle bounds;
    std::string text;
    IconType iconType;
    bool isHovered;
    float hoverProgress; 

    void DrawIcon(Theme theme);

public:
    // Constructor 6 thông số
    Button(float x, float y, float width, float height, std::string label, IconType icon);
    
    bool Update();
    void Draw(Theme theme, Font font);
    void SetText(std::string newText); // Hàm đổi chữ
};

#endif