#ifndef LIST_SCREEN_H
#define LIST_SCREEN_H

#include "Screens/Screen.h"
#include "UI/Button.h"
#include "UI/InputBox.h"
#include "DataStructures/SinglyLinkedList.h" 

class ListScreen : public Screen {
private:
    Button homeBtn;
    
    InputBox inputBox;
    Button addBtn;
    Button delBtn; // <--- Thêm nút Delete
    Button searchBtn;

    InputBox arrayInput; 
    Button initArrayBtn; 
    Button loadFileBtn; 
    Button clearBtn;

    Button modeBtn; 
    Button prevBtn;
    Button nextBtn;

    SinglyLinkedList list; 

public:
    ListScreen();
    void Update(AppState& currentState) override;
    void Draw(Theme theme, Font uiFont, Font monoFont) override;
};

#endif