#include "DataStructures/SinglyLinkedList.h"
#include <algorithm>

SinglyLinkedList::SinglyLinkedList() {
    currentStep = 0; timer = 0.0f; completionTimer = 0.0f;
    speedMultiplier = 1.0f; isStepByStep = false; selectedIndex = -1;
    currentOpText = ""; history.clear(); history.push_back(data); historyIndex = 0;
}

void SinglyLinkedList::SaveState() {
    if (historyIndex < (int)history.size() - 1) { history.erase(history.begin() + historyIndex + 1, history.end()); }
    history.push_back(data); historyIndex++;
}

void SinglyLinkedList::Undo() {
    if (historyIndex > 0) { historyIndex--; data = history[historyIndex]; steps.clear(); currentStep = 0; selectedIndex = -1; completionTimer = 0.0f; }
}

void SinglyLinkedList::Redo() {
    if (historyIndex < (int)history.size() - 1) { historyIndex++; data = history[historyIndex]; steps.clear(); currentStep = 0; selectedIndex = -1; completionTimer = 0.0f; }
}

void SinglyLinkedList::CheckNodeClick() {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mousePos = GetMousePosition(); bool clicked = false;
        for (int i = 0; i < nodeRects.size(); i++) {
            if (CheckCollisionPointRec(mousePos, nodeRects[i])) { selectedIndex = i; clicked = true; break; }
        }
        if (!clicked && mousePos.y > 180 && mousePos.y < GetScreenHeight() - 100) { selectedIndex = -1; }
    }
}

void SinglyLinkedList::UpdateNode(int newVal) {
    if (selectedIndex < 0 || selectedIndex >= data.size()) return;
    steps.clear(); currentStep = 0; completionTimer = 0.0f;
    currentOpText = "Updating to: " + std::to_string(newVal);
    
    std::vector<std::string> code = { "Node* curr = head;", "for (int i = 0; i < index; i++)", "    curr = curr->next;", "curr->data = newValue;" };
    AnimationStep step; step.values = data; step.codeText = code; step.currentOperation = currentOpText;
    
    for(int i = 0; i <= selectedIndex; i++) {
        step.states.assign(data.size(), NodeState::NORMAL); step.states[i] = NodeState::HIGHLIGHT;
        step.activeCodeLine = (i==0) ? 0 : 2; steps.push_back(step);
    }
    data[selectedIndex] = newVal; SaveState(); 
    step.values = data; step.states[selectedIndex] = NodeState::SUCCESS;
    step.activeCodeLine = 3; steps.push_back(step);
}

void SinglyLinkedList::InitRandom(int size) {
    ClearList(); std::vector<int> arr;
    for (int i = 0; i < size; i++) arr.push_back(GetRandomValue(10, 99));
    InitFromArray(arr);
}

void SinglyLinkedList::InitFromArray(std::vector<int> arr) {
    ClearList(); steps.clear(); currentStep = 0; selectedIndex = -1; completionTimer = 0.0f;
    for (int val : arr) { AddTail(val, false); } // NỐI TIẾP ANIMATION
    currentOpText = ""; SaveState();
}

void SinglyLinkedList::ClearList() {
    data.clear(); SaveState(); steps.clear(); currentStep = 0; selectedIndex = -1; completionTimer = 0.0f;
}

void SinglyLinkedList::AddTail(int val, bool clearSteps) {
    if (clearSteps) { steps.clear(); currentStep = 0; selectedIndex = -1; completionTimer = 0.0f; }
    currentOpText = "Inserting: " + std::to_string(val);
    
    data.push_back(val);
    if (clearSteps) SaveState(); 

    std::vector<std::string> code = { "Node* newNode = new Node(v);", "if (head == null) { head = tail = newNode; }", "else { tail->next = newNode; tail = newNode; }" };
    AnimationStep step; step.values = data; step.codeText = code; step.currentOperation = currentOpText;
    step.states.assign(data.size(), NodeState::NORMAL); step.states.back() = NodeState::HIGHLIGHT;
    step.activeCodeLine = 0; steps.push_back(step);

    if (data.size() == 1) { step.activeCodeLine = 1; step.states.back() = NodeState::SUCCESS; steps.push_back(step); } 
    else { step.activeCodeLine = 2; step.states.back() = NodeState::SUCCESS; steps.push_back(step); }
}

