// // #ifndef THEME_H
// // #define THEME_H

// // #include "raylib.h"

// // struct Theme {
// //     Color background;
// //     Color grid;
// //     Color panelBg;
// //     Color panelBorder;
// //     Color textMain;
// //     Color textMuted;
// //     Color textHighlighted;

// //     Color btnNormal;
// //     Color btnHoverBase;
// //     Color btnHoverGlow;
// //     Color btnGradStart;
// //     Color btnGradEnd;
// //     Color btnDanger;      // For delete/clear actions
// //     Color btnSuccess;     // For add/confirm actions

// //     Color nodeNormal;
// //     Color nodeHighlight;
// //     Color nodeSuccess;
// //     Color nodeError;
// //     Color edgeNormal;
// //     Color edgeHighlight;
// // };

// // const Theme DarkTheme = {
// //     GetColor(0x0F172AFF),  // background
// //     GetColor(0x1E293B88),  // grid
// //     GetColor(0x1E293BFF),  // panelBg
// //     GetColor(0x334155FF),  // panelBorder
// //     GetColor(0xF8FAFCFF),  // textMain
// //     GetColor(0x94A3B8FF),  // textMuted
// //     WHITE,                  // textHighlighted

// //     GetColor(0x2563EBFF),  // btnNormal
// //     GetColor(0x1E293BFF),  // btnHoverBase
// //     GetColor(0x38BDF8FF),  // btnHoverGlow
// //     GetColor(0x1E3A8AFF),  // btnGradStart
// //     GetColor(0x0EA5E9FF),  // btnGradEnd
// //     GetColor(0xDC2626FF),  // btnDanger
// //     GetColor(0x059669FF),  // btnSuccess

// //     GetColor(0x3B82F6FF),  // nodeNormal
// //     GetColor(0xF59E0BFF),  // nodeHighlight
// //     GetColor(0x10B981FF),  // nodeSuccess
// //     GetColor(0xEF4444FF),  // nodeError
// //     GetColor(0x475569FF),  // edgeNormal
// //     GetColor(0xF59E0BFF)   // edgeHighlight
// // };

// // const Theme LightTheme = {
// //     GetColor(0xF8FAFCFF),  // background
// //     GetColor(0xE2E8F088),  // grid
// //     GetColor(0xFFFFFFFF),  // panelBg
// //     GetColor(0xCBD5E1FF),  // panelBorder
// //     GetColor(0x0F172AFF),  // textMain
// //     GetColor(0x64748BFF),  // textMuted
// //     GetColor(0x0F172AFF),  // textHighlighted

// //     GetColor(0x3B82F6FF),  // btnNormal
// //     GetColor(0xDBEAFEFF),  // btnHoverBase
// //     GetColor(0x38BDF8FF),  // btnHoverGlow
// //     GetColor(0x2563EBFF),  // btnGradStart
// //     GetColor(0x38BDF8FF),  // btnGradEnd
// //     GetColor(0xDC2626FF),  // btnDanger
// //     GetColor(0x059669FF),  // btnSuccess

// //     GetColor(0x3B82F6FF),  // nodeNormal
// //     GetColor(0xF59E0BFF),  // nodeHighlight
// //     GetColor(0x10B981FF),  // nodeSuccess
// //     GetColor(0xEF4444FF),  // nodeError
// //     GetColor(0x94A3B8FF),  // edgeNormal
// //     GetColor(0xF59E0BFF)   // edgeHighlight
// // };

// // #endif

// #ifndef THEME_H
// #define THEME_H

// #include "raylib.h"

// struct Theme {
//     Color background;
//     Color grid;
//     Color panelBg;
//     Color panelBorder;
//     Color textMain;
//     Color textMuted;
//     Color textHighlighted;

//     Color btnNormal;
//     Color btnHoverBase;
//     Color btnHoverGlow;
//     Color btnGradStart;
//     Color btnGradEnd;
//     Color btnDanger;      // For delete/clear actions
//     Color btnSuccess;     // For add/confirm actions

//     Color nodeNormal;
//     Color nodeHighlight;
//     Color nodeSuccess;
//     Color nodeError;
//     Color edgeNormal;
//     Color edgeHighlight;
// };

// // ==========================================
// // BỘ MÀU HỒNG TỐI (DARK PINK THEME)
// // ==========================================
// const Theme DarkTheme = {
//     GetColor(0x2C1022FF),  // background (Hồng tím cực đậm)
//     GetColor(0x83184388),  // grid (Hồng mận có trong suốt)
//     GetColor(0x500724FF),  // panelBg (Hồng mận)
//     GetColor(0x831843FF),  // panelBorder (Viền hồng đậm)
//     GetColor(0xFCE7F3FF),  // textMain (Trắng pha chút hồng)
//     GetColor(0xF9A8D4FF),  // textMuted (Hồng nhạt)
//     WHITE,                 // textHighlighted

