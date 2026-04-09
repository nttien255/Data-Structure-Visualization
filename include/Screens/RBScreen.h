#ifndef RB_SCREEN_H
#define RB_SCREEN_H

#include "Screens/Screen.h"
#include "UI/Button.h"
#include "UI/InputBox.h"
#include "DataStructures/RBTree.h"

class RBScreen : public Screen {
private:
    Button homeBtn;
    InputBox inputBox;
    Button addBtn, delBtn, searchBtn;
    InputBox arrayInput;
    Button initBtn, randomBtn, clearBtn;
    Button undoBtn, redoBtn, prevBtn, modeBtn, nextBtn;
    RBTree rbt;

public:
    RBScreen();
    void Update(AppState& currentState) override;
    void Draw(Theme theme, Font uiFont, Font monoFont) override;
    bool IsAnyInputActive() { return inputBox.IsActive() || arrayInput.IsActive(); }
};

#endif