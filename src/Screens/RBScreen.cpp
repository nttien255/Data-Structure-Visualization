#include "Screens/RBScreen.h"
#include <sstream>

RBScreen::RBScreen() 
    : homeBtn(20, 20, 130, 75, "Home", IconType::NONE),
      inputBox(170, 20, 240, 75, "Value:", 10, true),
      addBtn(430, 20, 150, 75, "Insert", IconType::NONE),
      delBtn(600, 20, 150, 75, "Delete", IconType::NONE),
      searchBtn(770, 20, 150, 75, "Search", IconType::NONE),
      
      arrayInput(170, 110, 240, 75, "e.g. 1 2 3", 60),
      initBtn(430, 110, 150, 75, "Init", IconType::NONE),
      randomBtn(600, 110, 150, 75, "Random", IconType::NONE),
      clearBtn(770, 110, 150, 75, "Clear", IconType::NONE),
      
      undoBtn(20, 0, 150, 75, "Undo", IconType::NONE),
      redoBtn(190, 0, 150, 75, "Redo", IconType::NONE),
      prevBtn(360, 0, 120, 75, "<<", IconType::NONE),
      modeBtn(500, 0, 240, 75, "Mode: Auto", IconType::NONE),
      nextBtn(760, 0, 120, 75, ">>", IconType::NONE)
{ 
    // XOÁ INITRANDOM - MÀN HÌNH RỖNG LÚC ĐẦU
}

void RBScreen::Update(AppState& currentState) {
    float sh = GetScreenHeight();
    float botY = sh - 95.0f;
    
    undoBtn.SetPosition(20, botY); redoBtn.SetPosition(190, botY);
    prevBtn.SetPosition(360, botY); modeBtn.SetPosition(500, botY); nextBtn.SetPosition(760, botY);

    inputBox.Update(); arrayInput.Update(); rbt.Update();
    if (homeBtn.Update()) currentState = AppState::MAIN_MENU;

    if (addBtn.Update() && !inputBox.GetText().empty()) { rbt.Insert(std::stoi(inputBox.GetText())); inputBox.Clear(); }
    if (randomBtn.Update()) rbt.Insert(GetRandomValue(10, 99));
    if (clearBtn.Update()) rbt.ClearTree();
    if (undoBtn.Update()) rbt.Undo();
    if (redoBtn.Update()) rbt.Redo();

    if (initBtn.Update()) {
        std::string arrStr = arrayInput.GetText();
        if (!arrStr.empty()) {
            std::stringstream ss(arrStr); std::vector<int> arr; std::string token;
            while (ss >> token) { try { arr.push_back(std::stoi(token)); } catch (...) {} }
            if (!arr.empty()) { rbt.InitFromArray(arr); arrayInput.Clear(); }
        }
    }

    if (modeBtn.Update()) { rbt.isStepByStep = !rbt.isStepByStep; modeBtn.SetText(rbt.isStepByStep ? "Mode: Step" : "Mode: Auto"); }
    if (rbt.isStepByStep) { if (prevBtn.Update()) rbt.StepBackward(); if (nextBtn.Update()) rbt.StepForward(); }

    // DỜI SLIDER SANG 1120px
    float sliderX = 1120.0f;
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), {sliderX, botY + 15, 200, 40})) {
        float pct = (GetMousePosition().x - sliderX) / 200.0f;
        rbt.speedMultiplier = 0.5f + (std::max(0.0f, std::min(1.0f, pct)) * 4.5f);
    }
}

void RBScreen::Draw(Theme theme, Font uiFont, Font monoFont) {
    float sh = GetScreenHeight();
    
    homeBtn.Draw(theme, uiFont); inputBox.Draw(theme, uiFont); addBtn.Draw(theme, uiFont);
    delBtn.Draw(theme, uiFont); searchBtn.Draw(theme, uiFont);
    arrayInput.Draw(theme, uiFont); initBtn.Draw(theme, uiFont); 
    randomBtn.Draw(theme, uiFont); clearBtn.Draw(theme, uiFont);
    undoBtn.Draw(theme, uiFont); redoBtn.Draw(theme, uiFont); modeBtn.Draw(theme, uiFont);
    
    if (rbt.isStepByStep) { prevBtn.Draw(theme, uiFont); nextBtn.Draw(theme, uiFont); }
    else {
        Theme disabledTheme = theme; disabledTheme.btnGradStart = theme.panelBorder; disabledTheme.btnGradEnd = theme.panelBorder;
        prevBtn.Draw(disabledTheme, uiFont); nextBtn.Draw(disabledTheme, uiFont);
    }

    // DỜI SLIDER SANG 1120px
    float sliderX = 1120.0f;
    float botY = sh - 95.0f;
    DrawTextEx(monoFont, "Speed:", {sliderX - 110, botY + 25}, 24, 1.0f, theme.textMuted);
    DrawRectangleRounded({sliderX, botY + 35, 200, 8}, 0.5f, 10, theme.panelBorder);
    DrawRectangleRounded({sliderX, botY + 35, ((rbt.speedMultiplier - 0.5f)/4.5f)*200, 8}, 0.5f, 10, theme.btnGradEnd);
    DrawCircle(sliderX + ((rbt.speedMultiplier - 0.5f)/4.5f)*200, botY + 39, 12, theme.textMain);

    rbt.Draw(theme, uiFont, monoFont);
}