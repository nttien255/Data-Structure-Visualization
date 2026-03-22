#ifndef MAIN_MENU_SCREEN_H
#define MAIN_MENU_SCREEN_H

#include "Screens/Screen.h"
#include "UI/Button.h"
#include <vector>

class MainMenuScreen : public Screen {
private:
    float splashTimer;
    bool isSplashDone;
    std::vector<Button> menuButtons;
    Button exitBtn;

public:
    MainMenuScreen();
    void Update(AppState& currentState) override;
    void Draw(Theme theme, Font uiFont, Font monoFont) override;
};

#endif
