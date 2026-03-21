#include "Screens/ListScreen.h"
#include "UI/SystemUtils.h" 
#include <sstream>

ListScreen::ListScreen() 
    : homeBtn(30, 30, 120, 60, "Home", IconType::NONE),
      
      // Bố trí lại Hàng 1 để nhét thêm nút Delete
      inputBox(800, 30, 180, 60, "Value:", 10), 
      addBtn(1000, 30, 100, 60, "Add", IconType::NONE),
      delBtn(1120, 30, 120, 60, "Delete", IconType::NONE),
      searchBtn(1260, 30, 120, 60, "Search", IconType::NONE),
      
      // Hàng 2 
      arrayInput(800, 110, 320, 60, "Array:", 60),
      initArrayBtn(1140, 110, 120, 60, "Init", IconType::NONE),
      loadFileBtn(1280, 110, 120, 60, "File...", IconType::NONE),
      clearBtn(1420, 110, 120, 60, "Clear", IconType::NONE),

      // KÉO DÀI NÚT MODE LÊN 300px để không bị tràn viền chữ
      prevBtn(920, 800, 80, 60, "<<", IconType::NONE),
      modeBtn(1020, 800, 300, 60, "Mode: Run Auto", IconType::NONE),
      nextBtn(1340, 800, 80, 60, ">>", IconType::NONE)
{
    list.InitRandom(5); 
}

void ListScreen::Update(AppState& currentState) {
    inputBox.Update();
    arrayInput.Update();
    list.Update(); 
    
    if (homeBtn.Update()) currentState = AppState::MAIN_MENU;

    if (addBtn.Update()) {
        std::string valStr = inputBox.GetText();
        if (!valStr.empty()) { list.AddTail(std::stoi(valStr)); inputBox.Clear(); }
    }
    
    // Nối logic nút Delete
    if (delBtn.Update()) {
        std::string valStr = inputBox.GetText();
        if (!valStr.empty()) { list.Delete(std::stoi(valStr)); inputBox.Clear(); }
    }

    if (searchBtn.Update()) {
        std::string valStr = inputBox.GetText();
        if (!valStr.empty()) { list.Search(std::stoi(valStr)); inputBox.Clear(); }
    }
    
    if (clearBtn.Update()) list.ClearList();

    if (initArrayBtn.Update()) {
        std::stringstream ss(arrayInput.GetText());
        std::vector<int> newArr;
        std::string temp;
        while (ss >> temp) { try { newArr.push_back(std::stoi(temp)); } catch (...) {} }
        if (!newArr.empty()) { list.InitFromArray(newArr); arrayInput.Clear(); }
    }

    if (loadFileBtn.Update()) {
        bool isFull = IsWindowFullscreen();
        if (isFull) ToggleFullscreen(); 

        std::string filePath = OpenWindowsFileDialog(GetWindowHandle()); 
        
        if (isFull) ToggleFullscreen(); 

        if (!filePath.empty()) {
            char* fileText = LoadFileText(filePath.c_str());
            if (fileText != nullptr) {
                std::vector<int> newArr;
                std::stringstream ss(fileText);
                std::string temp;
                bool isValid = true;
                
                while (ss >> temp) {
                    try { newArr.push_back(std::stoi(temp)); } 
                    catch (...) { isValid = false; break; }
                }
                UnloadFileText(fileText);
                
                if (isValid && !newArr.empty()) list.InitFromArray(newArr);
            }
        }
    }

    if (modeBtn.Update()) {
        list.isStepByStep = !list.isStepByStep;
        if (list.isStepByStep) modeBtn.SetText("Mode: Step-by-Step");
        else modeBtn.SetText("Mode: Run Auto");
    }

    if (list.isStepByStep) {
        if (prevBtn.Update()) list.StepBackward();
        if (nextBtn.Update()) list.StepForward();
    }

    Rectangle sliderHitbox = {290, (float)GetScreenHeight() - 60, 220, 40}; 
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        Vector2 mousePos = GetMousePosition();
        if (CheckCollisionPointRec(mousePos, sliderHitbox)) {
            float pct = (mousePos.x - 300.0f) / 200.0f;
            if (pct < 0.0f) pct = 0.0f;
            if (pct > 1.0f) pct = 1.0f;
            list.speedMultiplier = 0.5f + (pct * 4.5f); 
        }
    }
}

void ListScreen::Draw(Theme theme, Font uiFont, Font monoFont) {
    DrawTextEx(uiFont, "SINGLY LINKED LIST", {183, 45}, 40, 1.0f, GetColor(0x00000080));
    DrawTextEx(uiFont, "SINGLY LINKED LIST", {180, 42}, 40, 1.0f, theme.textMain);
    
    homeBtn.Draw(theme, uiFont);
    
    inputBox.Draw(theme, uiFont); 
    addBtn.Draw(theme, uiFont);
    delBtn.Draw(theme, uiFont); // Vẽ nút Delete
    searchBtn.Draw(theme, uiFont);
    
    arrayInput.Draw(theme, uiFont);
    initArrayBtn.Draw(theme, uiFont);
    loadFileBtn.Draw(theme, uiFont);
    clearBtn.Draw(theme, uiFont);

    modeBtn.Draw(theme, uiFont);
    if (list.isStepByStep) {
        prevBtn.Draw(theme, uiFont);
        nextBtn.Draw(theme, uiFont);
    } else {
        Theme disabledTheme = theme;
        disabledTheme.btnGradStart = theme.panelBorder;
        disabledTheme.btnGradEnd = theme.panelBorder;
        prevBtn.Draw(disabledTheme, uiFont);
        nextBtn.Draw(disabledTheme, uiFont);
    }

    const char* speedText = "Speed:";
    DrawTextEx(monoFont, speedText, {30, (float)GetScreenHeight() - 50}, 24, 1.0f, theme.textMuted);
    DrawTextEx(monoFont, "0.5x", {240, (float)GetScreenHeight() - 46}, 20, 1.0f, theme.textMuted);
    DrawTextEx(monoFont, "5.0x", {515, (float)GetScreenHeight() - 46}, 20, 1.0f, theme.textMuted);
    
    Rectangle sliderBar = {300, (float)GetScreenHeight() - 40, 200, 8};
    DrawRectangleRounded(sliderBar, 0.5f, 10, theme.panelBorder);
    float fillWidth = ((list.speedMultiplier - 0.5f) / 4.5f) * 200.0f;
    Rectangle fillBar = {300, (float)GetScreenHeight() - 40, fillWidth, 8};
    DrawRectangleRounded(fillBar, 0.5f, 10, theme.btnGradEnd);

    DrawCircle(300 + fillWidth, GetScreenHeight() - 36 + 2, 10, GetColor(0x00000060)); 
    DrawCircle(300 + fillWidth, GetScreenHeight() - 36, 10, theme.textMain); 
    DrawCircleLines(300 + fillWidth, GetScreenHeight() - 36, 10, theme.btnHoverGlow); 

    list.Draw(theme, uiFont, monoFont);
}