#ifndef LIST_SCREEN_H
#define LIST_SCREEN_H

#include "Screens/Screen.h"
#include "Core/UILayout.h"
#include "UI/Button.h"
#include "UI/InputBox.h"
#include "DataStructures/SinglyLinkedList.h"

class ListScreen : public Screen {
private:
    UILayout layout;
    SinglyLinkedList list;

    Button homeBtn;

    // Hàng 1 (Nút thao tác 1 giá trị)
    InputBox valueInput;
    Button addBtn;
    Button delBtn;
    Button searchBtn;
    Button updateBtn;

    // Hàng 2 (Nút thao tác mảng)
    InputBox arrayInput;
    Button initBtn;
    Button fileBtn;
    Button randomBtn;
    Button clearBtn;

    // Cụm điều khiển dưới cùng
    Button undoBtn;
    Button redoBtn;
    Button prevBtn;
    Button modeBtn;
    Button nextBtn;

    void HandleActions();

public:
    ListScreen();
    void Update(AppState& currentState) override;
    void Draw(Theme theme, Font uiFont, Font monoFont) override;
    bool IsAnyInputActive() {
        return valueInput.IsActive() || arrayInput.IsActive();
    }
};

#endif