#ifndef BUTTON_H
#define BUTTON_H

#include "raylib.h"
#include "Core/Theme.h"
#include <string>

enum class IconType {
    NONE, SINGLY_LIST, HEAP, RBTREE, TRIE, MST, BFS,
    RANDOM, FILE, CLEAR, ADD, DELETE, SEARCH, HOME, EXIT
};

enum class ButtonStyle {
    PRIMARY,    // Default blue gradient
    SUCCESS,    // Green for add/confirm
    DANGER,     // Red for delete/clear
    SECONDARY   // Muted/gray
};

class Button {
private:
    Rectangle bounds;
    std::string text;
    IconType iconType;
    ButtonStyle style;
    bool isHovered;
    bool isEnabled;
    float hoverProgress;

    void DrawIcon(Theme theme, float x, float y, float size);

public:
    Button(float x, float y, float width, float height, 
           std::string label, IconType icon = IconType::NONE,
           ButtonStyle btnStyle = ButtonStyle::PRIMARY);

    void SetBounds(float x, float y, float w, float h);
    void SetText(std::string newText);
    void SetEnabled(bool enabled);
    bool Update();
    void Draw(Theme theme, Font font);
    Rectangle GetBounds() const { return bounds; }
};

#endif
