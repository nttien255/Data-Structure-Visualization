#ifndef CONTROL_PANEL_H
#define CONTROL_PANEL_H

#include "raylib.h"
#include "Core/Theme.h"
#include "Core/UILayout.h"
#include "UI/Button.h"
#include "UI/InputBox.h"
#include <string>

class ControlPanel {
private:
    UILayout layout;
    
    // Row 1: Operations
    InputBox valueInput;
    Button addBtn;
    Button deleteBtn;
    Button searchBtn;
    Button updateBtn; // NÚT MỚI: Cập nhật
    
    // Row 2: Init
    InputBox arrayInput;
    Button initArrayBtn;
    Button loadFileBtn;
    Button randomBtn;
    Button clearBtn;
    
    // Bottom Row: History & Playback
    Button undoBtn;   // NÚT MỚI
    Button redoBtn;   // NÚT MỚI
    Button prevBtn;
    Button modeBtn;
    Button nextBtn;
    
    float speedMultiplier;
    bool isStepByStep;
    void DrawSpeedSlider(Theme theme, Font monoFont, float x, float y);

public:
    ControlPanel();
    
    void UpdateLayout(UILayout& newLayout);
    void Update();
    void Draw(Theme theme, Font uiFont, Font monoFont);
    
    bool IsAddClicked();
    bool IsDeleteClicked();
    bool IsSearchClicked();
    bool IsUpdateClicked(); // NÚT MỚI
    bool IsRandomClicked();
    bool IsInitArrayClicked();
    bool IsLoadFileClicked();
    bool IsClearClicked();
    bool IsUndoClicked();   // NÚT MỚI
    bool IsRedoClicked();   // NÚT MỚI
    bool IsPrevClicked();
    bool IsNextClicked();
    bool IsModeClicked();
    
    std::string GetValueText() const;
    std::string GetArrayText() const;
    void ClearValueInput();
    void ClearArrayInput();
    
    float GetSpeed() const { return speedMultiplier; }
    bool IsStepMode() const { return isStepByStep; }
    void SetStepMode(bool step) { isStepByStep = step; }
};

#endif