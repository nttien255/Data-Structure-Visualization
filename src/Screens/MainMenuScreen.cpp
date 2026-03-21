#include "Screens/MainMenuScreen.h"

MainMenuScreen::MainMenuScreen() 
    // Khởi tạo nút Exit ở ngay dòng Initializer List
    : exitBtn((1600 - 300) / 2.0f, 750.0f, 300, 70, "Exit App", IconType::NONE)
{
    splashTimer = 0.0f;
    isSplashDone = false;

    float btnW = 450;
    float btnH = 100;
    
    float startX = (1600 / 2.0f) - btnW - 20.0f; 
    float startX2 = (1600 / 2.0f) + 20.0f;       
    float startY = 300.0f;                    

    buttons.push_back(Button(startX, startY, btnW, btnH, "Singly Linked List", IconType::SINGLY_LIST));
    buttons.push_back(Button(startX2, startY, btnW, btnH, "Min / Max Heap", IconType::HEAP));
    
    buttons.push_back(Button(startX, startY + 130.0f, btnW, btnH, "Red Black Tree", IconType::RBTREE));
    buttons.push_back(Button(startX2, startY + 130.0f, btnW, btnH, "Trie (Prefix Tree)", IconType::TRIE));
    
    buttons.push_back(Button(startX, startY + 260.0f, btnW, btnH, "MST Algorithm", IconType::MST));
    buttons.push_back(Button(startX2, startY + 260.0f, btnW, btnH, "BFS Algorithm", IconType::BFS));
}

void MainMenuScreen::Update(AppState& currentState) {
    if (!isSplashDone) {
        splashTimer += GetFrameTime();
        if (splashTimer >= 2.0f) isSplashDone = true;
        return; 
    }

    if (buttons[0].Update()) currentState = AppState::SINGLY_LINKED_LIST;
    if (buttons[1].Update()) currentState = AppState::MIN_HEAP;
    if (buttons[2].Update()) currentState = AppState::RBTREE;
    if (buttons[3].Update()) currentState = AppState::TRIE;     
    if (buttons[4].Update()) currentState = AppState::GRAPH_MST; 
    if (buttons[5].Update()) currentState = AppState::GRAPH_BFS; 

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
    DrawTextEx(uiFont, header, {(float)(screenW - hSize.x)/2, 120}, 50, 1.0f, theme.textMain);
    DrawTextEx(uiFont, "Press [T] to toggle Dark/Light Mode", {(float)(screenW/2) - 180, 190}, 20, 1.0f, theme.textMuted);

    for (auto& btn : buttons) btn.Draw(theme, uiFont);
    
    // Vẽ nút Exit
    exitBtn.Draw(theme, uiFont);
}