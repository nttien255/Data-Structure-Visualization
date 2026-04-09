#include "UI/Button.h"
#include <cmath>

void Button::SetPosition(float newX, float newY) {
    bounds.x = newX;
    bounds.y = newY;
}

static Color LerpColor(Color c1, Color c2, float t) {
    return {
        (unsigned char)(c1.r + (c2.r - c1.r) * t),
        (unsigned char)(c1.g + (c2.g - c1.g) * t),
        (unsigned char)(c1.b + (c2.b - c1.b) * t),
        (unsigned char)(c1.a + (c2.a - c1.a) * t)
    };
}

Button::Button(float x, float y, float width, float height, 
               std::string label, IconType icon, ButtonStyle btnStyle) {
    bounds = {x, y, width, height};
    text = label;
    iconType = icon;
    style = btnStyle;
    isHovered = false;
    isEnabled = true;
    hoverProgress = 0.0f;
}

void Button::SetBounds(float x, float y, float w, float h) {
    bounds = {x, y, w, h};
}

void Button::SetText(std::string newText) {
    text = newText;
}

void Button::SetEnabled(bool enabled) {
    isEnabled = enabled;
}

bool Button::Update() {
    if (!isEnabled) {
        hoverProgress = 0.0f;
        isHovered = false;
        return false;
    }

    isHovered = CheckCollisionPointRec(GetMousePosition(), bounds);

    float speed = 5.0f * GetFrameTime();
    if (isHovered) {
        hoverProgress += speed;
        if (hoverProgress > 1.0f) hoverProgress = 1.0f;
    } else {
        hoverProgress -= speed;
        if (hoverProgress < 0.0f) hoverProgress = 0.0f;
    }

    return isHovered && IsMouseButtonReleased(MOUSE_LEFT_BUTTON);
}

void Button::Draw(Theme theme, Font font) {
    Color gradStart, gradEnd, borderColor;

    if (!isEnabled) {
        gradStart = gradEnd = theme.panelBorder;
        borderColor = theme.panelBorder;
    } else {
        switch (style) {
            case ButtonStyle::SUCCESS:
                gradStart = theme.btnSuccess;
                gradEnd = LerpColor(theme.btnSuccess, WHITE, 0.2f);
                break;
            case ButtonStyle::DANGER:
                gradStart = theme.btnDanger;
                gradEnd = LerpColor(theme.btnDanger, WHITE, 0.2f);
                break;
            case ButtonStyle::SECONDARY:
                gradStart = theme.panelBorder;
                gradEnd = LerpColor(theme.panelBorder, WHITE, 0.1f);
                break;
            default:
                gradStart = theme.btnGradStart;
                gradEnd = theme.btnGradEnd;
        }
        borderColor = isHovered ? theme.btnHoverGlow : theme.panelBorder;
    }

    Rectangle renderBounds = bounds;
    if (isHovered && isEnabled) {
        renderBounds.y -= 2.0f;
    }

    // Shadow
    Rectangle shadowBounds = {
        renderBounds.x + 2, 
        renderBounds.y + 4, 
        renderBounds.width, 
        renderBounds.height
    };
    DrawRectangleRounded(shadowBounds, 0.2f, 8, GetColor(0x00000025));

    // Gradient background
    Color left = LerpColor(gradStart, GetColor(0x1D4ED8FF), hoverProgress * 0.3f);
    Color right = LerpColor(gradEnd, GetColor(0x3B82F6FF), hoverProgress * 0.3f);
    
    DrawRectangleGradientH(
        (int)renderBounds.x, (int)renderBounds.y,
        (int)renderBounds.width, (int)renderBounds.height,
        left, right
    );
    DrawRectangleRoundedLines(renderBounds, 0.2f, 8, borderColor);

    // Icon (Giữ nguyên kích thước to)
    float iconSize = bounds.height * 0.5f; 
    float iconX = renderBounds.x + 25; 
    float iconY = renderBounds.y + (renderBounds.height - iconSize) / 2;
    
    if (iconType != IconType::NONE) {
        DrawIcon(theme, iconX, iconY, iconSize);
    }

    // ==========================================
    // THUẬT TOÁN TEXT AUTO-SHRINK (CHỐNG TRÀN CHỮ)
    // ==========================================
    float fontSize = bounds.height * 0.40f; 
    if (fontSize > 45) fontSize = 45; 
    if (fontSize < 14) fontSize = 14;

    Vector2 textSize = MeasureTextEx(font, text.c_str(), fontSize, 1.0f);
    
    // 1. Tính toán khoảng trống tối đa cho phép chữ hiển thị
    float availableWidth;
    float startTextX;
    
    if (iconType != IconType::NONE) {
        startTextX = iconX + iconSize + 20; // Chữ bắt đầu sau icon
        availableWidth = bounds.width - (startTextX - bounds.x) - 20; // Trừ hao 20px lề phải
    } else {
        availableWidth = bounds.width - 40; // Trừ hao 20px lề trái, 20px lề phải
    }

    // 2. Vòng lặp bóp nhỏ chữ nếu bị tràn ngang
    while (textSize.x > availableWidth && fontSize > 12) {
        fontSize -= 1.0f; // Giảm dần từng size một
        textSize = MeasureTextEx(font, text.c_str(), fontSize, 1.0f);
    }

    // 3. Tính toán lại tọa độ vẽ sau khi đã có fontSize chuẩn
    float textX;
    if (iconType != IconType::NONE) {
        textX = startTextX;
    } else {
        // Nếu không có icon thì căn giữa
        textX = renderBounds.x + (renderBounds.width - textSize.x) / 2;
    }
    float textY = renderBounds.y + (renderBounds.height - textSize.y) / 2;

    Color textColor = isEnabled ? WHITE : theme.textMuted;
    DrawTextEx(font, text.c_str(), {textX, textY}, fontSize, 1.0f, textColor);
}

