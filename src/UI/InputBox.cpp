#include "UI/InputBox.h"

InputBox::InputBox(float x, float y, float width, float height, std::string labelText, int maxLen) {
    bounds = {x, y, width, height};
    label = labelText;
    text = "";
    isActive = false;
    framesCounter = 0;
    maxLength = maxLen; // Gán độ dài tối đa
}

void InputBox::Update() {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        isActive = CheckCollisionPointRec(GetMousePosition(), bounds);
    }

    if (isActive) {
        framesCounter++;

        // ==========================================
        // HỖ TRỢ PASTE (CTRL + V)
        // ==========================================
        if ((IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)) && IsKeyPressed(KEY_V)) {
            const char* clipboard = GetClipboardText();
            if (clipboard != nullptr) {
                std::string pasteText(clipboard);
                // Biến đổi dấu xuống dòng thành dấu cách để hiển thị trên 1 dòng
                for(char& c : pasteText) { if(c == '\n' || c == '\r') c = ' '; }
                text += pasteText;
                if (text.length() > maxLength) text = text.substr(0, maxLength); // Cắt nếu quá dài
            }
        }

        // ==========================================
        // GÕ BÀN PHÍM BÌNH THƯỜNG (Cho phép phím Space)
        // ==========================================
        int key = GetCharPressed();
        while (key > 0) {
            if ((key >= 32) && (key <= 125) && (text.length() < maxLength)) {
                text += (char)key;
            }
            key = GetCharPressed();
        }

        if (IsKeyPressed(KEY_BACKSPACE)) {
            if (text.length() > 0) text.pop_back();
        }
    } else {
        framesCounter = 0; 
    }
}

void InputBox::Draw(Theme theme, Font font) {
    Color bgColor = isActive ? theme.panelBg : theme.background;
    Color borderColor = isActive ? theme.nodeHighlight : theme.panelBorder;

    DrawRectangleRounded(bounds, 0.15f, 10, bgColor);
    DrawRectangleRoundedLines(bounds, 0.15f, 10, borderColor);

    Vector2 labelSize = MeasureTextEx(font, label.c_str(), 28, 1.0f);
    Vector2 labelPos = {bounds.x - labelSize.x - 15, bounds.y + 15};
    
    DrawTextEx(font, label.c_str(), labelPos, 28, 1.0f, theme.textHighlighted);
    DrawTextEx(font, label.c_str(), {labelPos.x + 1.0f, labelPos.y}, 28, 1.0f, theme.textHighlighted);

    // Nếu chữ quá dài so với ô, ta sẽ cắt phần đầu để luôn nhìn thấy chữ đang gõ ở cuối
    std::string displayText = text;
    Vector2 tSize = MeasureTextEx(font, displayText.c_str(), 28, 1.0f);
    while (tSize.x > bounds.width - 40 && displayText.length() > 0) {
        displayText.erase(0, 1);
        tSize = MeasureTextEx(font, displayText.c_str(), 28, 1.0f);
    }

    DrawTextEx(font, displayText.c_str(), {bounds.x + 15, bounds.y + 15}, 28, 1.0f, theme.textMain);

    if (isActive && (framesCounter / 20) % 2 == 0) {
        DrawLineEx({bounds.x + 18 + tSize.x, bounds.y + 10}, 
                   {bounds.x + 18 + tSize.x, bounds.y + bounds.height - 10}, 
                   3.0f, theme.nodeHighlight);
    }
}

std::string InputBox::GetText() { return text; }
void InputBox::Clear() { text = ""; }