#include "UI/ControlPanel.h"

ControlPanel::ControlPanel() 
    : valueInput(0,0,0,0, "Value", 10, true),
      addBtn(0,0,0,0, "Add", IconType::ADD, ButtonStyle::SUCCESS),
      deleteBtn(0,0,0,0, "Del", IconType::DELETE, ButtonStyle::DANGER),
      searchBtn(0,0,0,0, "Find", IconType::SEARCH),
      updateBtn(0,0,0,0, "Update", IconType::NONE, ButtonStyle::PRIMARY), // Update
      
      arrayInput(0,0,0,0, "e.g. 1 2 3", 50, false),
      initArrayBtn(0,0,0,0, "Init", IconType::NONE),
      loadFileBtn(0,0,0,0, "File", IconType::FILE),
      randomBtn(0,0,0,0, "Random", IconType::RANDOM, ButtonStyle::SUCCESS), // Đổi màu xanh
      clearBtn(0,0,0,0, "Clear", IconType::CLEAR, ButtonStyle::DANGER),
      
      undoBtn(0,0,0,0, "Undo", IconType::NONE, ButtonStyle::SECONDARY),
      redoBtn(0,0,0,0, "Redo", IconType::NONE, ButtonStyle::SECONDARY),
      prevBtn(0,0,0,0, "<<", IconType::NONE, ButtonStyle::SECONDARY),
      modeBtn(0,0,0,0, "Auto", IconType::NONE),
      nextBtn(0,0,0,0, ">>", IconType::NONE, ButtonStyle::SECONDARY)
{
    speedMultiplier = 1.0f;
    isStepByStep = false;
}

void ControlPanel::UpdateLayout(UILayout& newLayout) {
    layout = newLayout;
    
    float topRow1Y = 20.0f;
    float topRow2Y = 75.0f;
    float botY = layout.screenH - 65.0f;

    // --- HÀNG NGANG 1 (TOP LÊN TRÊN CÙNG) ---
    // SỬA Ở ĐÂY: Đổi từ 120.0f thành 180.0f để né nút Home
    float currX = 180.0f; 
    valueInput.SetBounds(currX, topRow1Y, 160, 45); currX += 175;
    addBtn.SetBounds(currX, topRow1Y, 110, 45); currX += 125;
    deleteBtn.SetBounds(currX, topRow1Y, 110, 45); currX += 125;
    searchBtn.SetBounds(currX, topRow1Y, 110, 45); currX += 125;
    updateBtn.SetBounds(currX, topRow1Y, 110, 45);

    // --- HÀNG NGANG 2 ---
    // SỬA Ở ĐÂY: Đổi từ 120.0f thành 180.0f để thẳng hàng với Hàng 1
    currX = 180.0f;
    arrayInput.SetBounds(currX, topRow2Y, 280, 45); currX += 295;
    initArrayBtn.SetBounds(currX, topRow2Y, 100, 45); currX += 115;
    loadFileBtn.SetBounds(currX, topRow2Y, 100, 45); currX += 115;
    randomBtn.SetBounds(currX, topRow2Y, 120, 45); currX += 135; 
    clearBtn.SetBounds(currX, topRow2Y, 100, 45);

    // --- HÀNG NGANG ĐÁY (BOTTOM BAR) ---
    currX = 20.0f;
    undoBtn.SetBounds(currX, botY, 100, 45); currX += 115;
    redoBtn.SetBounds(currX, botY, 100, 45); currX += 130; 

    prevBtn.SetBounds(currX, botY, 70, 45); currX += 85;
    modeBtn.SetBounds(currX, botY, 140, 45); currX += 155;
    nextBtn.SetBounds(currX, botY, 70, 45);
}

void ControlPanel::Update() {
    valueInput.Update(); arrayInput.Update();
    addBtn.Update(); deleteBtn.Update(); searchBtn.Update(); updateBtn.Update();
    initArrayBtn.Update(); loadFileBtn.Update(); randomBtn.Update(); clearBtn.Update();
    
    undoBtn.Update(); redoBtn.Update();
    modeBtn.Update();
    if (isStepByStep) { prevBtn.Update(); nextBtn.Update(); }

    if (modeBtn.Update()) {
        isStepByStep = !isStepByStep;
        modeBtn.SetText(isStepByStep ? "Step" : "Auto");
    }

    // SLIDER LOGIC
    float sliderX = 850.0f;
    float botY = layout.screenH - 65.0f;
    Rectangle sliderHitbox = {sliderX - 10, botY, 220, 45};
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        Vector2 mousePos = GetMousePosition();
        if (CheckCollisionPointRec(mousePos, sliderHitbox)) {
            float pct = (mousePos.x - sliderX) / 200.0f;
            if (pct < 0.0f) pct = 0.0f;
            if (pct > 1.0f) pct = 1.0f;
            speedMultiplier = 0.5f + (pct * 4.5f);
        }
    }
}

