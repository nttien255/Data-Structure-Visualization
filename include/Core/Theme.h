#ifndef THEME_H
#define THEME_H

#include "raylib.h"

struct Theme {
    Color background;    
    Color grid;          
    Color panelBg;       
    Color panelBorder;   
    Color textMain;      
    Color textMuted;     
    Color textHighlighted; // <--- Màu của chữ Value
    
    Color btnNormal;
    Color btnHoverBase;  
    Color btnHoverGlow;  
    Color btnGradStart;  
    Color btnGradEnd;    
    
    Color nodeNormal;    
    Color nodeHighlight; 
    Color nodeSuccess;   
    Color nodeError;     
    Color edgeNormal;    
    Color edgeHighlight; 
};

// ============================================================================
// DARK MODE
// ============================================================================
const Theme DarkTheme = {
    GetColor(0x0F172AFF), GetColor(0x1E293B88), GetColor(0x1E293BFF), GetColor(0x334155FF), 
    GetColor(0xF8FAFCFF), GetColor(0x94A3B8FF), 
    WHITE, // <--- textHighlighted: Đổi thành màu Trắng trong Dark Mode
    
    GetColor(0x2563EBFF), GetColor(0x1E293BFF), GetColor(0x38BDF8FF), 
    
    GetColor(0x1E3A8AFF), // btnGradStart
    GetColor(0x0EA5E9FF), // btnGradEnd
    
    GetColor(0x3B82F6FF), GetColor(0xF59E0BFF), GetColor(0x10B981FF), GetColor(0xEF4444FF), 
    GetColor(0x475569FF), GetColor(0xF59E0BFF)
};

// ============================================================================
// LIGHT MODE
// ============================================================================
const Theme LightTheme = {
    GetColor(0xF8FAFCFF), GetColor(0xE2E8F088), GetColor(0xFFFFFFFF), GetColor(0xCBD5E1FF), 
    GetColor(0x0F172AFF), GetColor(0x64748BFF), 
    GetColor(0x0F172AFF), // <--- textHighlighted: Đổi thành Đen/Navy Tối trong Light Mode
    
    GetColor(0x3B82F6FF), GetColor(0xDBEAFEFF), GetColor(0x38BDF8FF), 
    
    GetColor(0x2563EBFF), // btnGradStart
    GetColor(0x38BDF8FF), // btnGradEnd
    
    GetColor(0x3B82F6FF), GetColor(0xF59E0BFF), GetColor(0x10B981FF), GetColor(0xEF4444FF), 
    GetColor(0x94A3B8FF), GetColor(0xF59E0BFF)
};

#endif