#include "UI/Button.h"
#include <math.h>

static Color LerpColor(Color c1, Color c2, float t) {
    return {
        (unsigned char)(c1.r + (c2.r - c1.r) * t),
        (unsigned char)(c1.g + (c2.g - c1.g) * t),
        (unsigned char)(c1.b + (c2.b - c1.b) * t),
        (unsigned char)(c1.a + (c2.a - c1.a) * t)
    };
}

Button::Button(float x, float y, float width, float height, std::string label, IconType icon) {
    bounds = {x, y, width, height};
    text = label;
    iconType = icon;
    isHovered = false;
    hoverProgress = 0.0f; 
}

void Button::SetText(std::string newText) {
    text = newText;
}

bool Button::Update() {
    isHovered = CheckCollisionPointRec(GetMousePosition(), bounds);
    
    if (isHovered) {
        hoverProgress += GetFrameTime() * 1.0f; 
        if (hoverProgress > 1.0f) hoverProgress = 1.0f;
    } else {
        hoverProgress -= GetFrameTime() * 1.0f;
        if (hoverProgress < 0.0f) hoverProgress = 0.0f;
    }

    if (isHovered && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) return true; 
    return false;
}

void Button::Draw(Theme theme, Font font) {
    Color borderColor = isHovered ? theme.btnHoverGlow : theme.panelBorder;
    
    Rectangle renderBounds = bounds;
    if (isHovered) renderBounds.y -= 2.0f; 

    Rectangle shadowBounds = {renderBounds.x + 2, renderBounds.y + 6, renderBounds.width, renderBounds.height};
    DrawRectangleRounded(shadowBounds, 0.15f, 10, GetColor(0x00000030)); 

    Color slideColor = GetColor(0x1D4ED8FF); 
    float waveLeft = fmax(0.0f, 1.0f - fabsf(0.0f - hoverProgress) / 0.6f);
    float waveRight = fmax(0.0f, 1.0f - fabsf(1.0f - hoverProgress) / 0.6f);

    Color currentLeft = LerpColor(theme.btnGradStart, slideColor, waveLeft);
    Color currentRight = LerpColor(theme.btnGradEnd, slideColor, waveRight);

    DrawRectangleRounded(renderBounds, 0.15f, 10, GetColor(0x00000000)); 
    DrawRectangleGradientH(renderBounds.x, renderBounds.y, renderBounds.width, renderBounds.height, currentLeft, currentRight); 
    DrawRectangleRoundedLines(renderBounds, 0.15f, 10, GetColor(0x00000000)); 
    DrawRectangleRoundedLines(renderBounds, 0.15f, 10, borderColor);

    DrawIcon(theme);

    float textX;
    Vector2 textSize = MeasureTextEx(font, text.c_str(), 32, 1.0f);
    if (iconType != IconType::NONE) {
        textX = renderBounds.x + 120 + ((renderBounds.width - 120) / 2) - (textSize.x / 2);
    } else {
        textX = renderBounds.x + (renderBounds.width / 2) - (textSize.x / 2);
    }
    float textY = renderBounds.y + (renderBounds.height / 2) - (textSize.y / 2);
    
    Color currentTextColor = WHITE; 
    
    DrawTextEx(font, text.c_str(), {textX, textY}, 32, 1.0f, currentTextColor);
    DrawTextEx(font, text.c_str(), {textX + 1.0f, textY}, 32, 1.0f, currentTextColor);
}

void Button::DrawIcon(Theme theme) {
    if (iconType == IconType::NONE) return; 

    float activeY = isHovered ? bounds.y - 2.0f : bounds.y;
    float cx = bounds.x + 60; 
    float cy = activeY + bounds.height / 2; 
    
    Color col = WHITE; 
    Color edgeCol = WHITE;

    switch (iconType) {
        case IconType::SINGLY_LIST:
            DrawRectangle(cx - 30, cy - 10, 15, 20, col);
            DrawRectangle(cx -  5, cy - 10, 15, 20, col);
            DrawRectangle(cx + 20, cy - 10, 15, 20, col);
            DrawLineEx({cx - 15, cy}, {cx - 5, cy}, 2.0f, edgeCol);
            DrawLineEx({cx + 10, cy}, {cx + 20, cy}, 2.0f, edgeCol);
            break;
        case IconType::HEAP:
            DrawCircle(cx, cy - 15, 8, col);
            DrawCircle(cx - 15, cy + 10, 8, col);
            DrawCircle(cx + 15, cy + 10, 8, col);
            DrawLineEx({cx, cy - 15}, {cx - 15, cy + 10}, 2.0f, edgeCol);
            DrawLineEx({cx, cy - 15}, {cx + 15, cy + 10}, 2.0f, edgeCol);
            break;
        case IconType::RBTREE:
            DrawCircle(cx, cy - 15, 8, LIGHTGRAY); 
            DrawCircle(cx - 15, cy + 10, 8, theme.nodeError);  
            DrawCircle(cx + 15, cy + 10, 8, LIGHTGRAY);
            DrawLineEx({cx, cy - 15}, {cx - 15, cy + 10}, 2.0f, edgeCol);
            DrawLineEx({cx, cy - 15}, {cx + 15, cy + 10}, 2.0f, edgeCol);
            break;
        case IconType::TRIE:
            DrawCircle(cx, cy - 20, 6, col);
            DrawCircle(cx - 20, cy + 10, 6, col);
            DrawCircle(cx, cy + 10, 6, col);
            DrawCircle(cx + 20, cy + 10, 6, col);
            DrawLineEx({cx, cy-20}, {cx-20, cy+10}, 2.0f, edgeCol);
            DrawLineEx({cx, cy-20}, {cx, cy+10}, 2.0f, edgeCol);
            DrawLineEx({cx, cy-20}, {cx+20, cy+10}, 2.0f, edgeCol);
            break;
        case IconType::MST:
            DrawCircle(cx - 15, cy - 15, 6, col);
            DrawCircle(cx + 15, cy - 15, 6, col);
            DrawCircle(cx - 15, cy + 15, 6, col);
            DrawCircle(cx + 15, cy + 15, 6, col);
            DrawLineEx({cx - 15, cy - 15}, {cx + 15, cy - 15}, 3.0f, theme.nodeSuccess); 
            DrawLineEx({cx + 15, cy - 15}, {cx - 15, cy + 15}, 3.0f, theme.nodeSuccess); 
            break;
        case IconType::BFS:
            DrawCircleLines(cx, cy, 10, WHITE);
            DrawCircleLines(cx, cy, 20, WHITE);
            DrawCircle(cx, cy, 4, WHITE); 
            DrawCircle(cx - 20, cy, 4, col); 
            DrawCircle(cx + 20, cy, 4, col); 
            break;
        default: break;
    }
}