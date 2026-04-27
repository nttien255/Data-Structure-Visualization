#include "Screens/HeapScreen.h"
#include <sstream>

HeapScreen::HeapScreen() 
    : homeBtn(20, 20, 130, 75, "Home", IconType::NONE),
      inputBox(170, 20, 160, 75, "Value", 10, true),
      addBtn(340, 20, 130, 75, "Insert", IconType::NONE),
      extractBtn(480, 20, 180, 75, "Extract Top", IconType::NONE),
      toggleBtn(670, 20, 260, 75, "Mode: Min Heap", IconType::NONE),
      
      arrayInput(170, 110, 400, 75, "e.g. 5 3 8 1", 60, false),
      initBtn(580, 110, 120, 75, "Init", IconType::NONE),
      randomBtn(710, 110, 120, 75, "Random", IconType::NONE),
      clearBtn(840, 110, 120, 75, "Clear", IconType::NONE),
      
      undoBtn(20, 0, 150, 75, "Undo", IconType::NONE), redoBtn(190, 0, 150, 75, "Redo", IconType::NONE),
      prevBtn(360, 0, 120, 75, "<<", IconType::NONE), modeBtn(500, 0, 240, 75, "Mode: Auto", IconType::NONE), nextBtn(760, 0, 120, 75, ">>", IconType::NONE)
{}

void HeapScreen::Update(AppState& currentState) {
    float sh = GetScreenHeight(); float botY = sh - 95.0f;
    undoBtn.SetPosition(20, botY); redoBtn.SetPosition(190, botY); prevBtn.SetPosition(360, botY); modeBtn.SetPosition(500, botY); nextBtn.SetPosition(760, botY);

    inputBox.Update(); arrayInput.Update(); heap.Update();
    if (homeBtn.Update()) currentState = AppState::MAIN_MENU;

    if (addBtn.Update()) {
        std::stringstream ss(inputBox.GetText()); int val;
        if (ss >> val) { heap.Insert(val); inputBox.Clear(); }
    }
    if (extractBtn.Update()) { heap.ExtractTop(); }
    if (toggleBtn.Update()) {
        heap.ToggleHeapType();
        toggleBtn.SetText(heap.IsMaxHeap() ? "Mode: Max Heap" : "Mode: Min Heap");
    }
    
    if (initBtn.Update()) {
        std::stringstream ss(arrayInput.GetText()); std::vector<int> arr; int token;
        while (ss >> token) arr.push_back(token);
        if (!arr.empty()) { heap.InitFromArray(arr); arrayInput.Clear(); }
    }
    
    if (randomBtn.Update()) heap.InitRandom(7);
    if (clearBtn.Update()) heap.ClearHeap();
    if (undoBtn.Update()) heap.Undo(); if (redoBtn.Update()) heap.Redo();

    if (modeBtn.Update()) { heap.isStepByStep = !heap.isStepByStep; modeBtn.SetText(heap.isStepByStep ? "Mode: Step" : "Mode: Auto"); }
    if (heap.isStepByStep) { if (prevBtn.Update()) heap.StepBackward(); if (nextBtn.Update()) heap.StepForward(); }

    float sliderX = 1150.0f;
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), {sliderX, botY + 15, 200, 40})) {
        float pct = (GetMousePosition().x - sliderX) / 200.0f;
        heap.speedMultiplier = 0.5f + (std::max(0.0f, std::min(1.0f, pct)) * 4.5f);
    }
}

void HeapScreen::Draw(Theme theme, Font uiFont, Font monoFont) {
    float sh = GetScreenHeight();
    homeBtn.Draw(theme, uiFont); inputBox.Draw(theme, uiFont); addBtn.Draw(theme, uiFont); 
    extractBtn.Draw(theme, uiFont); toggleBtn.Draw(theme, uiFont);
    arrayInput.Draw(theme, uiFont); initBtn.Draw(theme, uiFont); randomBtn.Draw(theme, uiFont); clearBtn.Draw(theme, uiFont);
    undoBtn.Draw(theme, uiFont); redoBtn.Draw(theme, uiFont); modeBtn.Draw(theme, uiFont);
    
    if (heap.isStepByStep) { prevBtn.Draw(theme, uiFont); nextBtn.Draw(theme, uiFont); } 
    else { Theme disabledTheme = theme; disabledTheme.btnGradStart = theme.panelBorder; disabledTheme.btnGradEnd = theme.panelBorder; prevBtn.Draw(disabledTheme, uiFont); nextBtn.Draw(disabledTheme, uiFont); }

    float sliderX = 1150.0f; float botY = sh - 95.0f;
    DrawTextEx(monoFont, "Speed:", {sliderX - 110, botY + 25}, 24, 1.0f, theme.textMuted); DrawRectangleRounded({sliderX, botY + 35, 200, 8}, 0.5f, 10, theme.panelBorder);
    DrawRectangleRounded({sliderX, botY + 35, ((heap.speedMultiplier - 0.5f)/4.5f)*200, 8}, 0.5f, 10, theme.btnGradEnd); DrawCircle(sliderX + ((heap.speedMultiplier - 0.5f)/4.5f)*200, botY + 39, 12, theme.textMain);
    
    heap.Draw(theme, uiFont, monoFont);
}