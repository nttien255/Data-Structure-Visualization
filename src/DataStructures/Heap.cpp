#include "DataStructures/Heap.h"
#include <cmath>
#include <algorithm>

Heap::Heap() {
    currentStep = 0; timer = 0.0f; speedMultiplier = 1.0f; isStepByStep = false;
    isMaxHeap = false; historyIndex = -1; currentOpText = "";
    SaveState(); ForceStaticView();
}

bool Heap::Compare(int a, int b) { return isMaxHeap ? (a > b) : (a < b); }

void Heap::SaveState() {
    if (historyIndex < (int)history.size() - 1) history.erase(history.begin() + historyIndex + 1, history.end());
    history.push_back({data, isMaxHeap}); historyIndex++;
}

void Heap::Undo() {
    if (historyIndex > 0) { historyIndex--; data = history[historyIndex].d; isMaxHeap = history[historyIndex].isMax; ForceStaticView(); }
}

void Heap::Redo() {
    if (historyIndex < (int)history.size() - 1) { historyIndex++; data = history[historyIndex].d; isMaxHeap = history[historyIndex].isMax; ForceStaticView(); }
}

void Heap::ClearHeap() { data.clear(); SaveState(); ForceStaticView(); }

void Heap::ForceStaticView() {
    steps.clear(); currentStep = 0; currentOpText = "";
    TakeSnapshot(0, {"Heap is idle."}, std::vector<HeapNodeState>(data.size(), HeapNodeState::NORMAL));
}

void Heap::CalculatePositions(int index, float left, float right, float y, float verticalGap, HeapAnimationStep& step, std::vector<HeapNodeState>& states) {
    if (index >= data.size()) return;
    
    float x = (left + right) / 2.0f;
    step.nodes.push_back({data[index], x, y, states[index]});

    int leftChild = 2 * index + 1;
    int rightChild = 2 * index + 2;

    if (leftChild < data.size()) {
        float childLeft = left; float childRight = x; float childX = (childLeft + childRight) / 2.0f;
        step.edges.push_back({x, y, childX, y + verticalGap, GetColor(0x9CA3AFFF)});
        CalculatePositions(leftChild, childLeft, childRight, y + verticalGap, verticalGap, step, states);
    }
    if (rightChild < data.size()) {
        float childLeft = x; float childRight = right; float childX = (childLeft + childRight) / 2.0f;
        step.edges.push_back({x, y, childX, y + verticalGap, GetColor(0x9CA3AFFF)});
        CalculatePositions(rightChild, childLeft, childRight, y + verticalGap, verticalGap, step, states);
    }
}

void Heap::TakeSnapshot(int activeLine, std::vector<std::string> code, std::vector<HeapNodeState> states) {
    HeapAnimationStep step; step.activeCodeLine = activeLine; step.codeText = code; step.currentOperation = currentOpText;
    
    // Giả lập chiều rộng màn hình rất lớn để cây không bị đè, Camera sẽ tự zoom lại
    float maxW = std::max(1600.0f, (float)pow(2, std::floor(log2(data.size() + 1))) * 120.0f);
    CalculatePositions(0, 0.0f, maxW, 300.0f, 100.0f, step, states);
    
    steps.push_back(step);
}

void Heap::HeapifyUp(int index, std::vector<std::string>& code) {
    std::vector<HeapNodeState> states(data.size(), HeapNodeState::NORMAL);
    while (index > 0) {
        int parent = (index - 1) / 2;
        states[index] = HeapNodeState::HIGHLIGHT; states[parent] = HeapNodeState::HIGHLIGHT;
        TakeSnapshot(3, code, states); // Compare

        if (Compare(data[index], data[parent])) {
            TakeSnapshot(4, code, states); // Need swap
            std::swap(data[index], data[parent]);
            states[index] = HeapNodeState::SUCCESS; states[parent] = HeapNodeState::SUCCESS;
            TakeSnapshot(5, code, states); // Swapped
            states[index] = HeapNodeState::NORMAL; states[parent] = HeapNodeState::NORMAL;
            index = parent;
        } else {
            TakeSnapshot(6, code, states); // Break
            break;
        }
    }
    states.assign(data.size(), HeapNodeState::NORMAL);
    TakeSnapshot(0, code, states);
}

void Heap::Insert(int val, bool clearSteps) {
    if (clearSteps) { steps.clear(); currentStep = 0; }
    currentOpText = "Inserting: " + std::to_string(val);
    
    std::vector<std::string> code = {
        "Insert(val):", "  data.push_back(val)", "  HeapifyUp(data.size - 1)", "    Compare with parent", "    if condition met:", "      Swap(current, parent)", "    else break"
    };

    data.push_back(val);
    std::vector<HeapNodeState> states(data.size(), HeapNodeState::NORMAL);
    states.back() = HeapNodeState::SUCCESS;
    TakeSnapshot(1, code, states);

    HeapifyUp(data.size() - 1, code);
    if (clearSteps) SaveState();
}