void SinglyLinkedList::Delete(int target) {
    steps.clear(); currentStep = 0; selectedIndex = -1; completionTimer = 0.0f;
    currentOpText = "Deleting: " + std::to_string(target);

    std::vector<std::string> code = { "if (head == null) return;", "if (head->data == target) { head = head->next; return; }", "Node* curr = head;", "while (curr->next != null) {", "    if (curr->next->data == target) {", "        curr->next = curr->next->next; return;", "    }", "    curr = curr->next;", "}" };
    AnimationStep step; step.values = data; step.codeText = code; step.currentOperation = currentOpText;
    step.states.assign(data.size(), NodeState::NORMAL); step.activeCodeLine = 0; steps.push_back(step);
    if (data.empty()) return;

    if (data[0] == target) {
        step.activeCodeLine = 1; step.states[0] = NodeState::ERROR; steps.push_back(step); data.erase(data.begin()); SaveState();
        step.values = data; step.states.assign(data.size(), NodeState::NORMAL); steps.push_back(step); return;
    }

    step.activeCodeLine = 2; step.states[0] = NodeState::HIGHLIGHT; steps.push_back(step);
    for (size_t i = 0; i < data.size() - 1; i++) {
        step.activeCodeLine = 3; step.states.assign(data.size(), NodeState::NORMAL);
        step.states[i] = NodeState::HIGHLIGHT; step.states[i + 1] = NodeState::HIGHLIGHT; steps.push_back(step);
        if (data[i + 1] == target) {
            step.activeCodeLine = 5; step.states[i + 1] = NodeState::ERROR; steps.push_back(step);
            data.erase(data.begin() + i + 1); SaveState();
            step.values = data; step.states.assign(data.size(), NodeState::NORMAL); step.states[i] = NodeState::SUCCESS;
            steps.push_back(step); return;
        }
    }
}

void SinglyLinkedList::Search(int target) {
    steps.clear(); currentStep = 0; selectedIndex = -1; completionTimer = 0.0f;
    currentOpText = "Searching: " + std::to_string(target);

    std::vector<std::string> code = { "Node* curr = head;", "while (curr != nullptr) {", "    if (curr->data == target) return curr;", "    curr = curr->next;", "}", "return null;" };
    AnimationStep step; step.values = data; step.codeText = code; step.currentOperation = currentOpText;
    step.states.assign(data.size(), NodeState::NORMAL);
    
    if (data.empty()) { step.activeCodeLine = 5; steps.push_back(step); return; }

    bool found = false;
    for (size_t i = 0; i < data.size(); i++) {
        step.states.assign(data.size(), NodeState::NORMAL); step.states[i] = NodeState::HIGHLIGHT;
        step.activeCodeLine = 2; steps.push_back(step);
        if (data[i] == target) { step.states[i] = NodeState::SUCCESS; steps.push_back(step); found = true; break; } 
        else { step.activeCodeLine = 3; steps.push_back(step); }
    }
    if (!found) { step.states.assign(data.size(), NodeState::NORMAL); step.activeCodeLine = 5; steps.push_back(step); }
}

void SinglyLinkedList::Update() {
    CheckNodeClick(); 
    if (steps.empty()) { completionTimer = 0.0f; currentOpText = ""; return; }
    if (currentStep >= (int)steps.size() - 1) {
        completionTimer += GetFrameTime();
        if (completionTimer >= 3.0f) { steps.clear(); currentStep = 0; completionTimer = 0.0f; currentOpText = ""; }
        return;
    }
    completionTimer = 0.0f;
    if (!isStepByStep) { timer += GetFrameTime() * speedMultiplier; if (timer >= 1.0f) { timer = 0.0f; currentStep++; } }
}
void SinglyLinkedList::StepForward() { if (!steps.empty() && currentStep < (int)steps.size() - 1) { currentStep++; timer = 0.0f; } }
void SinglyLinkedList::StepBackward() { if (!steps.empty() && currentStep > 0) { currentStep--; timer = 0.0f; } }

