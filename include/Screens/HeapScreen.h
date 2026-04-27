#ifndef HEAP_SCREEN_H
#define HEAP_SCREEN_H

#include "Screens/Screen.h"
#include "UI/Button.h"
#include "UI/InputBox.h"
#include "DataStructures/Heap.h"

class HeapScreen : public Screen {
private:
    Button homeBtn;
    InputBox inputBox;
    Button addBtn, extractBtn, toggleBtn;
    
    InputBox arrayInput;
    Button initBtn, randomBtn, clearBtn;
    
    Button undoBtn, redoBtn, prevBtn, modeBtn, nextBtn;
    Heap heap;

public:
    HeapScreen();
    void Update(AppState& currentState) override;
    void Draw(Theme theme, Font uiFont, Font monoFont) override;
    bool IsAnyInputActive() { return inputBox.IsActive() || arrayInput.IsActive(); }
};

#endif