void Heap::HeapifyDown(int index, std::vector<std::string>& code) {
    int size = data.size();
    std::vector<HeapNodeState> states(size, HeapNodeState::NORMAL);
    
    while (true) {
        int largest = index;
        int left = 2 * index + 1;
        int right = 2 * index + 2;

        if (left < size) {
            states[largest] = HeapNodeState::HIGHLIGHT; states[left] = HeapNodeState::HIGHLIGHT;
            TakeSnapshot(4, code, states);
            if (Compare(data[left], data[largest])) largest = left;
            states[largest] = HeapNodeState::NORMAL; states[left] = HeapNodeState::NORMAL;
        }

        if (right < size) {
            states[largest] = HeapNodeState::HIGHLIGHT; states[right] = HeapNodeState::HIGHLIGHT;
            TakeSnapshot(5, code, states);
            if (Compare(data[right], data[largest])) largest = right;
            states[largest] = HeapNodeState::NORMAL; states[right] = HeapNodeState::NORMAL;
        }

        if (largest != index) {
            states[index] = HeapNodeState::HIGHLIGHT; states[largest] = HeapNodeState::HIGHLIGHT;
            TakeSnapshot(6, code, states); // Need Swap
            std::swap(data[index], data[largest]);
            states[index] = HeapNodeState::SUCCESS; states[largest] = HeapNodeState::SUCCESS;
            TakeSnapshot(7, code, states); // Swapped
            states[index] = HeapNodeState::NORMAL; states[largest] = HeapNodeState::NORMAL;
            index = largest;
        } else {
            TakeSnapshot(8, code, states); // Break
            break;
        }
    }
    TakeSnapshot(0, code, states);
}

void Heap::ExtractTop() {
    if (data.empty()) return;
    steps.clear(); currentStep = 0;
    currentOpText = "Extracting Top: " + std::to_string(data[0]);

    std::vector<std::string> code = {
        "ExtractTop():", "  if empty return", "  Swap(root, last)", "  data.pop_back()", "  HeapifyDown(root)", "    Compare children", "    if condition met:", "      Swap(current, bestChild)", "    else break"
    };

    std::vector<HeapNodeState> states(data.size(), HeapNodeState::NORMAL);
    states[0] = HeapNodeState::ERROR; // Highlight Root to delete
    TakeSnapshot(0, code, states);

    if (data.size() > 1) {
        states[0] = HeapNodeState::HIGHLIGHT; states.back() = HeapNodeState::HIGHLIGHT;
        TakeSnapshot(2, code, states); // Swap root and last
        std::swap(data[0], data.back());
        TakeSnapshot(2, code, states);
    }
    
    data.pop_back();
    states.assign(data.size(), HeapNodeState::NORMAL);
    TakeSnapshot(3, code, states);

    if (!data.empty()) HeapifyDown(0, code);
    
    currentOpText = ""; // Xóa text khi hoàn thành Extract
    SaveState();
}

void Heap::ToggleHeapType() {
    isMaxHeap = !isMaxHeap;
    steps.clear(); currentStep = 0;
    currentOpText = isMaxHeap ? "Rebuilding as Max Heap" : "Rebuilding as Min Heap";
    
    std::vector<std::string> code = { "BuildHeap(data):", "  for i from size/2 downto 0:", "    HeapifyDown(i)" };
    for (int i = data.size() / 2 - 1; i >= 0; i--) {
        HeapifyDown(i, code);
    }
    currentOpText = ""; SaveState();
}

void Heap::InitFromArray(std::vector<int> arr) {
    ClearHeap(); steps.clear(); currentStep = 0;
    for (int val : arr) Insert(val, false);
    currentOpText = ""; SaveState();
}

void Heap::InitRandom(int count) {
    std::vector<int> arr; for (int i=0; i<count; i++) arr.push_back(GetRandomValue(10, 99));
    InitFromArray(arr); ForceStaticView();
}

void Heap::Update() {
    if (steps.empty() || currentStep >= steps.size() - 1) { currentOpText = ""; return; }
    if (!isStepByStep) { timer += GetFrameTime() * speedMultiplier; if (timer >= 1.0f) { timer = 0.0f; currentStep++; } }
}

void Heap::StepForward() { if (!steps.empty() && currentStep < steps.size() - 1) { currentStep++; timer = 0.0f; } }
void Heap::StepBackward() { if (!steps.empty() && currentStep > 0) { currentStep--; timer = 0.0f; } }

