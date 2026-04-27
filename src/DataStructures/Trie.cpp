#include "DataStructures/Trie.h"
#include <ctype.h>
#include <algorithm> // Thư viện <algorithm> để dùng std::min toán học chuẩn

Trie::Trie() {
    root = new TrieNode();
    currentStep = 0;
    timer = 0.0f;
    speedMultiplier = 1.0f;
    isStepByStep = false;
    currentNodeHighlight = nullptr;
    currentSuccessNode = nullptr;
}

// ==========================================
// THUẬT TOÁN ĐỆ QUY TÍNH TỌA ĐỘ VÀ CHỤP ẢNH
// ==========================================
CalculateResult Trie::CalculatePositionsAndCapture(TrieNode* node, float x, float y, float horizontalGap, TrieAnimationStep& step, std::string currentPrefix) {
    if (!node) return {0.0f, 0};

    int parentLeafCount = 0;
    float currentX = x; 

    struct ChildInfo {
        float actualX;
        char childChar;
        TrieNode* nodePtr;
    };
    std::vector<ChildInfo> childrenInfo;

    for (int i = 0; i < 26; i++) {
        if (node->children[i]) {
            char childChar = 'a' + i; 
            CalculateResult childRes = CalculatePositionsAndCapture(node->children[i], currentX, y + 100.0f, horizontalGap, step, currentPrefix + childChar);
            
            childrenInfo.push_back({childRes.actualX, childChar, node->children[i]});
            parentLeafCount += childRes.leafCount;
            currentX += childRes.leafCount * horizontalGap;
        }
    }

    if (childrenInfo.empty()) parentLeafCount = 1;

    float actualX = x;
    if (!childrenInfo.empty()) {
        float minChildX = childrenInfo[0].actualX;
        float maxChildX = childrenInfo[0].actualX;
        for (auto& child : childrenInfo) {
            if (child.actualX < minChildX) minChildX = child.actualX;
            if (child.actualX > maxChildX) maxChildX = child.actualX;
        }
        actualX = (minChildX + maxChildX) / 2.0f;
    }

    for (auto& child : childrenInfo) {
        TrieDrawEdge edge;
        edge.x1 = actualX; edge.y1 = y; 
        edge.x2 = child.actualX; edge.y2 = y + 100.0f; 
        edge.color = (child.nodePtr == currentNodeHighlight) ? GetColor(0xF59E0BFF) : GetColor(0x9CA3AFFF);
        edge.letter = child.childChar;
        step.edges.push_back(edge);
    }

    TrieDrawNode dNode;
    dNode.x = actualX; 
    dNode.y = y;
    dNode.isEndOfWord = node->isEndOfWord;
    
    if (node == root) dNode.text = "Root"; 
    else if (node->isEndOfWord) dNode.text = currentPrefix; 
    else dNode.text = ""; 

    dNode.state = TrieNodeState::NORMAL;
    if (node == currentNodeHighlight) dNode.state = TrieNodeState::HIGHLIGHT;
    if (node == currentSuccessNode) dNode.state = TrieNodeState::SUCCESS;
    
    step.nodes.push_back(dNode);

    return {actualX, parentLeafCount};
}

void Trie::TakeSnapshot(int activeLine, std::vector<std::string> code) {
    TrieAnimationStep step;
    step.activeCodeLine = activeLine;
    step.codeText = code;
    
    // ĐÃ SỬA: Đẩy Root xuống y = 300.0f giống với RBTree
    float startX = 0.0f; 
    float startY = 300.0f; 
    float gap = 120.0f; 

    CalculatePositionsAndCapture(root, startX, startY, gap, step, "");
    
    steps.push_back(step);
}

// ==========================================
// TÍNH NĂNG: INSERT (THÊM TỪ)
// ==========================================
void Trie::Insert(std::string word, bool clearSteps) {
    
    steps.clear();
    currentStep = 0;
    currentNodeHighlight = nullptr;
    currentSuccessNode = nullptr;

    std::vector<std::string> code = {
        "TrieNode* curr = root;",
        "for (char c : word) {",
        "    if (curr->children[c - 'a'] == null)",
        "        curr->children[c - 'a'] = new TrieNode();",
        "    curr = curr->children[c - 'a'];",
        "}",
        "curr->isEndOfWord = true;"
    };

    TrieNode* curr = root;
    currentNodeHighlight = curr;
    TakeSnapshot(0, code);

    for (char c : word) {
        int index = tolower(c) - 'a';
        if (index < 0 || index > 25) continue; 

        TakeSnapshot(1, code);

        if (curr->children[index] == nullptr) {
            TakeSnapshot(2, code);
            curr->children[index] = new TrieNode();
            
            currentNodeHighlight = curr->children[index];
            TakeSnapshot(3, code); 
        } else {
            currentNodeHighlight = curr->children[index];
            TakeSnapshot(4, code); 
        }
        curr = curr->children[index];
    }

    curr->isEndOfWord = true;
    currentSuccessNode = curr;
    currentNodeHighlight = nullptr;
    TakeSnapshot(6, code); 
}

