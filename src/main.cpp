#include "raylib.h"
#include "Core/AppState.h"
#include "Core/Theme.h"
#include "Screens/MainMenuScreen.h"
#include "Screens/ListScreen.h" 

int main() {
    InitWindow(1600, 900, "Data Structure Visualizer");
    ToggleFullscreen();
    SetTargetFPS(60);

    // ==========================================
    // KHÓA PHÍM ESC: Không cho phép thoát app đột ngột
    // ==========================================
    SetExitKey(KEY_NULL); 

    Font uiFont = LoadFontEx("../assets/SF-Pro-Rounded-Regular.ttf", 100, 0, 0); 
    Font monoFont = LoadFontEx("../assets/FiraCode-Regular.ttf", 100, 0, 0); 
    
    if (uiFont.texture.id == 0) uiFont = GetFontDefault();
    if (monoFont.texture.id == 0) monoFont = GetFontDefault();

    bool isDarkMode = true;
    AppState currentState = AppState::MAIN_MENU; 

    MainMenuScreen mainMenu;
    ListScreen listScreen; 

    while (!WindowShouldClose()) {
        // ==========================================
        // LẮNG NGHE LỆNH EXIT TỪ NÚT BẤM:
        // Nếu nút Exit được bấm, thoát khỏi vòng lặp vô tận
        // ==========================================
        if (currentState == AppState::EXIT_APP) {
            break; 
        }

        if (IsKeyPressed(KEY_T)) isDarkMode = !isDarkMode;
        Theme currentTheme = isDarkMode ? DarkTheme : LightTheme;

        // UPDATE
        switch (currentState) {
            case AppState::MAIN_MENU:
                mainMenu.Update(currentState);
                break;
            case AppState::SINGLY_LINKED_LIST:
                listScreen.Update(currentState);
                break;
            default: break;
        }

        // DRAW
        BeginDrawing();
        ClearBackground(currentTheme.background); 
        
        switch (currentState) {
            case AppState::MAIN_MENU:
                mainMenu.Draw(currentTheme, uiFont, monoFont);
                break;
            case AppState::SINGLY_LINKED_LIST:
                listScreen.Draw(currentTheme, uiFont, monoFont); 
                break;
            default: break;
        }

        EndDrawing();
    }

    UnloadFont(uiFont);
    UnloadFont(monoFont);
    CloseWindow();
    return 0;
}