void Button::DrawIcon(Theme theme, float x, float y, float size) {
    Color col = WHITE;
    float cx = x + size / 2;
    float cy = y + size / 2;
    float r = size / 2;

    switch (iconType) {
        case IconType::SINGLY_LIST: {
            float boxW = size / 4;
            DrawRectangle(cx - r, cy - boxW/2, boxW, boxW, col);
            DrawRectangle(cx - boxW/2, cy - boxW/2, boxW, boxW, col);
            DrawRectangle(cx + r - boxW, cy - boxW/2, boxW, boxW, col);
            DrawLineEx({cx - r + boxW, cy}, {cx - boxW/2, cy}, 2, col);
            DrawLineEx({cx + boxW/2, cy}, {cx + r - boxW, cy}, 2, col);
            break;
        }
        case IconType::HEAP: {
            DrawCircle(cx, cy - r/2, r/4, col);
            DrawCircle(cx - r/2, cy + r/3, r/4, col);
            DrawCircle(cx + r/2, cy + r/3, r/4, col);
            DrawLineEx({cx, cy - r/2}, {cx - r/2, cy + r/3}, 2, col);
            DrawLineEx({cx, cy - r/2}, {cx + r/2, cy + r/3}, 2, col);
            break;
        }
        case IconType::RBTREE: {
            DrawCircle(cx, cy - r/2, r/4, LIGHTGRAY);
            DrawCircle(cx - r/2, cy + r/3, r/4, theme.nodeError);
            DrawCircle(cx + r/2, cy + r/3, r/4, LIGHTGRAY);
            DrawLineEx({cx, cy - r/2}, {cx - r/2, cy + r/3}, 2, col);
            DrawLineEx({cx, cy - r/2}, {cx + r/2, cy + r/3}, 2, col);
            break;
        }
        case IconType::TRIE: {
            float nodeR = r / 5;
            DrawCircle(cx, cy - r/2, nodeR, col);
            DrawCircle(cx - r/2, cy + r/3, nodeR, col);
            DrawCircle(cx, cy + r/3, nodeR, col);
            DrawCircle(cx + r/2, cy + r/3, nodeR, col);
            DrawLineEx({cx, cy - r/2}, {cx - r/2, cy + r/3}, 2, col);
            DrawLineEx({cx, cy - r/2}, {cx, cy + r/3}, 2, col);
            DrawLineEx({cx, cy - r/2}, {cx + r/2, cy + r/3}, 2, col);
            break;
        }
        case IconType::MST: {
            float nodeR = r / 5;
            DrawCircle(cx - r/3, cy - r/3, nodeR, col);
            DrawCircle(cx + r/3, cy - r/3, nodeR, col);
            DrawCircle(cx - r/3, cy + r/3, nodeR, col);
            DrawCircle(cx + r/3, cy + r/3, nodeR, col);
            DrawLineEx({cx - r/3, cy - r/3}, {cx + r/3, cy - r/3}, 3, theme.nodeSuccess);
            DrawLineEx({cx + r/3, cy - r/3}, {cx - r/3, cy + r/3}, 3, theme.nodeSuccess);
            break;
        }
        case IconType::BFS: {
            DrawCircleLines(cx, cy, r * 0.8f, col);
            DrawCircleLines(cx, cy, r * 0.5f, col);
            DrawCircle(cx, cy, r * 0.15f, col);
            break;
        }
        case IconType::RANDOM: {
            // Dice icon
            DrawRectangleRounded({x, y, size, size}, 0.2f, 4, GetColor(0xFFFFFF30));
            DrawRectangleRoundedLines({x, y, size, size}, 0.2f, 4, col);
            float dotR = size / 10;
            DrawCircle(cx - size/4, cy - size/4, dotR, col);
            DrawCircle(cx + size/4, cy + size/4, dotR, col);
            DrawCircle(cx, cy, dotR, col);
            break;
        }
        case IconType::FILE: {
            DrawRectangle(x + 2, y, size - 8, size, col);
            DrawTriangle(
                {x + size - 6, y},
                {x + size - 6, y + 8},
                {x + size - 14, y},
                col
            );
            break;
        }
        case IconType::CLEAR: {
            DrawLineEx({x + 3, y + 3}, {x + size - 3, y + size - 3}, 3, col);
            DrawLineEx({x + size - 3, y + 3}, {x + 3, y + size - 3}, 3, col);
            break;
        }
        case IconType::ADD: {
            DrawLineEx({cx, y + 3}, {cx, y + size - 3}, 3, col);
            DrawLineEx({x + 3, cy}, {x + size - 3, cy}, 3, col);
            break;
        }
        case IconType::DELETE: {
            DrawLineEx({x + 4, y + 4}, {x + size - 4, y + size - 4}, 3, col);
            DrawLineEx({x + size - 4, y + 4}, {x + 4, y + size - 4}, 3, col);
            break;
        }
        case IconType::SEARCH: {
            DrawCircleLines(cx - 2, cy - 2, r * 0.5f, col);
            DrawLineEx({cx + r*0.2f, cy + r*0.2f}, {cx + r*0.7f, cy + r*0.7f}, 3, col);
            break;
        }
        case IconType::HOME: {
            // Simple house
            DrawTriangle({cx, y}, {x, cy}, {x + size, cy}, col);
            DrawRectangle(x + size/4, cy, size/2, size/2, col);
            break;
        }
        case IconType::EXIT: {
            DrawRectangleLines(x + 2, y + 2, size - 4, size - 4, col);
            DrawLineEx({cx, cy - r/3}, {cx + r/2, cy}, 2, col);
            DrawLineEx({cx + r/2, cy}, {cx, cy + r/3}, 2, col);
            DrawLineEx({x + 4, cy}, {cx + r/2, cy}, 2, col);
            break;
        }
        default:
            break;
    }
}