void Heap::Draw(Theme theme, Font uiFont, Font monoFont) {
    if (steps.empty()) ForceStaticView(); if (steps.empty()) return;
    HeapAnimationStep& step = steps[currentStep];

    // AUTO ZOOM THEO KHUNG CHUẨN
    float minX = 0, maxX = 0, minY = 300.0f, maxY = 300.0f; 
    if (!step.nodes.empty()) {
        minX = step.nodes[0].x; maxX = step.nodes[0].x; minY = step.nodes[0].y; maxY = step.nodes[0].y;
        for (auto& n : step.nodes) { 
            if(n.x < minX) minX = n.x; if(n.x > maxX) maxX = n.x; 
            if(n.y < minY) minY = n.y; if(n.y > maxY) maxY = n.y;
        }
    }

    float treeW = (maxX - minX) + 150.0f; float treeH = (maxY - minY) + 150.0f; 
    float availableW = GetScreenWidth() - 450.0f; float availableH = GetScreenHeight() - 350.0f; 
    float zoomX = availableW / (treeW > 0 ? treeW : 1.0f); float zoomY = availableH / (treeH > 0 ? treeH : 1.0f);
    float targetZoom = std::min(zoomX, zoomY); if (targetZoom > 1.5f) targetZoom = 1.5f; if (targetZoom < 0.3f) targetZoom = 0.3f; 

    Camera2D camera = { 0 };
    camera.target = { (minX + maxX) / 2.0f, 300.0f }; 
    camera.offset = { availableW / 2.0f + 20.0f, 320.0f }; 
    camera.rotation = 0.0f; camera.zoom = targetZoom;

    BeginMode2D(camera);
    for (auto& edge : step.edges) DrawLineEx({edge.x1, edge.y1}, {edge.x2, edge.y2}, 3.0f, edge.color);
    
    for (auto& node : step.nodes) {
        Color bgColor = theme.panelBg; Color borderColor = theme.nodeNormal;
        if (node.state == HeapNodeState::HIGHLIGHT) { borderColor = theme.nodeHighlight; bgColor = Fade(theme.nodeHighlight, 0.4f); }
        else if (node.state == HeapNodeState::SUCCESS) { borderColor = theme.nodeSuccess; bgColor = Fade(theme.nodeSuccess, 0.4f); }
        else if (node.state == HeapNodeState::ERROR) { borderColor = theme.nodeError; bgColor = Fade(theme.nodeError, 0.4f); }

        DrawCircle(node.x, node.y, 32.0f, bgColor); DrawCircleLines(node.x, node.y, 32.0f, borderColor);
        const char* valText = TextFormat("%d", node.data);
        Vector2 tSize = MeasureTextEx(monoFont, valText, 26, 1.0f);
        DrawTextEx(monoFont, valText, {node.x - tSize.x/2, node.y - 13}, 26, 1.0f, theme.textMain);
    }
    EndMode2D();
    
    if (!step.currentOperation.empty()) {
        float boxW = MeasureTextEx(uiFont, step.currentOperation.c_str(), 28, 1.0f).x + 40.0f; float boxX = GetScreenWidth() / 2.0f - boxW / 2.0f;
        DrawRectangleRounded({boxX, 190.0f, boxW, 50.0f}, 0.5f, 10, Fade(theme.btnGradStart, 0.9f)); DrawRectangleRoundedLines({boxX, 190.0f, boxW, 50.0f}, 0.5f, 10, theme.nodeHighlight);
        DrawTextEx(uiFont, step.currentOperation.c_str(), {boxX + 20.0f, 190.0f + 11.0f}, 28, 1.0f, WHITE);
    }

    if (!step.codeText.empty()) {
        float panelW = 460.0f; float panelH = step.codeText.size() * 35.0f + 50.0f; float panelX = GetScreenWidth() - panelW - 30.0f; float panelY = 100.0f;
        DrawRectangleRounded({panelX, panelY, panelW, panelH}, 0.1f, 10, theme.panelBg);
        for (int i = 0; i < step.codeText.size(); i++) {
            float lineY = panelY + 20.0f + i * 35.0f;
            if (i == step.activeCodeLine) { DrawRectangle(panelX + 2, lineY - 2, panelW - 4, 35, theme.btnHoverBase); DrawRectangle(panelX + 2, lineY - 2, 4, 35, theme.nodeHighlight); }
            DrawTextEx(monoFont, step.codeText[i].c_str(), {panelX + 20.0f, lineY + 6.0f}, 20, 1.0f, (i == step.activeCodeLine) ? theme.textMain : theme.textMuted);
        }
    }
}