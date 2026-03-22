#ifndef UI_LAYOUT_H
#define UI_LAYOUT_H

#include "raylib.h"

struct UILayout {
    float screenW;
    float screenH;
    
    // Giao diện thiết kế theo 2 thanh ngang: Top Bar và Bottom Bar
    float topBarH;
    float bottomBarH;
    
    float vizX;
    float vizY;
    float vizW;
    float vizH;

    float padding; // Thêm lại biến padding
    
    void Update() {
        screenW = (float)GetScreenWidth();
        screenH = (float)GetScreenHeight();
        
        topBarH = 130.0f;    // Thanh công cụ phía trên (Chứa thao tác)
        bottomBarH = 80.0f;  // Thanh công cụ phía dưới (Chứa Playback & Undo)
        
        padding = 20.0f;     // Khởi tạo khoảng cách căn lề chuẩn
        
        // Khu vực vẽ Visualization nằm ở giữa
        vizX = 0;
        vizY = topBarH;
        vizW = screenW;
        vizH = screenH - topBarH - bottomBarH; 
    }
};

#endif