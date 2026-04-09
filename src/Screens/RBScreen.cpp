#include "Screens/RBScreen.h"
#include <sstream>

RBScreen::RBScreen() 
    : homeBtn(30, 30, 130, 75, "Home", IconType::HOME),
      inputBox(190, 30, 260, 75, "Value:", 10, true),
      addBtn(470, 30, 140, 75, "Insert", IconType::NONE),
      delBtn(630, 30, 140, 75, "Delete", IconType::NONE),
      searchBtn(790, 30, 140, 75, "Search", IconType::NONE),
      arrayInput(190, 120, 260, 75, "e.g. 1 2 3", 60),
      initBtn(470, 120, 140, 75, "Init", IconType::NONE),
      randomBtn(630, 120, 140, 75, "Random", IconType::NONE),
      clearBtn(790, 120, 140, 75, "Clear", IconType::NONE),
      undoBtn(30, 0, 130, 75, "Undo", IconType::NONE),
      redoBtn(180, 0, 130, 75, "Redo", IconType::NONE),
      prevBtn(340, 0, 100, 75, "<<", IconType::NONE),
      modeBtn(460, 0, 260, 75, "Mode: Auto", IconType::NONE),
      nextBtn(740, 0, 100, 75, ">>", IconType::NONE)
{ rbt.InitRandom(7); }

void RBScreen::Update(AppState& currentState) {
    float sh = GetScreenHeight();
    float botY = sh - 100;
    undoBtn.SetPosition(30, botY); redoBtn.SetPosition(180, botY);
    prevBtn.SetPosition(340, botY); modeBtn.SetPosition(460, botY); nextBtn.SetPosition(740, botY);

    inputBox.Update(); arrayInput.Update(); rbt.Update();
    if (homeBtn.Update()) currentState = AppState::MAIN_MENU;

    if (addBtn.Update() && !inputBox.GetText().empty()) { rbt.Insert(std::stoi(inputBox.GetText())); inputBox.Clear(); }
    if (randomBtn.Update()) rbt.Insert(GetRandomValue(10, 99));
    if (clearBtn.Update()) rbt.ClearTree();
    if (undoBtn.Update()) rbt.Undo();
    if (redoBtn.Update()) rbt.Redo();

    // SỰ KIỆN MỚI: Xử lý chức năng Init cây từ danh sách số
    if (initBtn.Update()) {
        std::string arrStr = arrayInput.GetText();
        if (!arrStr.empty()) {
            std::stringstream ss(arrStr);
            std::string token;
            rbt.ClearTree();
            while (ss >> token) {
                try { rbt.Insert(std::stoi(token)); } catch (...) {}
            }
            rbt.ForceStaticView(); // Ép vẽ ngay cây đã hoàn thành
            arrayInput.Clear();
        }
    }

    if (modeBtn.Update()) { rbt.isStepByStep = !rbt.isStepByStep; modeBtn.SetText(rbt.isStepByStep ? "Mode: Step" : "Mode: Auto"); }
    if (rbt.isStepByStep) { if (prevBtn.Update()) rbt.StepBackward(); if (nextBtn.Update()) rbt.StepForward(); }

    float sliderX = 980.0f;
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), {sliderX, sh - 70, 200, 50})) {
        float pct = (GetMousePosition().x - sliderX) / 200.0f;
        rbt.speedMultiplier = 0.5f + (std::max(0.0f, std::min(1.0f, pct)) * 4.5f);
    }
}

void RBScreen::Draw(Theme theme, Font uiFont, Font monoFont) {
    float sh = GetScreenHeight();
    homeBtn.Draw(theme, uiFont); inputBox.Draw(theme, uiFont); addBtn.Draw(theme, uiFont);
    delBtn.Draw(theme, uiFont); searchBtn.Draw(theme, uiFont);
    arrayInput.Draw(theme, uiFont); initBtn.Draw(theme, uiFont); randomBtn.Draw(theme, uiFont); clearBtn.Draw(theme, uiFont);
    undoBtn.Draw(theme, uiFont); redoBtn.Draw(theme, uiFont); modeBtn.Draw(theme, uiFont);
    if (rbt.isStepByStep) { prevBtn.Draw(theme, uiFont); nextBtn.Draw(theme, uiFont); }

    float sliderX = 980.0f;
    DrawTextEx(monoFont, "Speed:", {sliderX - 130, sh - 60}, 24, 1.0f, theme.textMuted);
    DrawRectangleRounded({sliderX, sh - 50, 200, 10}, 1.0f, 10, theme.panelBorder);
    DrawRectangleRounded({sliderX, sh - 50, ((rbt.speedMultiplier - 0.5f)/4.5f)*200, 10}, 1.0f, 10, theme.btnGradEnd);
    DrawCircle(sliderX + ((rbt.speedMultiplier - 0.5f)/4.5f)*200, sh - 45, 12, theme.textMain);

    rbt.Draw(theme, uiFont, monoFont);
}