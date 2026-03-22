#include "raylib.h"
#include "Core/AppState.h"
#include "Core/Theme.h"
#include "Screens/MainMenuScreen.h"
#include "Screens/ListScreen.h"

int main() {
    // Windowed mode with reasonable default size
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(1280, 720, "Data Structure Visualizer");
    SetWindowMinSize(1024, 600);
    SetTargetFPS(60);
    SetExitKey(KEY_NULL);

    Font uiFont = LoadFontEx("../assets/SF-Pro-Rounded-Regular.ttf", 64, 0, 0);
    Font monoFont = LoadFontEx("../assets/FiraCode-Regular.ttf", 64, 0, 0);

    if (uiFont.texture.id == 0) uiFont = GetFontDefault();
    if (monoFont.texture.id == 0) monoFont = GetFontDefault();

    SetTextureFilter(uiFont.texture, TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(monoFont.texture, TEXTURE_FILTER_BILINEAR);

    bool isDarkMode = true;
    AppState currentState = AppState::MAIN_MENU;

    MainMenuScreen mainMenu;
    ListScreen listScreen;

    while (!WindowShouldClose()) {
        if (currentState == AppState::EXIT_APP) break;

        if (IsKeyPressed(KEY_T)) isDarkMode = !isDarkMode;
        if (IsKeyPressed(KEY_F11)) ToggleFullscreen();

        Theme currentTheme = isDarkMode ? DarkTheme : LightTheme;

        switch (currentState) {
            case AppState::MAIN_MENU:
                mainMenu.Update(currentState);
                break;
            case AppState::SINGLY_LINKED_LIST:
                listScreen.Update(currentState);
                break;
            default:
                break;
        }

        BeginDrawing();
        ClearBackground(currentTheme.background);

        switch (currentState) {
            case AppState::MAIN_MENU:
                mainMenu.Draw(currentTheme, uiFont, monoFont);
                break;
            case AppState::SINGLY_LINKED_LIST:
                listScreen.Draw(currentTheme, uiFont, monoFont);
                break;
            default:
                break;
        }

        // Footer hint
        const char* hint = "[T] Theme  [F11] Fullscreen";
        Vector2 hintSize = MeasureTextEx(monoFont, hint, 16, 1.0f);
        DrawTextEx(monoFont, hint, 
            {(float)GetScreenWidth() - hintSize.x - 15, (float)GetScreenHeight() - 25}, 
            16, 1.0f, currentTheme.textMuted);

        EndDrawing();
    }

    UnloadFont(uiFont);
    UnloadFont(monoFont);
    CloseWindow();
    return 0;
}
