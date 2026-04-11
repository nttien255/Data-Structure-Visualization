#ifndef MST_SCREEN_H
#define MST_SCREEN_H

#include "Screens/Screen.h"
#include "UI/Button.h"
#include "UI/InputBox.h"
#include "DataStructures/GraphMST.h"

class MSTScreen : public Screen {
private:
    Button homeBtn;
    InputBox edgeInput, oldNodeInput, newNodeInput, arrayInput;
    Button addBtn, delBtn, updateBtn, runBtn; 
    
    // ĐÃ FIX: Thêm randomBtn vào đây để class nhận diện được
    Button initBtn, randomBtn, clearBtn; 
    
    Button undoBtn, redoBtn, prevBtn, modeBtn, nextBtn;
    GraphMST graph;

public:
    MSTScreen();
    void Update(AppState& currentState) override;
    void Draw(Theme theme, Font uiFont, Font monoFont) override;
    bool IsAnyInputActive() { return edgeInput.IsActive() || oldNodeInput.IsActive() || newNodeInput.IsActive() || arrayInput.IsActive(); }
};

#endif