//     GetColor(0xDB2777FF),  // btnNormal (Hồng cánh sen đậm)
//     GetColor(0x831843FF),  // btnHoverBase
//     GetColor(0xF9A8D4FF),  // btnHoverGlow
//     GetColor(0x9D174DFF),  // btnGradStart (Gradient hồng đậm)
//     GetColor(0xEC4899FF),  // btnGradEnd (Gradient hồng sáng)
//     GetColor(0xDC2626FF),  // btnDanger (Giữ màu Đỏ)
//     GetColor(0x059669FF),  // btnSuccess (Giữ màu Xanh)

//     GetColor(0xEC4899FF),  // nodeNormal (Node màu hồng sáng)
//     GetColor(0xF59E0BFF),  // nodeHighlight (Màu vàng cam để nổi bật)
//     GetColor(0x10B981FF),  // nodeSuccess
//     GetColor(0xEF4444FF),  // nodeError
//     GetColor(0xBE185DFF),  // edgeNormal (Màu cạnh hồng đậm)
//     GetColor(0xF59E0BFF)   // edgeHighlight
// };

// // ==========================================
// // BỘ MÀU HỒNG SÁNG (LIGHT PINK THEME)
// // ==========================================
// const Theme LightTheme = {
//     GetColor(0xFDF2F8FF),  // background (Hồng sữa siêu nhạt)
//     GetColor(0xFBCFE888),  // grid
//     GetColor(0xFFFFFFFF),  // panelBg (Trắng tinh)
//     GetColor(0xFBCFE8FF),  // panelBorder (Viền hồng pastel)
//     GetColor(0x500724FF),  // textMain (Chữ hồng siêu đậm)
//     GetColor(0x9D174DFF),  // textMuted (Chữ hồng xám)
//     GetColor(0x831843FF),  // textHighlighted

//     GetColor(0xEC4899FF),  // btnNormal
//     GetColor(0xFCE7F3FF),  // btnHoverBase
//     GetColor(0xF9A8D4FF),  // btnHoverGlow
//     GetColor(0xDB2777FF),  // btnGradStart (Gradient hồng cánh sen)
//     GetColor(0xF9A8D4FF),  // btnGradEnd (Gradient hồng pastel)
//     GetColor(0xDC2626FF),  // btnDanger
//     GetColor(0x059669FF),  // btnSuccess

//     GetColor(0xDB2777FF),  // nodeNormal (Node màu hồng cánh sen)
//     GetColor(0xF59E0BFF),  // nodeHighlight (Màu vàng cam)
//     GetColor(0x10B981FF),  // nodeSuccess
//     GetColor(0xEF4444FF),  // nodeError
//     GetColor(0xF9A8D4FF),  // edgeNormal (Cạnh màu hồng pastel)
//     GetColor(0xF59E0BFF)   // edgeHighlight
// };

// #endif

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

// ==========================================
// DARK MODE: BỘ MÀU SLATE / NAVY (CŨ)
// ==========================================
const Theme DarkTheme = {
    GetColor(0x0F172AFF),  // background
    GetColor(0x1E293B88),  // grid
    GetColor(0x1E293BFF),  // panelBg
    GetColor(0x334155FF),  // panelBorder
    GetColor(0xF8FAFCFF),  // textMain
    GetColor(0x94A3B8FF),  // textMuted
    WHITE,                 // textHighlighted

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

// ==========================================
// LIGHT MODE: BỘ MÀU PINK PASTEL (MỚI)
// ==========================================
const Theme LightTheme = {
    GetColor(0xFDF2F8FF),  // background (Hồng sữa siêu nhạt)
    GetColor(0xFBCFE888),  // grid
    GetColor(0xFFFFFFFF),  // panelBg (Trắng tinh)
    GetColor(0xFBCFE8FF),  // panelBorder (Viền hồng pastel)
    GetColor(0x500724FF),  // textMain (Chữ hồng siêu đậm)
    GetColor(0x9D174DFF),  // textMuted (Chữ hồng xám)
    GetColor(0x831843FF),  // textHighlighted

    GetColor(0xEC4899FF),  // btnNormal
    GetColor(0xFCE7F3FF),  // btnHoverBase
    GetColor(0xF9A8D4FF),  // btnHoverGlow
    GetColor(0xDB2777FF),  // btnGradStart (Gradient hồng cánh sen)
    GetColor(0xF9A8D4FF),  // btnGradEnd (Gradient hồng pastel)
    GetColor(0xDC2626FF),  // btnDanger
    GetColor(0x059669FF),  // btnSuccess

    GetColor(0xDB2777FF),  // nodeNormal (Node màu hồng cánh sen)
    GetColor(0xF59E0BFF),  // nodeHighlight (Màu vàng cam)
    GetColor(0x10B981FF),  // nodeSuccess
    GetColor(0xEF4444FF),  // nodeError
    GetColor(0xF9A8D4FF),  // edgeNormal (Cạnh màu hồng pastel)
    GetColor(0xF59E0BFF)   // edgeHighlight
};

#endif