void SinglyLinkedList::Draw(Theme theme, Font uiFont, Font monoFont, UILayout layout) {
    std::vector<int> currentData = steps.empty() ? data : steps[currentStep].values;
    std::vector<NodeState> currentStates = steps.empty() ? std::vector<NodeState>(data.size(), NodeState::NORMAL) : steps[currentStep].states;
    nodeRects.clear(); 

    if (currentData.empty()) {
        DrawTextEx(uiFont, "List is empty. Add elements or initialize.", { layout.screenW/2 - 200, layout.screenH/2 }, 24, 1.0f, theme.textMuted);
    } else {
        float baseNodeW = 130.0f; float baseNodeH = 80.0f; float baseSpacing = 150.0f; int n = currentData.size();
        float totalBaseWidth = n * baseSpacing; float availableWidth = layout.screenW - 100.0f; 
        float scale = 1.0f; if (totalBaseWidth > availableWidth) scale = availableWidth / totalBaseWidth;
        float nodeW = baseNodeW * scale; float nodeH = baseNodeH * scale; float spacing = baseSpacing * scale; float totalWidth = n * spacing;
        float startX = (layout.screenW - totalWidth) / 2 + (spacing - nodeW) / 2; float startY = layout.screenH / 2 - nodeH / 2;

        for (int i = 0; i < n; i++) {
            float x = startX + i * spacing; float y = startY;
            Color boxColor = theme.panelBg; Color borderColor = theme.nodeNormal;
            if (i == selectedIndex) { borderColor = theme.btnGradStart; boxColor = Fade(theme.btnGradStart, 0.2f); } 
            else {
                switch (currentStates[i]) {
                    case NodeState::HIGHLIGHT: borderColor = theme.nodeHighlight; boxColor = Fade(theme.nodeHighlight, 0.15f); break;
                    case NodeState::SUCCESS: borderColor = theme.nodeSuccess; boxColor = Fade(theme.nodeSuccess, 0.15f); break;
                    case NodeState::ERROR: borderColor = theme.nodeError; boxColor = Fade(theme.nodeError, 0.15f); break;
                    default: break;
                }
            }
            Rectangle nodeRect = {x, y, nodeW, nodeH}; nodeRects.push_back(nodeRect);
            DrawRectangleRounded(nodeRect, 0.2f, 8, boxColor); DrawRectangleRoundedLines(nodeRect, 0.2f, 8, borderColor);
            if (i == selectedIndex) DrawRectangleRoundedLines(nodeRect, 0.2f, 8, theme.textHighlighted);

            const char* valText = TextFormat("%d", currentData[i]); float fontSize = 28 * scale; if (fontSize < 10) fontSize = 10; 
            Vector2 tSize = MeasureTextEx(monoFont, valText, fontSize, 1.0f);
            DrawTextEx(monoFont, valText, {x + (nodeW - tSize.x) / 2, y + (nodeH - tSize.y) / 2}, fontSize, 1.0f, theme.textMain);

            if (i < n - 1) {
                float nextX = startX + (i + 1) * spacing; float arrowY = y + nodeH/2;
                DrawLineEx({x + nodeW, arrowY}, {nextX - 5 * scale, arrowY}, 2.0f * scale, theme.edgeNormal);
                DrawTriangle({nextX, arrowY}, {nextX - 8 * scale, arrowY - 5 * scale}, {nextX - 8 * scale, arrowY + 5 * scale}, theme.edgeNormal);
            }
        }
    }

    // UI BOX HIỂN THỊ CURRENT OPERATION
    if (!steps.empty() && !steps[currentStep].currentOperation.empty()) {
        float boxW = MeasureTextEx(uiFont, steps[currentStep].currentOperation.c_str(), 28, 1.0f).x + 40.0f;
        float boxH = 50.0f; float boxX = layout.screenW / 2.0f - boxW / 2.0f; float boxY = 190.0f;
        DrawRectangleRounded({boxX, boxY, boxW, boxH}, 0.5f, 10, Fade(theme.btnGradStart, 0.9f));
        DrawRectangleRoundedLines({boxX, boxY, boxW, boxH}, 0.5f, 10, theme.nodeHighlight);
        DrawTextEx(uiFont, steps[currentStep].currentOperation.c_str(), {boxX + 20.0f, boxY + 11.0f}, 28, 1.0f, WHITE);
    }

    if (!steps.empty() && !steps[currentStep].codeText.empty()) {
        auto& step = steps[currentStep]; float panelW = 460.0f; float panelH = step.codeText.size() * 35.0f + 70.0f; 
        float panelX = layout.screenW - panelW - 30.0f; float panelY = 30.0f;
        Rectangle codeRec = {panelX, panelY, panelW, panelH};
        DrawRectangleRounded(codeRec, 0.1f, 10, theme.panelBg); DrawRectangleRoundedLines(codeRec, 0.1f, 10, theme.panelBorder);

        static float listCodeScrollX = 0.0f; float maxTextWidth = 0.0f;
        for (const auto& line : step.codeText) { float w = MeasureTextEx(monoFont, line.c_str(), 20, 1.0f).x; if (w > maxTextWidth) maxTextWidth = w; }
        float maxScroll = maxTextWidth - panelW + 60.0f; if (maxScroll < 0) maxScroll = 0;

        if (CheckCollisionPointRec(GetMousePosition(), codeRec)) { listCodeScrollX -= GetMouseWheelMove() * 30.0f; }
        if (maxScroll > 0) {
            Rectangle leftBtn = {panelX + panelW - 80, panelY + panelH - 35, 30, 25}; Rectangle rightBtn = {panelX + panelW - 40, panelY + panelH - 35, 30, 25};
            if (CheckCollisionPointRec(GetMousePosition(), leftBtn) && IsMouseButtonDown(MOUSE_LEFT_BUTTON)) listCodeScrollX -= 10.0f;
            if (CheckCollisionPointRec(GetMousePosition(), rightBtn) && IsMouseButtonDown(MOUSE_LEFT_BUTTON)) listCodeScrollX += 10.0f;
            if (listCodeScrollX < 0) listCodeScrollX = 0; if (listCodeScrollX > maxScroll) listCodeScrollX = maxScroll;
            DrawRectangleRounded(leftBtn, 0.2f, 5, theme.btnGradStart); DrawTextEx(monoFont, "<", {leftBtn.x + 8, leftBtn.y + 2}, 20, 1.0f, theme.textMain);
            DrawRectangleRounded(rightBtn, 0.2f, 5, theme.btnGradStart); DrawTextEx(monoFont, ">", {rightBtn.x + 8, rightBtn.y + 2}, 20, 1.0f, theme.textMain);
            float scrollPct = listCodeScrollX / maxScroll; float scrollBarW = panelW - 140; 
            DrawRectangle(panelX + 20, panelY + panelH - 22, scrollBarW, 4, GetColor(0xFFFFFF20)); DrawRectangle(panelX + 20 + scrollPct * (scrollBarW - 40), panelY + panelH - 24, 40, 8, theme.btnGradEnd);
        } else { listCodeScrollX = 0.0f; }

        BeginScissorMode((int)panelX, (int)panelY, (int)panelW, (int)(panelH - 40));
        for (int i = 0; i < step.codeText.size(); i++) {
            float lineY = panelY + 20.0f + i * 35.0f;
            if (i == step.activeCodeLine) { DrawRectangle(panelX + 2, lineY - 2, panelW - 4, 35, theme.btnHoverBase); DrawRectangle(panelX + 2, lineY - 2, 4, 35, theme.nodeHighlight); }
            Color cColor = (i == step.activeCodeLine) ? theme.textMain : theme.textMuted;
            DrawTextEx(monoFont, step.codeText[i].c_str(), {panelX + 20.0f - listCodeScrollX, lineY + 6.0f}, 20, 1.0f, cColor);
        }
        EndScissorMode();
    }
}