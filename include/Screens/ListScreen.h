#ifndef LIST_SCREEN_H
#define LIST_SCREEN_H

#include "Screens/Screen.h"
#include "Core/UILayout.h"
#include "UI/Button.h"
#include "UI/InputBox.h"
#include "UI/ControlPanel.h"
#include "DataStructures/SinglyLinkedList.h"

class ListScreen : public Screen {
private:
    UILayout layout;
    Button homeBtn;
    ControlPanel controlPanel;
    SinglyLinkedList list;

    void HandleActions();

public:
    ListScreen();
    void Update(AppState& currentState) override;
    void Draw(Theme theme, Font uiFont, Font monoFont) override;
};

#endif
