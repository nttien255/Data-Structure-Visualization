#include "Screens/TrieScreen.h"
#include "UI/SystemUtils.h" 
#include <sstream>

TrieScreen::TrieScreen() 
    : homeBtn(20, 20, 130, 75, "Home", IconType::NONE),
      inputBox(170, 20, 240, 75, "Word:", 15), 
      addBtn(430, 20, 150, 75, "Insert", IconType::NONE),
      delBtn(600, 20, 150, 75, "Delete", IconType::NONE),
      searchBtn(770, 20, 150, 75, "Search", IconType::NONE),
      arrayInput(170, 110, 240, 75, "Words:", 60),
      initArrayBtn(430, 110, 150, 75, "Init", IconType::NONE),
      loadFileBtn(600, 110, 150, 75, "File", IconType::NONE),
      clearBtn(770, 110, 150, 75, "Clear", IconType::NONE),
      
      undoBtn(20, 0, 150, 75, "Undo", IconType::NONE), 
      redoBtn(190, 0, 150, 75, "Redo", IconType::NONE),
      
      prevBtn(360, 0, 120, 75, "<<", IconType::NONE),
      modeBtn(500, 0, 240, 75, "Mode: Auto", IconType::NONE),
      nextBtn(760, 0, 120, 75, ">>", IconType::NONE)
{ 
}

void TrieScreen::Update(AppState& currentState) {
    float sh = GetScreenHeight(); float botY = sh - 95.0f;
    prevBtn.SetPosition(360, botY); modeBtn.SetPosition(500, botY); nextBtn.SetPosition(760, botY);
    undoBtn.SetPosition(20, botY); redoBtn.SetPosition(190, botY);
    
    inputBox.Update(); arrayInput.Update(); trie.Update(); 
    if (homeBtn.Update()) currentState = AppState::MAIN_MENU;

    if (addBtn.Update()) { std::string valStr = inputBox.GetText(); if (!valStr.empty()) { trie.Insert(valStr); inputBox.Clear(); } }
    if (delBtn.Update()) { std::string valStr = inputBox.GetText(); if (!valStr.empty()) { trie.Delete(valStr); inputBox.Clear(); } }
    if (searchBtn.Update()) { std::string valStr = inputBox.GetText(); if (!valStr.empty()) { trie.Search(valStr); inputBox.Clear(); } }
    if (clearBtn.Update()) { trie.ClearList(); } 
    if (undoBtn.Update()) trie.Undo();
    if (redoBtn.Update()) trie.Redo();
    if (initArrayBtn.Update()) {
        std::stringstream ss(arrayInput.GetText()); std::vector<std::string> arr; std::string temp;
        while (ss >> temp) { arr.push_back(temp); }
        if (!arr.empty()) { trie.InitFromArray(arr); arrayInput.Clear(); }
    }

    if (loadFileBtn.Update()) {
        bool isFull = IsWindowFullscreen(); if (isFull) ToggleFullscreen(); 
        std::string filePath = OpenWindowsFileDialog(GetWindowHandle()); if (isFull) ToggleFullscreen(); 
        if (!filePath.empty()) {
            char* fileText = LoadFileText(filePath.c_str());
            if (fileText != nullptr) {
                std::stringstream ss(fileText); std::vector<std::string> arr; std::string temp;
                while (ss >> temp) { arr.push_back(temp); }
                UnloadFileText(fileText);
                if (!arr.empty()) { trie.InitFromArray(arr); }
            }
        }
    }

    if (modeBtn.Update()) { trie.isStepByStep = !trie.isStepByStep; if (trie.isStepByStep) modeBtn.SetText("Mode: Step"); else modeBtn.SetText("Mode: Auto"); }
    if (trie.isStepByStep) { if (prevBtn.Update()) trie.StepBackward(); if (nextBtn.Update()) trie.StepForward(); }

    // DỜI SLIDER SANG 1120px
    float sliderX = 1120.0f; Rectangle sliderHitbox = {sliderX, botY + 15, 200, 40}; 
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        Vector2 mousePos = GetMousePosition();
        if (CheckCollisionPointRec(mousePos, sliderHitbox)) {
            float pct = (mousePos.x - sliderX) / 200.0f;
            if (pct < 0.0f) pct = 0.0f; if (pct > 1.0f) pct = 1.0f;
            trie.speedMultiplier = 0.5f + (pct * 4.5f); 
        }
    }
}

void TrieScreen::Draw(Theme theme, Font uiFont, Font monoFont) {
    float sh = (float)GetScreenHeight();
    homeBtn.Draw(theme, uiFont); inputBox.Draw(theme, uiFont); addBtn.Draw(theme, uiFont);
    delBtn.Draw(theme, uiFont); searchBtn.Draw(theme, uiFont);
    arrayInput.Draw(theme, uiFont); initArrayBtn.Draw(theme, uiFont);
    loadFileBtn.Draw(theme, uiFont); clearBtn.Draw(theme, uiFont);
    modeBtn.Draw(theme, uiFont);
    undoBtn.Draw(theme, uiFont); 
    redoBtn.Draw(theme, uiFont);
    if (trie.isStepByStep) { prevBtn.Draw(theme, uiFont); nextBtn.Draw(theme, uiFont); } 
    else { Theme disabledTheme = theme; disabledTheme.btnGradStart = theme.panelBorder; disabledTheme.btnGradEnd = theme.panelBorder; prevBtn.Draw(disabledTheme, uiFont); nextBtn.Draw(disabledTheme, uiFont); }

    // DỜI SLIDER SANG 1120px
    float sliderX = 1120.0f; float botY = sh - 95.0f;
    DrawTextEx(monoFont, "Speed:", {sliderX - 110.0f, botY + 25.0f}, 24, 1.0f, theme.textMuted);
    Rectangle sliderBar = {sliderX, botY + 35.0f, 200.0f, 8.0f}; DrawRectangleRounded(sliderBar, 0.5f, 10, theme.panelBorder);
    float fillWidth = ((trie.speedMultiplier - 0.5f) / 4.5f) * 200.0f; Rectangle fillBar = {sliderX, botY + 35.0f, fillWidth, 8.0f};
    DrawRectangleRounded(fillBar, 0.5f, 10, theme.btnGradEnd); DrawCircle(sliderX + fillWidth, botY + 39.0f, 12.0f, theme.textMain); 

    trie.Draw(theme, uiFont, monoFont);
}