#ifndef BFS_SCREEN_H
#define BFS_SCREEN_H

#include "Screens/Screen.h"
#include "UI/Button.h"
#include "UI/InputBox.h"
#include "DataStructures/GraphBFS.h"

class BFSScreen : public Screen {
private:
    Button homeBtn;
    InputBox edgeInput, oldNodeInput, newNodeInput, srcInput, arrayInput;
    Button addBtn, delBtn, updateBtn, runBtn, initBtn, randomBtn, clearBtn;
    Button undoBtn, redoBtn, prevBtn, modeBtn, nextBtn;
    GraphBFS graph;

public:
    BFSScreen();
    void Update(AppState& currentState) override;
    void Draw(Theme theme, Font uiFont, Font monoFont) override;
    bool IsAnyInputActive() { return edgeInput.IsActive() || oldNodeInput.IsActive() || newNodeInput.IsActive() || srcInput.IsActive() || arrayInput.IsActive(); }
};
#endif