void Trie::InitFromArray(std::vector<std::string> arr) {
    ClearList(); 
    steps.clear(); 
    currentStep = 0; 
    currentNodeHighlight = nullptr; 
    currentSuccessNode = nullptr;
    
    // 1. Chạy vòng lặp chèn toàn bộ các từ vào cây
    // (Lúc này mảng steps sẽ bị nhét đầy các frame animation)
    for (const std::string& w : arr) {
        Insert(w, false); 
    }
    
    // ==================================================
    // 2. ÉP HIỂN THỊ NGUYÊN CÂY (FORCE STATIC VIEW)
    // ==================================================
    steps.clear(); // Xóa sạch toàn bộ animation thừa vừa tạo ra
    currentStep = 0;
    currentOpText = ""; // Xóa chữ "Inserting..."
    currentNodeHighlight = nullptr; 
    currentSuccessNode = nullptr;
    
    // Chụp lại đúng 1 frame duy nhất chứa kết quả cuối cùng của toàn bộ cây
    TakeSnapshot(0, {"Trie Initialized with " + std::to_string(arr.size()) + " words."});
}

// ==========================================
// TÍNH NĂNG: SEARCH (TÌM KIẾM TỪ)
// ==========================================
void Trie::Search(std::string word) {
    steps.clear();
    currentStep = 0;
    currentNodeHighlight = nullptr;
    currentSuccessNode = nullptr;

    std::vector<std::string> code = {
        "TrieNode* curr = root;",
        "for (char c : word) {",
        "    if (curr->children[c - 'a'] == null)",
        "        return false;",
        "    curr = curr->children[c - 'a'];",
        "}",
        "return curr->isEndOfWord;"
    };

    TrieNode* curr = root;
    currentNodeHighlight = curr;
    TakeSnapshot(0, code);

    for (char c : word) {
        int index = tolower(c) - 'a';
        if (index < 0 || index > 25) continue;

        TakeSnapshot(1, code);

        if (curr->children[index] == nullptr) {
            TakeSnapshot(2, code);
            TakeSnapshot(3, code); // return false
            currentNodeHighlight = nullptr;
            return; 
        }
        
        TakeSnapshot(4, code);
        curr = curr->children[index];
        currentNodeHighlight = curr;
    }

    TakeSnapshot(6, code);
    if (curr->isEndOfWord) {
        currentSuccessNode = curr; 
    } 
    currentNodeHighlight = nullptr;
    TakeSnapshot(6, code);
}

// ==========================================
// TÍNH NĂNG: DELETE (HARD DELETE)
// ==========================================
bool Trie::DeleteHelper(TrieNode* curr, std::string word, int depth, std::vector<std::string>& code) {
    if (!curr) return false;

    if (depth == word.length()) {
        if (!curr->isEndOfWord) return false;
        curr->isEndOfWord = false; 
        currentNodeHighlight = curr; 
        TakeSnapshot(6, code);

        for (int i = 0; i < 26; i++) {
            if (curr->children[i]) return false; 
        }
        return true; 
    }

    int index = tolower(word[depth]) - 'a';
    if (index < 0 || index > 25) return false;

    currentNodeHighlight = curr->children[index]; 
    TakeSnapshot(4, code);

    if (DeleteHelper(curr->children[index], word, depth + 1, code)) {
        delete curr->children[index];
        curr->children[index] = nullptr;

        if (curr->isEndOfWord) return false;
        for (int i = 0; i < 26; i++) {
            if (curr->children[i]) return false; 
        }
        if (curr == root) return false; // Không bao giờ xóa Root
        return true; 
    }
    return false;
}

void Trie::Delete(std::string word) {
    steps.clear();
    currentStep = 0;
    currentNodeHighlight = nullptr;
    currentSuccessNode = nullptr;

    std::vector<std::string> code = {
        "Delete(curr, word, depth):",
        "if (!curr) return false;",
        "if (depth == word.len) {",
        "    curr->isEndOfWord = false;",
        "    return hasNoChildren(curr);",
        "}",
        "if (Delete(curr->child, word, depth+1))",
        "    delete curr->child;",
        "return !curr->isEndOfWord && hasNoChildren;"
    };

    currentNodeHighlight = root; 
    TakeSnapshot(0, code);
    
    DeleteHelper(root, word, 0, code);
    
    currentNodeHighlight = nullptr; 
    TakeSnapshot(8, code);
}