void ControlPanel::Draw(Theme theme, Font uiFont, Font monoFont) {
    // Vẽ nền cho Top Bar và Bottom Bar
    DrawRectangle(0, 0, layout.screenW, layout.topBarH, Fade(theme.panelBg, 0.9f));
    DrawLine(0, layout.topBarH, layout.screenW, layout.topBarH, theme.panelBorder);
    
    float botY = layout.screenH - layout.bottomBarH;
    DrawRectangle(0, botY, layout.screenW, layout.bottomBarH, Fade(theme.panelBg, 0.9f));
    DrawLine(0, botY, layout.screenW, botY, theme.panelBorder);

    valueInput.Draw(theme, uiFont); arrayInput.Draw(theme, uiFont);
    addBtn.Draw(theme, uiFont); deleteBtn.Draw(theme, uiFont); 
    searchBtn.Draw(theme, uiFont); updateBtn.Draw(theme, uiFont);
    initArrayBtn.Draw(theme, uiFont); loadFileBtn.Draw(theme, uiFont); 
    randomBtn.Draw(theme, uiFont); clearBtn.Draw(theme, uiFont);
    
    undoBtn.Draw(theme, uiFont); redoBtn.Draw(theme, uiFont);
    modeBtn.Draw(theme, uiFont);
    
    prevBtn.SetEnabled(isStepByStep);
    nextBtn.SetEnabled(isStepByStep);
    prevBtn.Draw(theme, uiFont); nextBtn.Draw(theme, uiFont);

    // Vẽ Slider
    DrawSpeedSlider(theme, monoFont, 850.0f, layout.screenH - 45.0f);
}

void ControlPanel::DrawSpeedSlider(Theme theme, Font monoFont, float x, float y) {
    DrawTextEx(monoFont, "Speed:", {x - 80, y - 10}, 20, 1.0f, theme.textMuted);
    DrawTextEx(monoFont, "0.5x", {x - 40, y - 10}, 16, 1.0f, theme.textMuted);
    DrawTextEx(monoFont, "5.0x", {x + 215, y - 10}, 16, 1.0f, theme.textMuted);
    
    DrawRectangleRounded({x, y, 200, 6}, 1.0f, 10, theme.panelBorder);
    float fillWidth = ((speedMultiplier - 0.5f) / 4.5f) * 200.0f;
    DrawRectangleRounded({x, y, fillWidth, 6}, 1.0f, 10, theme.btnGradEnd);
    DrawCircle(x + fillWidth, y + 3, 10, theme.textMain);
}

bool ControlPanel::IsAddClicked() { return addBtn.Update(); }
bool ControlPanel::IsDeleteClicked() { return deleteBtn.Update(); }
bool ControlPanel::IsSearchClicked() { return searchBtn.Update(); }
bool ControlPanel::IsUpdateClicked() { return updateBtn.Update(); }
bool ControlPanel::IsRandomClicked() { return randomBtn.Update(); }
bool ControlPanel::IsInitArrayClicked() { return initArrayBtn.Update(); }
bool ControlPanel::IsLoadFileClicked() { return loadFileBtn.Update(); }
bool ControlPanel::IsClearClicked() { return clearBtn.Update(); }
bool ControlPanel::IsUndoClicked() { return undoBtn.Update(); }
bool ControlPanel::IsRedoClicked() { return redoBtn.Update(); }
bool ControlPanel::IsPrevClicked() { return prevBtn.Update(); }
bool ControlPanel::IsNextClicked() { return nextBtn.Update(); }
bool ControlPanel::IsModeClicked() { return modeBtn.Update(); }
std::string ControlPanel::GetValueText() const { return valueInput.GetText(); }
std::string ControlPanel::GetArrayText() const { return arrayInput.GetText(); }
void ControlPanel::ClearValueInput() { valueInput.Clear(); }
void ControlPanel::ClearArrayInput() { arrayInput.Clear(); }