#include "UI/InputBox.h"

InputBox::InputBox(float x, float y, float width, float height,
                   std::string placeholderText, int maxLen, bool onlyNumbers) {
    bounds = {x, y, width, height};
    placeholder = placeholderText;
    text = "";
    isActive = false;
    framesCounter = 0;
    maxLength = maxLen;
    numbersOnly = onlyNumbers;
}

void InputBox::SetBounds(float x, float y, float w, float h) {
    bounds = {x, y, w, h};
}

void InputBox::Update() {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        isActive = CheckCollisionPointRec(GetMousePosition(), bounds);
    }

    if (!isActive) {
        framesCounter = 0;
        return;
    }

    framesCounter++;

    // Handle paste (Ctrl+V)
    if ((IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)) 
         && IsKeyPressed(KEY_V)) {
        const char* clipboard = GetClipboardText();
        if (clipboard) {
            std::string paste(clipboard);
            for (char c : paste) {
                if (text.length() >= maxLength) break;
                if (numbersOnly) {
                    if (c >= '0' && c <= '9') text += c;
                    else if (c == '-' && text.empty()) text += c;
                } else {
                    if (c >= 32 && c <= 126) text += c;
                }
            }
        }
    }

    // Handle typing
    int key = GetCharPressed();
    while (key > 0) {
        if (text.length() < maxLength) {
            if (numbersOnly) {
                if ((key >= '0' && key <= '9') || 
                    (key == '-' && text.empty())) {
                    text += (char)key;
                }
            } else {
                if (key >= 32 && key <= 126) {
                    text += (char)key;
                }
            }
        }
        key = GetCharPressed();
    }

    // Backspace
    if (IsKeyPressed(KEY_BACKSPACE) || 
        (IsKeyDown(KEY_BACKSPACE) && framesCounter % 4 == 0)) {
        if (!text.empty()) text.pop_back();
    }

    // Clear all with Ctrl+Backspace
    if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_BACKSPACE)) {
        text.clear();
    }
}

void InputBox::Draw(Theme theme, Font font) {
    Color bgColor = isActive ? theme.panelBg : 
                    Fade(theme.panelBg, 0.7f);
    Color borderColor = isActive ? theme.nodeHighlight : theme.panelBorder;

    // Background
    DrawRectangleRounded(bounds, 0.15f, 8, bgColor);
    DrawRectangleRoundedLines(bounds, 0.15f, 8, borderColor);

    float fontSize = bounds.height * 0.5f;
    if (fontSize > 24) fontSize = 24;
    if (fontSize < 12) fontSize = 12;

    float textPadding = 12.0f;
    float maxTextWidth = bounds.width - textPadding * 2;

    // Calculate visible text (scroll if too long)
    std::string displayText = text;
    Vector2 tSize = MeasureTextEx(font, displayText.c_str(), fontSize, 1.0f);
    
    while (tSize.x > maxTextWidth && !displayText.empty()) {
        displayText.erase(0, 1);
        tSize = MeasureTextEx(font, displayText.c_str(), fontSize, 1.0f);
    }

    float textY = bounds.y + (bounds.height - fontSize) / 2;

    if (text.empty() && !isActive) {
        // Show placeholder
        DrawTextEx(font, placeholder.c_str(), 
                   {bounds.x + textPadding, textY}, 
                   fontSize, 1.0f, theme.textMuted);
    } else {
        // Show text
        DrawTextEx(font, displayText.c_str(),
                   {bounds.x + textPadding, textY},
                   fontSize, 1.0f, theme.textMain);
    }

    // Cursor
    if (isActive && (framesCounter / 25) % 2 == 0) {
        float cursorX = bounds.x + textPadding + tSize.x + 2;
        DrawLineEx(
            {cursorX, bounds.y + 8},
            {cursorX, bounds.y + bounds.height - 8},
            2.0f, theme.nodeHighlight
        );
    }
}

std::string InputBox::GetText() const { return text; }

void InputBox::SetText(const std::string& newText) { 
    text = newText.substr(0, maxLength); 
}

void InputBox::Clear() { text.clear(); }
