#include "Screens/MainMenuScreen.h"

MainMenuScreen::MainMenuScreen() 
    // Tọa độ nút Exit tự động căn giữa cho màn hình 1366
    : exitBtn((1366 - 300) / 2.0f, 620.0f, 300, 70, "Exit App", IconType::NONE)
{
    splashTimer = 0.0f;
    isSplashDone = false;

    float btnW = 450;
    float btnH = 80; // Giảm chiều cao nút xuống một chút cho đỡ chật
    
    float startX = (1366 / 2.0f) - btnW - 20.0f; 
    float startX2 = (1366 / 2.0f) + 20.0f;       
    float startY = 250.0f; // Đẩy toàn bộ menu lên trên một chút                    

    // Đổi tên biến thành menuButtons cho khớp với file .h của bạn
    menuButtons.push_back(Button(startX, startY, btnW, btnH, "Singly Linked List", IconType::SINGLY_LIST));
    menuButtons.push_back(Button(startX2, startY, btnW, btnH, "Min / Max Heap", IconType::HEAP));
    
    menuButtons.push_back(Button(startX, startY + 110.0f, btnW, btnH, "Red Black Tree", IconType::RBTREE));
    menuButtons.push_back(Button(startX2, startY + 110.0f, btnW, btnH, "Trie (Prefix Tree)", IconType::TRIE));
    
    menuButtons.push_back(Button(startX, startY + 220.0f, btnW, btnH, "MST Algorithm", IconType::MST));
    menuButtons.push_back(Button(startX2, startY + 220.0f, btnW, btnH, "BFS Algorithm", IconType::BFS));
}

void MainMenuScreen::Update(AppState& currentState) {
    if (!isSplashDone) {
        splashTimer += GetFrameTime();
        if (splashTimer >= 2.0f) isSplashDone = true;
        return; 
    }

    if (menuButtons[0].Update()) currentState = AppState::SINGLY_LINKED_LIST;
    if (menuButtons[1].Update()) currentState = AppState::MIN_HEAP;
    if (menuButtons[2].Update()) currentState = AppState::RBTREE;
    if (menuButtons[3].Update()) currentState = AppState::TRIE;     
    if (menuButtons[4].Update()) currentState = AppState::GRAPH_MST; 
    if (menuButtons[5].Update()) currentState = AppState::GRAPH_BFS; 

    // Bắt sự kiện bấm nút Exit
    if (exitBtn.Update()) {
        currentState = AppState::EXIT_APP;
    }
}

void MainMenuScreen::Draw(Theme theme, Font uiFont, Font monoFont) {
    int screenW = GetScreenWidth();
    int screenH = GetScreenHeight();

    if (!isSplashDone) {
        const char* title = "DATA STRUCTURE VISUALIZER";
        Vector2 tSize = MeasureTextEx(uiFont, title, 60, 2.0f);
        
        float textX = (screenW - tSize.x) / 2.0f;
        float textY = screenH / 2.0f - 50.0f;

        DrawTextEx(uiFont, title, {textX + 3.0f, textY + 3.0f}, 60, 2.0f, GetColor(0x00000080));
        DrawTextEx(uiFont, title, {textX, textY}, 60, 2.0f, theme.textMain);
        
        float progress = splashTimer / 2.0f;
        DrawRectangle(screenW/2 - 200, screenH/2 + 50, 400, 10, theme.panelBorder); 
        DrawRectangle(screenW/2 - 200, screenH/2 + 50, (int)(400 * progress), 10, theme.nodeHighlight); 
        return; 
    }

    const char* header = "Select Data Structure";
    Vector2 hSize = MeasureTextEx(uiFont, header, 50, 1.0f);
    DrawTextEx(uiFont, header, {(float)(screenW - hSize.x)/2, 80}, 50, 1.0f, theme.textMain);
    DrawTextEx(uiFont, "Press [T] to toggle Dark/Light Mode", {(float)(screenW/2) - 180, 150}, 20, 1.0f, theme.textMuted);

    for (auto& btn : menuButtons) btn.Draw(theme, uiFont);
    
    exitBtn.Draw(theme, uiFont);
}