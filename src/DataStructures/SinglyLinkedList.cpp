#include "DataStructures/SinglyLinkedList.h"
#include <algorithm>

SinglyLinkedList::SinglyLinkedList() {
    currentStep = 0;
    timer = 0.0f;
    completionTimer = 0.0f;
    speedMultiplier = 1.0f;
    isStepByStep = false;
    selectedIndex = -1;
    
    // Khởi tạo trạng thái gốc cho Undo
    history.clear();
    history.push_back(data);
    historyIndex = 0;
}

// LƯU LẠI LỊCH SỬ SAU KHI DỮ LIỆU ĐÃ THAY ĐỔI
void SinglyLinkedList::SaveState() {
    if (historyIndex < (int)history.size() - 1) {
        history.erase(history.begin() + historyIndex + 1, history.end());
    }
    history.push_back(data);
    historyIndex++;
}

void SinglyLinkedList::Undo() {
    if (historyIndex > 0) {
        historyIndex--;
        data = history[historyIndex];
        steps.clear(); currentStep = 0; selectedIndex = -1; completionTimer = 0.0f;
    }
}

void SinglyLinkedList::Redo() {
    if (historyIndex < (int)history.size() - 1) {
        historyIndex++;
        data = history[historyIndex];
        steps.clear(); currentStep = 0; selectedIndex = -1; completionTimer = 0.0f;
    }
}

void SinglyLinkedList::CheckNodeClick() {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mousePos = GetMousePosition();
        bool clicked = false;
        for (int i = 0; i < nodeRects.size(); i++) {
            if (CheckCollisionPointRec(mousePos, nodeRects[i])) {
                selectedIndex = i;
                clicked = true;
                break;
            }
        }
        if (!clicked && mousePos.y > 130 && mousePos.y < GetScreenHeight() - 80) {
            selectedIndex = -1;
        }
    }
}

void SinglyLinkedList::UpdateNode(int newVal) {
    if (selectedIndex < 0 || selectedIndex >= data.size()) return;
    steps.clear(); currentStep = 0; completionTimer = 0.0f;
    
    std::vector<std::string> code = {
        "Node* curr = head;",
        "for (int i = 0; i < index; i++)",
        "    curr = curr->next;",
        "curr->data = newValue;"
    };

    AnimationStep step; step.values = data; step.codeText = code;
    
    for(int i = 0; i <= selectedIndex; i++) {
        step.states.assign(data.size(), NodeState::NORMAL);
        step.states[i] = NodeState::HIGHLIGHT;
        step.activeCodeLine = (i==0) ? 0 : 2;
        steps.push_back(step);
    }

    // Cập nhật dữ liệu và lưu lịch sử
    data[selectedIndex] = newVal;
    SaveState(); 
    
    step.values = data; 
    step.states[selectedIndex] = NodeState::SUCCESS;
    step.activeCodeLine = 3;
    steps.push_back(step);
}

void SinglyLinkedList::InitRandom(int size) {
    data.clear();
    for (int i = 0; i < size; i++) data.push_back(GetRandomValue(10, 99));
    SaveState();
    steps.clear(); currentStep = 0; selectedIndex = -1; completionTimer = 0.0f;
}

void SinglyLinkedList::InitFromArray(std::vector<int> arr) {
    data = arr;
    SaveState();
    steps.clear(); currentStep = 0; selectedIndex = -1; completionTimer = 0.0f;
}

void SinglyLinkedList::ClearList() {
    data.clear();
    SaveState();
    steps.clear(); currentStep = 0; selectedIndex = -1; completionTimer = 0.0f;
}

void SinglyLinkedList::AddTail(int val) {
    steps.clear(); currentStep = 0; selectedIndex = -1; completionTimer = 0.0f;
    
    data.push_back(val);
    SaveState(); 

    std::vector<std::string> code = {
        "Node* newNode = new Node(v);",
        "if (head == null) { head = tail = newNode; }",
        "else { tail->next = newNode; tail = newNode; }"
    };

    AnimationStep step; step.values = data; step.codeText = code;
    step.states.assign(data.size(), NodeState::NORMAL);
    step.states.back() = NodeState::HIGHLIGHT;
    step.activeCodeLine = 0; steps.push_back(step);

    if (data.size() == 1) {
        step.activeCodeLine = 1; step.states.back() = NodeState::SUCCESS; steps.push_back(step);
    } else {
        step.activeCodeLine = 2; step.states.back() = NodeState::SUCCESS; steps.push_back(step);
    }
}

