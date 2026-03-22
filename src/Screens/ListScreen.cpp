#include "Screens/ListScreen.h"
#include "UI/SystemUtils.h"
#include <sstream>

// Mở rộng nút Home thành 130px để không bị tràn chữ
ListScreen::ListScreen() : homeBtn(20, 20, 130, 45, "Home", IconType::HOME) {
    list.InitRandom(5);
}

void ListScreen::Update(AppState& currentState) {
    layout.Update();
    controlPanel.UpdateLayout(layout);
    controlPanel.Update();
    
    // Cập nhật tọa độ và kích thước nút Home bám theo Layout
    homeBtn.SetBounds(layout.padding, layout.padding, 130, 45); 
    
    if (homeBtn.Update()) { currentState = AppState::MAIN_MENU; return; }
    
    HandleActions();
    
    if (!controlPanel.IsStepMode()) list.speedMultiplier = controlPanel.GetSpeed();
    list.isStepByStep = controlPanel.IsStepMode();
    list.Update();
}

void ListScreen::HandleActions() {
    std::string valStr = controlPanel.GetValueText();
    
    if (controlPanel.IsAddClicked() && !valStr.empty()) {
        try { list.AddTail(std::stoi(valStr)); controlPanel.ClearValueInput(); } catch (...) {}
    }
    if (controlPanel.IsDeleteClicked() && !valStr.empty()) {
        try { list.Delete(std::stoi(valStr)); controlPanel.ClearValueInput(); } catch (...) {}
    }
    if (controlPanel.IsSearchClicked() && !valStr.empty()) {
        try { list.Search(std::stoi(valStr)); controlPanel.ClearValueInput(); } catch (...) {}
    }
    
    if (controlPanel.IsUpdateClicked() && !valStr.empty()) {
        try { 
            if (list.selectedIndex != -1) {
                list.UpdateNode(std::stoi(valStr)); 
                controlPanel.ClearValueInput(); 
            }
        } catch (...) {}
    }
    
    if (controlPanel.IsRandomClicked()) {
        list.AddTail(GetRandomValue(10, 99));
    }
    
    // ==========================================================
    // SỬA LỖI Ở ĐÂY: Lắng nghe sự kiện Next / Prev trong chế độ Step
    // Hỗ trợ cả click chuột lẫn phím Mũi tên Trái / Phải
    // ==========================================================
    if (controlPanel.IsStepMode()) {
        if (controlPanel.IsPrevClicked() || IsKeyPressed(KEY_LEFT)) list.StepBackward();
        if (controlPanel.IsNextClicked() || IsKeyPressed(KEY_RIGHT)) list.StepForward();
    }
    
    if (controlPanel.IsUndoClicked()) list.Undo();
    if (controlPanel.IsRedoClicked()) list.Redo();

    if (controlPanel.IsInitArrayClicked()) {
        std::string arrStr = controlPanel.GetArrayText();
        if (!arrStr.empty()) {
            std::stringstream ss(arrStr); std::vector<int> arr; std::string token;
            while (ss >> token) { try { arr.push_back(std::stoi(token)); } catch (...) {} }
            if (!arr.empty()) { list.InitFromArray(arr); controlPanel.ClearArrayInput(); }
        }
    }
    
    if (controlPanel.IsLoadFileClicked()) {
        std::string path = OpenWindowsFileDialog(GetWindowHandle());
        if (!path.empty()) {
            char* content = LoadFileText(path.c_str());
            if (content) {
                std::stringstream ss(content); std::vector<int> arr; std::string token;
                while (ss >> token) { try { arr.push_back(std::stoi(token)); } catch (...) {} }
                UnloadFileText(content);
                if (!arr.empty()) list.InitFromArray(arr);
            }
        }
    }
    
    if (controlPanel.IsClearClicked()) list.ClearList();
}

void ListScreen::Draw(Theme theme, Font uiFont, Font monoFont) {
    controlPanel.Draw(theme, uiFont, monoFont);
    homeBtn.Draw(theme, uiFont);
    list.Draw(theme, uiFont, monoFont, layout);
}