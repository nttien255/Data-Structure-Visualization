#include "DataStructures/RBTree.h"
#include <algorithm>

RBTree::RBTree() {
    TNULL = new RBNode(0);
    TNULL->color = RBColor::RB_BLACK;
    TNULL->left = nullptr;
    TNULL->right = nullptr;
    root = TNULL;

    currentStep = 0; timer = 0.0f; speedMultiplier = 1.0f; isStepByStep = false;
    currentNodeHighlight = nullptr; historyIndex = -1; currentOpText = "";
    SaveState();
}

RBNode* RBTree::CloneTree(RBNode* node, RBNode* parent, RBNode* oldTNULL) {
    if (node == oldTNULL || node == nullptr) return TNULL;
    RBNode* newNode = new RBNode(node->data);
    newNode->color = node->color;
    newNode->parent = parent;
    newNode->left = CloneTree(node->left, newNode, oldTNULL);
    newNode->right = CloneTree(node->right, newNode, oldTNULL);
    return newNode;
}

void RBTree::SaveState() {
    if (historyIndex < (int)history.size() - 1) {
        for (int i = historyIndex + 1; i < history.size(); i++) ClearTreeMemory(history[i]);
        history.erase(history.begin() + historyIndex + 1, history.end());
    }
    history.push_back(CloneTree(root, nullptr, TNULL));
    historyIndex++;
}

void RBTree::Undo() {
    if (historyIndex > 0) {
        historyIndex--; ClearTreeMemory(root);
        root = CloneTree(history[historyIndex], nullptr, TNULL);
        ForceStaticView();
    }
}

void RBTree::Redo() {
    if (historyIndex < (int)history.size() - 1) {
        historyIndex++; ClearTreeMemory(root);
        root = CloneTree(history[historyIndex], nullptr, TNULL);
        ForceStaticView();
    }
}

void RBTree::ClearTreeMemory(RBNode* node) {
    if (node == TNULL || node == nullptr) return;
    ClearTreeMemory(node->left);
    ClearTreeMemory(node->right);
    delete node;
}

void RBTree::ClearTree() {
    ClearTreeMemory(root);
    root = TNULL;
    SaveState(); 
    ForceStaticView();
}

void RBTree::ForceStaticView() {
    steps.clear(); currentStep = 0; currentOpText = "";
    TakeSnapshot(0, {"Tree is idle."});
}

float RBTree::CalculatePositions(RBNode* node, int depth, float& currentX, float horizontalGap, float verticalGap, RBAnimationStep& step) {
    if (node == TNULL || node == nullptr) return -1.0f;
    
    float leftChildX = CalculatePositions(node->left, depth + 1, currentX, horizontalGap, verticalGap, step);
    
    float nodeX = currentX;
    float nodeY = depth * verticalGap + 300.0f; 
    currentX += horizontalGap;
    
    float rightChildX = CalculatePositions(node->right, depth + 1, currentX, horizontalGap, verticalGap, step);

    RBDrawNode dNode = {node->data, nodeX, nodeY, node->color, RBNodeState::NORMAL};
    if (node == currentNodeHighlight) dNode.state = RBNodeState::HIGHLIGHT;
    step.nodes.push_back(dNode);

    if (node->left != TNULL && leftChildX != -1.0f) 
        step.edges.push_back({nodeX, nodeY, leftChildX, nodeY + verticalGap, GetColor(0x9CA3AFFF)});
    if (node->right != TNULL && rightChildX != -1.0f) 
        step.edges.push_back({nodeX, nodeY, rightChildX, nodeY + verticalGap, GetColor(0x9CA3AFFF)});
        
    return nodeX;
}

void RBTree::TakeSnapshot(int activeLine, std::vector<std::string> code) {
    RBAnimationStep step; step.activeCodeLine = activeLine; step.codeText = code;
    step.currentOperation = currentOpText; // TRUYỀN VĂN BẢN VÀO FRAME
    float startX = 0.0f; 
    CalculatePositions(root, 0, startX, 100.0f, 100.0f, step);
    steps.push_back(step);
}

void RBTree::LeftRotate(RBNode* x, std::vector<std::string>& code) {
    RBNode* y = x->right; x->right = y->left;
    if (y->left != TNULL) y->left->parent = x;
    y->parent = x->parent;
    if (x->parent == nullptr) root = y;
    else if (x == x->parent->left) x->parent->left = y;
    else x->parent->right = y;
    y->left = x; x->parent = y;
    TakeSnapshot(7, code);
}