void ClearTrieMemory(TrieNode* node) {
    if (!node) return;
    for (int i = 0; i < 26; i++) {
        if (node->children[i]) ClearTrieMemory(node->children[i]);
    }
    delete node;
}

void Trie::ClearList() {
    ClearTrieMemory(root);
    root = new TrieNode();
    steps.clear();
    currentStep = 0;
    timer = 0.0f;
    currentNodeHighlight = nullptr;
    currentSuccessNode = nullptr;
}

void Trie::InitRandom() {
    ClearList();
}

// ==========================================
// ĐIỀU KHIỂN & VẼ
// ==========================================
void Trie::Update() {
    if (steps.empty() || currentStep >= steps.size() - 1) return;
    if (!isStepByStep) {
        timer += GetFrameTime() * speedMultiplier;
        if (timer >= 1.0f) { timer = 0.0f; currentStep++; }
    }
}

void Trie::StepForward() {
    if (!steps.empty() && currentStep < steps.size() - 1) { currentStep++; timer = 0.0f; }
}

void Trie::StepBackward() {
    if (!steps.empty() && currentStep > 0) { currentStep--; timer = 0.0f; }
}

void Trie::Draw(Theme theme, Font uiFont, Font monoFont) {
    if (steps.empty()) return;
    TrieAnimationStep& step = steps[currentStep];

    // ==========================================
    // THUẬT TOÁN AUTO-ZOOM (ĐỒNG BỘ RB TREE)
    // ==========================================
    float minX = 0.0f, maxX = 0.0f, minY = 300.0f, maxY = 300.0f; 
    if (!step.nodes.empty()) {
        minX = step.nodes[0].x;
        maxX = step.nodes[0].x;
        minY = step.nodes[0].y;
        maxY = step.nodes[0].y;
        for (auto& node : step.nodes) {
            if (node.x < minX) minX = node.x;
            if (node.x > maxX) maxX = node.x;
            if (node.y < minY) minY = node.y;
            if (node.y > maxY) maxY = node.y;
        }
    }

    float treeW = (maxX - minX) + 150.0f; 
    float treeH = (maxY - minY) + 150.0f; 
    float availableW = GetScreenWidth() - 450.0f; 
    float availableH = GetScreenHeight() - 350.0f; // Đã sửa thành -350.0f để căn giống RBTree

    float zoomX = availableW / (treeW > 0 ? treeW : 1.0f);
    float zoomY = availableH / (treeH > 0 ? treeH : 1.0f);
    
    float targetZoom = std::min(zoomX, zoomY);
    if (targetZoom > 2.0f) targetZoom = 2.0f; // Giới hạn phóng to như RBTree
    if (targetZoom < 0.3f) targetZoom = 0.3f; // Giới hạn thu nhỏ như RBTree

    Camera2D camera = { 0 };
    camera.target = { (minX + maxX) / 2.0f, 300.0f }; // Căn giữa màn theo root Y = 300
    camera.offset = { availableW / 2.0f + 20.0f, 320.0f }; 
    camera.rotation = 0.0f;
    camera.zoom = targetZoom;

    BeginMode2D(camera);

    for (auto& edge : step.edges) {
        DrawLineEx({edge.x1, edge.y1}, {edge.x2, edge.y2}, 3.0f, edge.color);

        float midX = (edge.x1 + edge.x2) / 2.0f;
        float midY = (edge.y1 + edge.y2) / 2.0f;
        
        char text[2] = {(char)toupper(edge.letter), '\0'};
        
        DrawCircle(midX, midY, 14.0f, theme.panelBg);
        DrawCircleLines(midX, midY, 14.0f, edge.color);
        DrawTextEx(monoFont, text, {midX - 7.0f, midY - 12.0f}, 24, 1.0f, theme.textMain);
    }

    for (auto& node : step.nodes) {
        Color bgColor = theme.panelBg;
        Color borderColor = theme.nodeNormal;

        if (node.state == TrieNodeState::HIGHLIGHT) {
            borderColor = theme.nodeHighlight; 
            bgColor = GetColor(0xF59E0B30); 
        } else if (node.state == TrieNodeState::SUCCESS || node.isEndOfWord) {
            borderColor = theme.nodeSuccess;   
            bgColor = (node.state == TrieNodeState::SUCCESS) ? GetColor(0x10B98160) : GetColor(0x10B98130);
        } else if (node.state == TrieNodeState::ERROR) {
            borderColor = theme.nodeError;
            bgColor = GetColor(0xEF444430);
        }

        if (node.text != "") {
            Vector2 textSize = MeasureTextEx(uiFont, node.text.c_str(), 26, 1.0f);
            float rectW = textSize.x + 30.0f;
            if (rectW < 70.0f) rectW = 70.0f;
            Rectangle rect = {node.x - rectW / 2.0f, node.y - 25.0f, rectW, 50.0f};
            
            DrawRectangleRounded(rect, 0.5f, 10, bgColor);
            DrawRectangleRoundedLines(rect, 0.5f, 10, borderColor);
            DrawTextEx(uiFont, node.text.c_str(), {node.x - textSize.x / 2.0f, node.y - 10.0f}, 20, 1.0f, theme.textMain);
        } else {
            DrawCircle(node.x, node.y, 12.0f, bgColor);
            DrawCircleLines(node.x, node.y, 12.0f, borderColor);
        }
    }

    EndMode2D(); 

    if (!step.codeText.empty()) {
        float panelW = 420.0f;
        float panelH = step.codeText.size() * 35.0f + 70.0f;
        float panelX = GetScreenWidth() - panelW - 30.0f;
        float panelY = 100.0f;

        Rectangle codeRec = {panelX, panelY, panelW, panelH};
        DrawRectangleRounded(codeRec, 0.1f, 10, theme.panelBg);
        DrawRectangleRoundedLines(codeRec, 0.1f, 10, theme.panelBorder);

        static float codeScrollX = 0.0f;
        float maxTextWidth = 0.0f;
        for (const auto& line : step.codeText) {
            float w = MeasureTextEx(monoFont, line.c_str(), 20, 1.0f).x;
            if (w > maxTextWidth) maxTextWidth = w;
        }
        
        float maxScroll = maxTextWidth - panelW + 60.0f; 
        if (maxScroll < 0) maxScroll = 0;

        if (CheckCollisionPointRec(GetMousePosition(), codeRec)) {
            codeScrollX -= GetMouseWheelMove() * 30.0f;
        }

        if (maxScroll > 0) {
            Rectangle leftBtn = {panelX + panelW - 80, panelY + panelH - 35, 30, 25};
            Rectangle rightBtn = {panelX + panelW - 40, panelY + panelH - 35, 30, 25};
            
            if (CheckCollisionPointRec(GetMousePosition(), leftBtn) && IsMouseButtonDown(MOUSE_LEFT_BUTTON)) codeScrollX -= 10.0f;
            if (CheckCollisionPointRec(GetMousePosition(), rightBtn) && IsMouseButtonDown(MOUSE_LEFT_BUTTON)) codeScrollX += 10.0f;
            
            if (codeScrollX < 0) codeScrollX = 0;
            if (codeScrollX > maxScroll) codeScrollX = maxScroll;

            DrawRectangleRounded(leftBtn, 0.2f, 5, theme.btnGradStart);
            DrawTextEx(monoFont, "<", {leftBtn.x + 8, leftBtn.y + 2}, 20, 1.0f, theme.textMain);

            DrawRectangleRounded(rightBtn, 0.2f, 5, theme.btnGradStart);
            DrawTextEx(monoFont, ">", {rightBtn.x + 8, rightBtn.y + 2}, 20, 1.0f, theme.textMain);
            
            float scrollPct = codeScrollX / maxScroll;
            float scrollBarW = panelW - 140; 
            DrawRectangle(panelX + 20, panelY + panelH - 22, scrollBarW, 4, GetColor(0xFFFFFF20));
            DrawRectangle(panelX + 20 + scrollPct * (scrollBarW - 40), panelY + panelH - 24, 40, 8, theme.btnGradEnd);
        } else {
            codeScrollX = 0.0f; 
        }

        BeginScissorMode((int)panelX, (int)panelY, (int)panelW, (int)(panelH - 40));

        for (int i = 0; i < step.codeText.size(); i++) {
            float lineY = panelY + 20.0f + i * 35.0f;
            
            if (i == step.activeCodeLine) {
                DrawRectangle(panelX + 2, lineY - 2, panelW - 4, 35, theme.btnHoverBase);
                DrawRectangle(panelX + 2, lineY - 2, 4, 35, theme.nodeHighlight); 
            }
            
            Color cColor = (i == step.activeCodeLine) ? theme.textMain : theme.textMuted;
            DrawTextEx(monoFont, step.codeText[i].c_str(), {panelX + 20.0f - codeScrollX, lineY + 6.0f}, 20, 1.0f, cColor);
        }

        EndScissorMode();
    }
}