void SinglyLinkedList::Delete(int target) {
    steps.clear(); currentStep = 0; selectedIndex = -1; completionTimer = 0.0f;

    std::vector<std::string> code = {
        "if (head == null) return;",
        "if (head->data == target) { head = head->next; return; }",
        "Node* curr = head;",
        "while (curr->next != null) {",
        "    if (curr->next->data == target) {",
        "        curr->next = curr->next->next; return;",
        "    }",
        "    curr = curr->next;",
        "}"
    };

    AnimationStep step; step.values = data; step.codeText = code;
    step.states.assign(data.size(), NodeState::NORMAL);
    step.activeCodeLine = 0; steps.push_back(step);
    if (data.empty()) return;

    if (data[0] == target) {
        step.activeCodeLine = 1; step.states[0] = NodeState::ERROR; steps.push_back(step);
        
        data.erase(data.begin()); 
        SaveState();
        
        step.values = data; step.states.assign(data.size(), NodeState::NORMAL);
        steps.push_back(step); return;
    }

    step.activeCodeLine = 2; step.states[0] = NodeState::HIGHLIGHT; steps.push_back(step);

    for (size_t i = 0; i < data.size() - 1; i++) {
        step.activeCodeLine = 3; step.states.assign(data.size(), NodeState::NORMAL);
        step.states[i] = NodeState::HIGHLIGHT; step.states[i + 1] = NodeState::HIGHLIGHT; steps.push_back(step);

        if (data[i + 1] == target) {
            step.activeCodeLine = 5; step.states[i + 1] = NodeState::ERROR; steps.push_back(step);
            
            data.erase(data.begin() + i + 1); 
            SaveState();
            
            step.values = data;
            step.states.assign(data.size(), NodeState::NORMAL); step.states[i] = NodeState::SUCCESS;
            steps.push_back(step); return;
        }
    }
}

void SinglyLinkedList::Search(int target) {
    steps.clear(); currentStep = 0; selectedIndex = -1; completionTimer = 0.0f;

    std::vector<std::string> code = {
        "Node* curr = head;",
        "while (curr != nullptr) {",
        "    if (curr->data == target) return curr;",
        "    curr = curr->next;",
        "}",
        "return null;"
    };

    AnimationStep step; step.values = data; step.codeText = code;
    step.states.assign(data.size(), NodeState::NORMAL);
    
    if (data.empty()) { step.activeCodeLine = 5; steps.push_back(step); return; }

    bool found = false;
    for (size_t i = 0; i < data.size(); i++) {
        step.states.assign(data.size(), NodeState::NORMAL); step.states[i] = NodeState::HIGHLIGHT;
        step.activeCodeLine = 2; steps.push_back(step);

        if (data[i] == target) {
            step.states[i] = NodeState::SUCCESS; steps.push_back(step); found = true; break;
        } else {
            step.activeCodeLine = 3; steps.push_back(step);
        }
    }
    if (!found) { step.states.assign(data.size(), NodeState::NORMAL); step.activeCodeLine = 5; steps.push_back(step); }
}

void SinglyLinkedList::Update() {
    CheckNodeClick(); 
    
    if (steps.empty()) {
        completionTimer = 0.0f;
        return;
    }

    // NÚT THẮT 3 GIÂY: Khi chạy xong step cuối cùng
    if (currentStep >= (int)steps.size() - 1) {
        completionTimer += GetFrameTime();
        if (completionTimer >= 3.0f) {
            steps.clear();      // Xóa màu và bảng Pseudocode
            currentStep = 0;
            completionTimer = 0.0f;
        }
        return;
    }

    completionTimer = 0.0f;
    if (!isStepByStep) {
        timer += GetFrameTime() * speedMultiplier;
        if (timer >= 1.0f) { timer = 0.0f; currentStep++; }
    }
}

void SinglyLinkedList::StepForward() { if (!steps.empty() && currentStep < (int)steps.size() - 1) { currentStep++; timer = 0.0f; } }
void SinglyLinkedList::StepBackward() { if (!steps.empty() && currentStep > 0) { currentStep--; timer = 0.0f; } }

