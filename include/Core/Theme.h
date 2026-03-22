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
    Color textHighlighted;

    Color btnNormal;
    Color btnHoverBase;
    Color btnHoverGlow;
    Color btnGradStart;
    Color btnGradEnd;
    Color btnDanger;      // For delete/clear actions
    Color btnSuccess;     // For add/confirm actions

    Color nodeNormal;
    Color nodeHighlight;
    Color nodeSuccess;
    Color nodeError;
    Color edgeNormal;
    Color edgeHighlight;
};

const Theme DarkTheme = {
    GetColor(0x0F172AFF),  // background
    GetColor(0x1E293B88),  // grid
    GetColor(0x1E293BFF),  // panelBg
    GetColor(0x334155FF),  // panelBorder
    GetColor(0xF8FAFCFF),  // textMain
    GetColor(0x94A3B8FF),  // textMuted
    WHITE,                  // textHighlighted

    GetColor(0x2563EBFF),  // btnNormal
    GetColor(0x1E293BFF),  // btnHoverBase
    GetColor(0x38BDF8FF),  // btnHoverGlow
    GetColor(0x1E3A8AFF),  // btnGradStart
    GetColor(0x0EA5E9FF),  // btnGradEnd
    GetColor(0xDC2626FF),  // btnDanger
    GetColor(0x059669FF),  // btnSuccess

    GetColor(0x3B82F6FF),  // nodeNormal
    GetColor(0xF59E0BFF),  // nodeHighlight
    GetColor(0x10B981FF),  // nodeSuccess
    GetColor(0xEF4444FF),  // nodeError
    GetColor(0x475569FF),  // edgeNormal
    GetColor(0xF59E0BFF)   // edgeHighlight
};

const Theme LightTheme = {
    GetColor(0xF8FAFCFF),  // background
    GetColor(0xE2E8F088),  // grid
    GetColor(0xFFFFFFFF),  // panelBg
    GetColor(0xCBD5E1FF),  // panelBorder
    GetColor(0x0F172AFF),  // textMain
    GetColor(0x64748BFF),  // textMuted
    GetColor(0x0F172AFF),  // textHighlighted

    GetColor(0x3B82F6FF),  // btnNormal
    GetColor(0xDBEAFEFF),  // btnHoverBase
    GetColor(0x38BDF8FF),  // btnHoverGlow
    GetColor(0x2563EBFF),  // btnGradStart
    GetColor(0x38BDF8FF),  // btnGradEnd
    GetColor(0xDC2626FF),  // btnDanger
    GetColor(0x059669FF),  // btnSuccess

    GetColor(0x3B82F6FF),  // nodeNormal
    GetColor(0xF59E0BFF),  // nodeHighlight
    GetColor(0x10B981FF),  // nodeSuccess
    GetColor(0xEF4444FF),  // nodeError
    GetColor(0x94A3B8FF),  // edgeNormal
    GetColor(0xF59E0BFF)   // edgeHighlight
};

#endif
