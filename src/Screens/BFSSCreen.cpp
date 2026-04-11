#include "Screens/BFSScreen.h"
#include <sstream>

BFSScreen::BFSScreen() 
    : homeBtn(20, 20, 130, 75, "Home", IconType::NONE),
      edgeInput(170, 20, 140, 75, "u v", 15, false),
      addBtn(320, 20, 90, 75, "Add", IconType::NONE),
      delBtn(420, 20, 90, 75, "Del", IconType::NONE),
      oldNodeInput(520, 20, 90, 75, "Old", 10, true),
      newNodeInput(620, 20, 90, 75, "New", 10, true),
      updateBtn(720, 20, 110, 75, "Update", IconType::NONE),
      srcInput(840, 20, 100, 75, "Src", 10, true),
      runBtn(950, 20, 130, 75, "Run BFS", IconType::NONE),
      
      arrayInput(170, 110, 400, 75, "1 2, 2 3", 60, false),
      initBtn(580, 110, 120, 75, "Init", IconType::NONE),
      randomBtn(710, 110, 120, 75, "Random", IconType::NONE), // VỊ TRÍ ĐÃ ĐƯỢC CHUẨN HOÁ
      clearBtn(840, 110, 120, 75, "Clear", IconType::NONE),
      
      undoBtn(20, 0, 150, 75, "Undo", IconType::NONE), redoBtn(190, 0, 150, 75, "Redo", IconType::NONE),
      prevBtn(360, 0, 120, 75, "<<", IconType::NONE), modeBtn(500, 0, 240, 75, "Mode: Auto", IconType::NONE), nextBtn(760, 0, 120, 75, ">>", IconType::NONE)
{}

void BFSScreen::Update(AppState& currentState) {
    float sh = GetScreenHeight(); float botY = sh - 95.0f;
    undoBtn.SetPosition(20, botY); redoBtn.SetPosition(190, botY); prevBtn.SetPosition(360, botY); modeBtn.SetPosition(500, botY); nextBtn.SetPosition(760, botY);

    edgeInput.Update(); oldNodeInput.Update(); newNodeInput.Update(); srcInput.Update(); arrayInput.Update(); graph.Update();
    if (homeBtn.Update()) currentState = AppState::MAIN_MENU;

    if (addBtn.Update()) {
        std::stringstream ss(edgeInput.GetText()); int u, v;
        if (ss >> u) { if (ss >> v) graph.AddEdge(u, v); else graph.AddVertex(u); edgeInput.Clear(); }
    }
    if (delBtn.Update()) { std::stringstream ss(edgeInput.GetText()); int u; if (ss >> u) { graph.DeleteVertex(u); edgeInput.Clear(); } }
    
    if (updateBtn.Update()) {
        std::string oldStr = oldNodeInput.GetText(); std::string newStr = newNodeInput.GetText();
        if (!oldStr.empty() && !newStr.empty()) { graph.UpdateVertex(std::stoi(oldStr), std::stoi(newStr)); oldNodeInput.Clear(); newNodeInput.Clear(); }
    }
    
    if (runBtn.Update()) {
        std::string srcStr = srcInput.GetText();
        if (!srcStr.empty()) { graph.RunBFS(std::stoi(srcStr)); srcInput.Clear(); }
    }
    
    if (initBtn.Update()) {
        std::string text = arrayInput.GetText(); std::vector<std::pair<int, int>> edgeList; std::vector<int> isolated;
        std::stringstream ss(text); std::string token;
        while (std::getline(ss, token, ',')) {
            std::stringstream ts(token); int u, v;
            if (ts >> u) { if (ts >> v) edgeList.push_back({u, v}); else isolated.push_back(u); }
        }
        if(!edgeList.empty() || !isolated.empty()){ graph.InitFromEdges(edgeList, isolated); arrayInput.Clear(); }
    }
    
    if (randomBtn.Update()) graph.InitRandom(6);
    if (clearBtn.Update()) graph.ClearGraph();
    if (undoBtn.Update()) graph.Undo(); if (redoBtn.Update()) graph.Redo();
    if (modeBtn.Update()) { graph.isStepByStep = !graph.isStepByStep; modeBtn.SetText(graph.isStepByStep ? "Mode: Step" : "Mode: Auto"); }
    if (graph.isStepByStep) { if (prevBtn.Update()) graph.StepBackward(); if (nextBtn.Update()) graph.StepForward(); }

    float sliderX = 1150.0f;
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), {sliderX, botY + 15, 200, 40})) {
        float pct = (GetMousePosition().x - sliderX) / 200.0f;
        graph.speedMultiplier = 0.5f + (std::max(0.0f, std::min(1.0f, pct)) * 4.5f);
    }
}

void BFSScreen::Draw(Theme theme, Font uiFont, Font monoFont) {
    float sh = GetScreenHeight();
    homeBtn.Draw(theme, uiFont); edgeInput.Draw(theme, uiFont); addBtn.Draw(theme, uiFont); delBtn.Draw(theme, uiFont);
    oldNodeInput.Draw(theme, uiFont); newNodeInput.Draw(theme, uiFont); updateBtn.Draw(theme, uiFont);
    srcInput.Draw(theme, uiFont); runBtn.Draw(theme, uiFont);
    arrayInput.Draw(theme, uiFont); initBtn.Draw(theme, uiFont); randomBtn.Draw(theme, uiFont); clearBtn.Draw(theme, uiFont);
    undoBtn.Draw(theme, uiFont); redoBtn.Draw(theme, uiFont); modeBtn.Draw(theme, uiFont);
    if (graph.isStepByStep) { prevBtn.Draw(theme, uiFont); nextBtn.Draw(theme, uiFont); } else { Theme disabledTheme = theme; disabledTheme.btnGradStart = theme.panelBorder; disabledTheme.btnGradEnd = theme.panelBorder; prevBtn.Draw(disabledTheme, uiFont); nextBtn.Draw(disabledTheme, uiFont); }

    float sliderX = 1150.0f; float botY = sh - 95.0f;
    DrawTextEx(monoFont, "Speed:", {sliderX - 110, botY + 25}, 24, 1.0f, theme.textMuted); DrawRectangleRounded({sliderX, botY + 35, 200, 8}, 0.5f, 10, theme.panelBorder);
    DrawRectangleRounded({sliderX, botY + 35, ((graph.speedMultiplier - 0.5f)/4.5f)*200, 8}, 0.5f, 10, theme.btnGradEnd); DrawCircle(sliderX + ((graph.speedMultiplier - 0.5f)/4.5f)*200, botY + 39, 12, theme.textMain);
    graph.Draw(theme, uiFont, monoFont);
}