void SinglyLinkedList::Draw(Theme theme, Font uiFont, Font monoFont, UILayout layout) {
    std::vector<int> currentData = steps.empty() ? data : steps[currentStep].values;
    std::vector<NodeState> currentStates = steps.empty() ? std::vector<NodeState>(data.size(), NodeState::NORMAL) : steps[currentStep].states;
    
    nodeRects.clear(); 

    if (currentData.empty()) {
        const char* msg = "List is empty. Add elements or initialize.";
        DrawTextEx(uiFont, msg, { layout.screenW/2 - 150, layout.vizY + layout.vizH/2 }, 24, 1.0f, theme.textMuted);
        return;
    }

    float nodeW = 70.0f; float nodeH = 50.0f; float spacing = 110.0f;
    int n = currentData.size();
    float totalWidth = n * spacing;
    
    float startX = (layout.screenW - totalWidth) / 2 + 50.0f; 
    if (startX < 20.0f) startX = 20.0f;
    float startY = layout.vizY + (layout.vizH - nodeH) / 2;

    for (int i = 0; i < n; i++) {
        float x = startX + i * spacing;
        float y = startY;

        Color boxColor = theme.panelBg;
        Color borderColor = theme.nodeNormal;

        if (i == selectedIndex) {
            borderColor = theme.btnGradStart; 
            boxColor = Fade(theme.btnGradStart, 0.2f);
        } else {
            switch (currentStates[i]) {
                case NodeState::HIGHLIGHT: borderColor = theme.nodeHighlight; boxColor = Fade(theme.nodeHighlight, 0.15f); break;
                case NodeState::SUCCESS: borderColor = theme.nodeSuccess; boxColor = Fade(theme.nodeSuccess, 0.15f); break;
                case NodeState::ERROR: borderColor = theme.nodeError; boxColor = Fade(theme.nodeError, 0.15f); break;
                default: break;
            }
        }

        Rectangle nodeRect = {x, y, nodeW, nodeH};
        nodeRects.push_back(nodeRect);

        DrawRectangleRounded(nodeRect, 0.2f, 8, boxColor);
        DrawRectangleRoundedLines(nodeRect, 0.2f, 8, borderColor);
        
        if (i == selectedIndex) DrawRectangleRoundedLines(nodeRect, 0.2f, 8, theme.textHighlighted);

        const char* valText = TextFormat("%d", currentData[i]);
        Vector2 tSize = MeasureTextEx(monoFont, valText, 24, 1.0f);
        DrawTextEx(monoFont, valText, {x + (nodeW - tSize.x) / 2, y + (nodeH - tSize.y) / 2}, 24, 1.0f, theme.textMain);

        if (i < n - 1) {
            float nextX = startX + (i + 1) * spacing;
            DrawLineEx({x + nodeW, y + nodeH/2}, {nextX - 5, y + nodeH/2}, 2.0f, theme.edgeNormal);
            DrawTriangle({nextX, y + nodeH/2}, {nextX - 8, y + nodeH/2 - 5}, {nextX - 8, y + nodeH/2 + 5}, theme.edgeNormal);
        }
    }

    if (!steps.empty() && !steps[currentStep].codeText.empty()) {
        auto& step = steps[currentStep];
        float panelW = 340.0f;
        float lineH = 28.0f;
        float panelH = step.codeText.size() * lineH + 40.0f;
        
        float panelX = layout.screenW - panelW - 20.0f; 
        float panelY = layout.vizY + 20.0f;

        Rectangle codeRect = {panelX, panelY, panelW, panelH};
        DrawRectangleRounded(codeRect, 0.05f, 8, theme.panelBg);
        DrawRectangleRoundedLines(codeRect, 0.05f, 8, theme.panelBorder);
        DrawTextEx(uiFont, "Pseudocode", {panelX + 15, panelY + 10}, 18, 1.0f, theme.textMuted);

        for (size_t i = 0; i < step.codeText.size(); i++) {
            float lineY = panelY + 35.0f + i * lineH;
            if ((int)i == step.activeCodeLine) {
                DrawRectangle(panelX + 5, lineY, panelW - 10, lineH, Fade(theme.nodeHighlight, 0.2f));
                DrawRectangle(panelX + 5, lineY, 3, lineH, theme.nodeHighlight);
            }
            Color textCol = ((int)i == step.activeCodeLine) ? theme.textMain : theme.textMuted;
            DrawTextEx(monoFont, step.codeText[i].c_str(), {panelX + 15, lineY + 4}, 16, 1.0f, textCol);
        }
    }
}