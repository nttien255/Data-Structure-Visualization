#ifndef TRIE_SCREEN_H
#define TRIE_SCREEN_H

#include "Screens/Screen.h"
#include "UI/Button.h"
#include "UI/InputBox.h"
#include "DataStructures/Trie.h" 

class TrieScreen : public Screen {
private:
    Button homeBtn;
    
    // Hàng 1: Thao tác với 1 từ (Word)
    InputBox inputBox;
    Button addBtn;
    Button delBtn;
    Button searchBtn;

    // Hàng 2: Khởi tạo với nhiều từ (Words)
    InputBox arrayInput; 
    Button initArrayBtn; 
    Button loadFileBtn; 
    Button clearBtn;

    // Cụm điều khiển Animation
    Button modeBtn; 
    Button prevBtn;
    Button nextBtn;

    Trie trie; 

public:
    TrieScreen();
    void Update(AppState& currentState) override;
    void Draw(Theme theme, Font uiFont, Font monoFont) override;
    bool IsAnyInputActive() {
        return inputBox.IsActive() || arrayInput.IsActive();
    }
};

#endif