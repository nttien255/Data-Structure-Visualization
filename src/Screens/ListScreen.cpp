#include "Screens/ListScreen.h"
#include "UI/SystemUtils.h"
#include <sstream>

ListScreen::ListScreen() 
    : homeBtn(20, 20, 130, 75, "Home", IconType::NONE),
      valueInput(170, 20, 240, 75, "Value:", 10),
      addBtn(430, 20, 150, 75, "Add", IconType::NONE),
      delBtn(600, 20, 150, 75, "Delete", IconType::NONE),
      searchBtn(770, 20, 150, 75, "Search", IconType::NONE),
      updateBtn(940, 20, 150, 75, "Update", IconType::NONE),
      arrayInput(170, 110, 240, 75, "e.g. 1 2 3", 60),
      initBtn(430, 110, 150, 75, "Init", IconType::NONE),
      fileBtn(600, 110, 150, 75, "File", IconType::NONE),
      randomBtn(770, 110, 150, 75, "Random", IconType::NONE),
      clearBtn(940, 110, 150, 75, "Clear", IconType::NONE),
      undoBtn(20, 0, 150, 75, "Undo", IconType::NONE),
      redoBtn(190, 0, 150, 75, "Redo", IconType::NONE),
      prevBtn(360, 0, 120, 75, "<<", IconType::NONE),
      modeBtn(500, 0, 240, 75, "Mode: Auto", IconType::NONE),
      nextBtn(760, 0, 120, 75, ">>", IconType::NONE)
{ 
    // XOÁ INITRANDOM - MÀN HÌNH RỖNG LÚC ĐẦU
}

void ListScreen::Update(AppState& currentState) {
    layout.Update(); float sh = GetScreenHeight(); float botY = sh - 95.0f;
    undoBtn.SetPosition(20, botY); redoBtn.SetPosition(190, botY);
    prevBtn.SetPosition(360, botY); modeBtn.SetPosition(500, botY); nextBtn.SetPosition(760, botY);

    valueInput.Update(); arrayInput.Update();
    if (homeBtn.Update()) { currentState = AppState::MAIN_MENU; return; }

    HandleActions(); list.Update();
}

void ListScreen::HandleActions() {
    std::string valStr = valueInput.GetText();
    
    if (addBtn.Update() && !valStr.empty()) { try { list.AddTail(std::stoi(valStr)); valueInput.Clear(); } catch (...) {} }
    if (delBtn.Update() && !valStr.empty()) { try { list.Delete(std::stoi(valStr)); valueInput.Clear(); } catch (...) {} }
    if (searchBtn.Update() && !valStr.empty()) { try { list.Search(std::stoi(valStr)); valueInput.Clear(); } catch (...) {} }
    if (updateBtn.Update() && !valStr.empty()) { try { if (list.selectedIndex != -1) { list.UpdateNode(std::stoi(valStr)); valueInput.Clear(); } } catch (...) {} }
    
    if (randomBtn.Update()) list.AddTail(GetRandomValue(10, 99));
    if (modeBtn.Update()) { list.isStepByStep = !list.isStepByStep; modeBtn.SetText(list.isStepByStep ? "Mode: Step" : "Mode: Auto"); }
    if (list.isStepByStep) { if (prevBtn.Update() || IsKeyPressed(KEY_LEFT)) list.StepBackward(); if (nextBtn.Update() || IsKeyPressed(KEY_RIGHT)) list.StepForward(); }
    if (undoBtn.Update()) list.Undo();
    if (redoBtn.Update()) list.Redo();

    if (initBtn.Update()) {
        std::string arrStr = arrayInput.GetText();
        if (!arrStr.empty()) {
            std::stringstream ss(arrStr); std::vector<int> arr; std::string token;
            while (ss >> token) { try { arr.push_back(std::stoi(token)); } catch (...) {} }
            if (!arr.empty()) { list.InitFromArray(arr); arrayInput.Clear(); }
        }
    }
    
    if (fileBtn.Update()) {
        bool isFull = IsWindowFullscreen(); if (isFull) ToggleFullscreen(); 
        std::string path = OpenWindowsFileDialog(GetWindowHandle()); if (isFull) ToggleFullscreen(); 
        if (!path.empty()) {
            char* content = LoadFileText(path.c_str());
            if (content) {
                std::stringstream ss(content); std::vector<int> arr; std::string token;
                while (ss >> token) { try { arr.push_back(std::stoi(token)); } catch (...) {} }
                UnloadFileText(content); if (!arr.empty()) list.InitFromArray(arr);
            }
        }
    }
    
    if (clearBtn.Update()) list.ClearList();

    // DỜI SLIDER SANG 1120px
    float sliderX = 1120.0f; float botY = GetScreenHeight() - 95.0f;
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), {sliderX, botY + 15, 200.0f, 40.0f})) {
        float pct = (GetMousePosition().x - sliderX) / 200.0f;
        list.speedMultiplier = 0.5f + (std::max(0.0f, std::min(1.0f, pct)) * 4.5f); 
    }
}

void ListScreen::Draw(Theme theme, Font uiFont, Font monoFont) {
    float sh = GetScreenHeight();
    homeBtn.Draw(theme, uiFont); valueInput.Draw(theme, uiFont); addBtn.Draw(theme, uiFont);
    delBtn.Draw(theme, uiFont); searchBtn.Draw(theme, uiFont); updateBtn.Draw(theme, uiFont);
    arrayInput.Draw(theme, uiFont); initBtn.Draw(theme, uiFont);
    fileBtn.Draw(theme, uiFont); randomBtn.Draw(theme, uiFont); clearBtn.Draw(theme, uiFont);
    undoBtn.Draw(theme, uiFont); redoBtn.Draw(theme, uiFont); modeBtn.Draw(theme, uiFont);

    if (list.isStepByStep) { prevBtn.Draw(theme, uiFont); nextBtn.Draw(theme, uiFont); } 
    else { Theme disabledTheme = theme; disabledTheme.btnGradStart = theme.panelBorder; disabledTheme.btnGradEnd = theme.panelBorder; prevBtn.Draw(disabledTheme, uiFont); nextBtn.Draw(disabledTheme, uiFont); }

    // DỜI SLIDER SANG 1120px
    float sliderX = 1120.0f; float botY = sh - 95.0f;
    DrawTextEx(monoFont, "Speed:", {sliderX - 110, botY + 25}, 24, 1.0f, theme.textMuted);
    DrawRectangleRounded({sliderX, botY + 35, 200, 8}, 0.5f, 10, theme.panelBorder);
    DrawRectangleRounded({sliderX, botY + 35, ((list.speedMultiplier - 0.5f) / 4.5f) * 200.0f, 8}, 0.5f, 10, theme.btnGradEnd);
    DrawCircle(sliderX + ((list.speedMultiplier - 0.5f) / 4.5f) * 200.0f, botY + 39, 12, theme.textMain); 

    list.Draw(theme, uiFont, monoFont, layout);
}