void RBTree::RightRotate(RBNode* x, std::vector<std::string>& code) {
    RBNode* y = x->left; x->left = y->right;
    if (y->right != TNULL) y->right->parent = x;
    y->parent = x->parent;
    if (x->parent == nullptr) root = y;
    else if (x == x->parent->right) x->parent->right = y;
    else x->parent->left = y;
    y->right = x; x->parent = y;
    TakeSnapshot(7, code);
}

void RBTree::InsertFixup(RBNode* k, std::vector<std::string>& code) {
    RBNode* u;
    while (k->parent->color == RBColor::RB_RED) {
        if (k->parent == k->parent->parent->right) {
            u = k->parent->parent->left; 
            if (u->color == RBColor::RB_RED) { 
                u->color = RBColor::RB_BLACK; k->parent->color = RBColor::RB_BLACK;
                k->parent->parent->color = RBColor::RB_RED; k = k->parent->parent;
                currentNodeHighlight = k; TakeSnapshot(4, code);
            } else {
                if (k == k->parent->left) { k = k->parent; RightRotate(k, code); }
                k->parent->color = RBColor::RB_BLACK; k->parent->parent->color = RBColor::RB_RED;
                TakeSnapshot(5, code); LeftRotate(k->parent->parent, code);
            }
        } else {
            u = k->parent->parent->right;
            if (u->color == RBColor::RB_RED) {
                u->color = RBColor::RB_BLACK; k->parent->color = RBColor::RB_BLACK;
                k->parent->parent->color = RBColor::RB_RED; k = k->parent->parent;
                currentNodeHighlight = k; TakeSnapshot(4, code);
            } else {
                if (k == k->parent->right) { k = k->parent; LeftRotate(k, code); }
                k->parent->color = RBColor::RB_BLACK; k->parent->parent->color = RBColor::RB_RED;
                TakeSnapshot(5, code); RightRotate(k->parent->parent, code);
            }
        }
        if (k == root) break;
    }
    root->color = RBColor::RB_BLACK; currentNodeHighlight = nullptr; TakeSnapshot(8, code);
}

void RBTree::Insert(int key, bool clearSteps) {
    if (clearSteps) { steps.clear(); currentStep = 0; }
    currentOpText = "Inserting: " + std::to_string(key);

    std::vector<std::string> code = {
        "Insert(node, key):", "BST Insert to Leaf", "Node color = RED", 
        "while parent is RED:", "  if Uncle is RED: Recoloring", 
        "  else: Recolor & Rotate", "  ...", "Rotate Left / Right", "Root = BLACK"
    };

    RBNode* node = new RBNode(key);
    node->parent = nullptr; node->data = key;
    node->left = TNULL; node->right = TNULL; node->color = RBColor::RB_RED;

    RBNode* y = nullptr; RBNode* x = this->root;
    while (x != TNULL) {
        y = x; currentNodeHighlight = x; TakeSnapshot(1, code);
        if (node->data < x->data) x = x->left; else x = x->right;
    }
    node->parent = y;
    if (y == nullptr) root = node;
    else if (node->data < y->data) y->left = node;
    else y->right = node;

    currentNodeHighlight = node; TakeSnapshot(2, code);

    if (node->parent == nullptr) { 
        node->color = RBColor::RB_BLACK; currentNodeHighlight = nullptr; 
        TakeSnapshot(8, code); if (clearSteps) SaveState(); return; 
    }
    if (node->parent->parent == nullptr) { 
        currentNodeHighlight = nullptr; TakeSnapshot(8, code); if (clearSteps) SaveState(); return; 
    }

    InsertFixup(node, code);
    if (clearSteps) SaveState();
}

void RBTree::InitFromArray(std::vector<int> arr) {
    ClearTreeMemory(root); root = TNULL; history.clear(); historyIndex = -1;
    steps.clear(); currentStep = 0;
    
    for (int val : arr) { Insert(val, false); } // NỐI TIẾP ANIMATION
    
    currentOpText = ""; SaveState();
}

void RBTree::InitRandom(int count) {
    std::vector<int> arr;
    for (int i=0; i<count; i++) arr.push_back(GetRandomValue(10, 99));
    InitFromArray(arr);
}

void RBTree::Update() {
    if (steps.empty() || currentStep >= steps.size() - 1) { currentOpText = ""; return; }
    if (!isStepByStep) {
        timer += GetFrameTime() * speedMultiplier;
        if (timer >= 1.0f) { timer = 0.0f; currentStep++; }
    }
}

void RBTree::StepForward() { if (!steps.empty() && currentStep < steps.size() - 1) { currentStep++; timer = 0.0f; } }
void RBTree::StepBackward() { if (!steps.empty() && currentStep > 0) { currentStep--; timer = 0.0f; } }

