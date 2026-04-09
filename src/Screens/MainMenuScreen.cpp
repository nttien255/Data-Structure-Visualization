#include "Screens/MainMenuScreen.h"

MainMenuScreen::MainMenuScreen() 
    : exitBtn(0, 0, 400, 90, "Exit App", IconType::NONE) // Nút Exit cũng to hơn (cao 90)
{
    splashTimer = 0.0f;
    isSplashDone = false;

    // TĂNG KÍCH THƯỚC NÚT CỰC ĐẠI
    float btnW = 700; // Cũ: 640
    float btnH = 140; // Cũ: 120
    
    menuButtons.push_back(Button(0, 0, btnW, btnH, "Singly Linked List", IconType::SINGLY_LIST));
    menuButtons.push_back(Button(0, 0, btnW, btnH, "Min / Max Heap", IconType::HEAP));
    menuButtons.push_back(Button(0, 0, btnW, btnH, "Red Black Tree", IconType::RBTREE));
    menuButtons.push_back(Button(0, 0, btnW, btnH, "Trie (Prefix Tree)", IconType::TRIE));
    menuButtons.push_back(Button(0, 0, btnW, btnH, "MST Algorithm", IconType::MST));
    menuButtons.push_back(Button(0, 0, btnW, btnH, "BFS Algorithm", IconType::BFS));
}

void MainMenuScreen::Update(AppState& currentState) {
    if (!isSplashDone) {
        splashTimer += GetFrameTime();
        if (splashTimer >= 2.0f) isSplashDone = true;
        return; 
    }

    float sw = GetScreenWidth();
    float sh = GetScreenHeight();

    // ĐỒNG BỘ THÔNG SỐ VỚI HÀM CONSTRUCTOR Ở TRÊN
    float btnW = 700.0f; 
    float btnH = 140.0f;
    float gapX = 60.0f;  // Thu hẹp gap ngang một xíu để nhường chỗ cho nút to ra
    float gapY = 40.0f;  

    float col1X = (sw / 2.0f) - btnW - (gapX / 2.0f);
    float col2X = (sw / 2.0f) + (gapX / 2.0f);

    int dsRows = (menuButtons.size() + 1) / 2; 
    float totalHeight = (btnH * (dsRows + 1)) + (gapY * dsRows); 
    float startY = (sh - totalHeight) / 2.0f + 80.0f; 

    for (int i = 0; i < menuButtons.size(); i++) {
        float x = (i % 2 == 0) ? col1X : col2X;
        float y = startY + (i / 2) * (btnH + gapY);
        menuButtons[i].SetPosition(x, y);
    }

    exitBtn.SetPosition((sw - 400.0f) / 2.0f, startY + dsRows * (btnH + gapY));

    if (menuButtons[0].Update()) currentState = AppState::SINGLY_LINKED_LIST;
    if (menuButtons[1].Update()) currentState = AppState::MIN_HEAP;
    if (menuButtons[2].Update()) currentState = AppState::RBTREE;
    if (menuButtons[3].Update()) currentState = AppState::TRIE;     
    if (menuButtons[4].Update()) currentState = AppState::GRAPH_MST; 
    if (menuButtons[5].Update()) currentState = AppState::GRAPH_BFS; 

    if (exitBtn.Update()) {
        currentState = AppState::EXIT_APP;
    }
}

void MainMenuScreen::Draw(Theme theme, Font uiFont, Font monoFont) {
    int screenW = GetScreenWidth();
    int screenH = GetScreenHeight();

    if (!isSplashDone) {
        const char* title = "DATA STRUCTURE VISUALIZER";
        Vector2 tSize = MeasureTextEx(uiFont, title, 80, 2.0f);
        
        float textX = (screenW - tSize.x) / 2.0f;
        float textY = screenH / 2.0f - 60.0f;

        DrawTextEx(uiFont, title, {textX + 4.0f, textY + 4.0f}, 80, 2.0f, GetColor(0x00000080));
        DrawTextEx(uiFont, title, {textX, textY}, 80, 2.0f, theme.textMain);
        
        float progress = splashTimer / 2.0f;
        DrawRectangle(screenW/2 - 300, screenH/2 + 70, 600, 10, theme.panelBorder); 
        DrawRectangle(screenW/2 - 300, screenH/2 + 70, (int)(600 * progress), 10, theme.nodeHighlight); 
        return; 
    }

    float btnH = 140.0f;
    float gapY = 40.0f;
    
    int dsRows = (menuButtons.size() + 1) / 2;
    float totalHeight = (btnH * (dsRows + 1)) + (gapY * dsRows);
    float startY = (screenH - totalHeight) / 2.0f + 80.0f;
    
    // Đẩy cụm tiêu đề lên thêm một chút để không chạm vào nút
    float headerY = startY - 200.0f; 

    const char* header = "Select Data Structure";
    Vector2 hSize = MeasureTextEx(uiFont, header, 76, 1.0f); // Tăng size chữ tiêu đề
    DrawTextEx(uiFont, header, {(float)(screenW - hSize.x)/2, headerY}, 76, 1.0f, theme.textMain);
    
    const char* sub = "Press [T] to toggle Dark/Light Mode";
    Vector2 subSize = MeasureTextEx(monoFont, sub, 32, 1.0f); // Tăng size chữ phụ
    DrawTextEx(monoFont, sub, {(float)(screenW - subSize.x)/2, headerY + 100.0f}, 32, 1.0f, theme.textMuted);

    for (auto& btn : menuButtons) btn.Draw(theme, uiFont);
    exitBtn.Draw(theme, uiFont);
}