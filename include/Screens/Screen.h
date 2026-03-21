#ifndef SCREEN_H
#define SCREEN_H

#include "Core/AppState.h"
#include "Core/Theme.h"

class Screen {
public:
    virtual ~Screen() = default;
    virtual void Update(AppState& currentState) = 0;
    
    // Cập nhật: Truyền vào cả 2 font (UI Font cho giao diện, Mono Font cho số/code)
    virtual void Draw(Theme theme, Font uiFont, Font monoFont) = 0; 
};

#endif