void RBTree::Draw(Theme theme, Font uiFont, Font monoFont) {
    if (steps.empty()) ForceStaticView();
    if (steps.empty()) return;

    RBAnimationStep& step = steps[currentStep];

    float minX = 0, maxX = 0, minY = 300.0f, maxY = 300.0f; 
    if (!step.nodes.empty()) {
        minX = step.nodes[0].x; maxX = step.nodes[0].x;
        minY = step.nodes[0].y; maxY = step.nodes[0].y;
        for (auto& n : step.nodes) { 
            if(n.x < minX) minX = n.x; if(n.x > maxX) maxX = n.x; 
            if(n.y < minY) minY = n.y; if(n.y > maxY) maxY = n.y;
        }
    }

    float treeW = (maxX - minX) + 150.0f; float treeH = (maxY - minY) + 150.0f; 
    float availableW = GetScreenWidth() - 450.0f; float availableH = GetScreenHeight() - 350.0f; 

    float zoomX = availableW / (treeW > 0 ? treeW : 1.0f);
    float zoomY = availableH / (treeH > 0 ? treeH : 1.0f);
    
    float targetZoom = std::min(zoomX, zoomY);
    if (targetZoom > 1.5f) targetZoom = 1.5f; 
    if (targetZoom < 0.3f) targetZoom = 0.3f; 

    Camera2D camera = { 0 };
    camera.target = { (minX + maxX) / 2.0f, 300.0f }; 
    camera.offset = { availableW / 2.0f + 20.0f, 320.0f }; 
    camera.rotation = 0.0f; camera.zoom = targetZoom;

    BeginMode2D(camera);
    for (auto& edge : step.edges) DrawLineEx({edge.x1, edge.y1}, {edge.x2, edge.y2}, 3.0f, edge.color);
    
    for (auto& node : step.nodes) {
        Color bgColor = (node.color == RBColor::RB_RED) ? GetColor(0xEF4444FF) : GetColor(0x111827FF);
        Color borderColor = (node.color == RBColor::RB_RED) ? GetColor(0xF87171FF) : theme.panelBorder;

        if (node.state == RBNodeState::HIGHLIGHT) { 
            borderColor = theme.nodeHighlight; DrawCircle(node.x, node.y, 40.0f, GetColor(0xF59E0B60)); 
        }
        
        DrawCircle(node.x, node.y, 32.0f, bgColor); DrawCircleLines(node.x, node.y, 32.0f, borderColor);
        
        const char* valText = TextFormat("%d", node.data);
        Vector2 tSize = MeasureTextEx(monoFont, valText, 26, 1.0f);
        DrawTextEx(monoFont, valText, {node.x - tSize.x/2, node.y - 13}, 26, 1.0f, WHITE);
    }
    EndMode2D();
    
    // UI BOX HIỂN THỊ CURRENT OPERATION
    if (!step.currentOperation.empty()) {
        float boxW = MeasureTextEx(uiFont, step.currentOperation.c_str(), 28, 1.0f).x + 40.0f;
        float boxH = 50.0f;
        float boxX = GetScreenWidth() / 2.0f - boxW / 2.0f;
        float boxY = 190.0f;
        DrawRectangleRounded({boxX, boxY, boxW, boxH}, 0.5f, 10, Fade(theme.btnGradStart, 0.9f));
        DrawRectangleRoundedLines({boxX, boxY, boxW, boxH}, 0.5f, 10, theme.nodeHighlight);
        DrawTextEx(uiFont, step.currentOperation.c_str(), {boxX + 20.0f, boxY + 11.0f}, 28, 1.0f, WHITE);
    }

    if (!step.codeText.empty()) {
        float panelW = 420.0f; float panelH = step.codeText.size() * 35.0f + 50.0f;
        float panelX = GetScreenWidth() - panelW - 30.0f; float panelY = 100.0f;
        DrawRectangleRounded({panelX, panelY, panelW, panelH}, 0.1f, 10, theme.panelBg);
        for (int i = 0; i < step.codeText.size(); i++) {
            float lineY = panelY + 20.0f + i * 35.0f;
            if (i == step.activeCodeLine) { DrawRectangle(panelX + 2, lineY - 2, panelW - 4, 35, theme.btnHoverBase); DrawRectangle(panelX + 2, lineY - 2, 4, 35, theme.nodeHighlight); }
            DrawTextEx(monoFont, step.codeText[i].c_str(), {panelX + 20.0f, lineY + 6.0f}, 20, 1.0f, (i == step.activeCodeLine) ? theme.textMain : theme.textMuted);
        }
    }
}