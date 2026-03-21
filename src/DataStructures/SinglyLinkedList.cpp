#include "DataStructures/SinglyLinkedList.h"
#include <algorithm>

SinglyLinkedList::SinglyLinkedList() {
    currentStep = 0;
    timer = 0.0f;
    speedMultiplier = 1.0f;
    isStepByStep = false;
}

void SinglyLinkedList::InitRandom(int size) {
    data.clear();
    steps.clear();
    for (int i = 0; i < size; i++) data.push_back(GetRandomValue(10, 99));
    
    AnimationStep step;
    step.values = data;
    step.states.assign(data.size(), NodeState::NORMAL);
    step.activeCodeLine = -1; 
    steps.push_back(step);
    currentStep = 0;
}

void SinglyLinkedList::InitFromArray(std::vector<int> arr) {
    data = arr;
    steps.clear();
    AnimationStep step;
    step.values = data;
    step.states.assign(data.size(), NodeState::NORMAL);
    step.activeCodeLine = -1; 
    steps.push_back(step);
    currentStep = 0;
}

void SinglyLinkedList::ClearList() {
    data.clear();
    steps.clear();
}

// ==========================================
// TÍNH NĂNG: ADD TAIL (ĐÃ FIX LỖI ẢO GIÁC)
// ==========================================
void SinglyLinkedList::AddTail(int val) {
    steps.clear();
    currentStep = 0;

    data.push_back(val); // Cập nhật thẳng vào bộ nhớ để hiện hình ngay lập tức

    std::vector<std::string> code = {
        "Node* vtx = new Node(v);",
        "if (head == null) {",
        "    head = tail = vtx;",
        "} else {",
        "    tail->next = vtx;",
        "    tail = vtx;",
        "}"
    };

    AnimationStep step;
    step.values = data; // Chứa luôn Node mới
    step.states.assign(data.size(), NodeState::NORMAL);
    step.states.back() = NodeState::HIGHLIGHT; // Node mới hiện màu Cam ngay từ Step 0
    step.codeText = code;

    step.activeCodeLine = 0; 
    steps.push_back(step);

    if (data.size() == 1) {
        step.activeCodeLine = 2;
        step.states.back() = NodeState::SUCCESS; // Nối xong đổi màu Xanh Lá
        steps.push_back(step);
    } else {
        step.activeCodeLine = 4;
        steps.push_back(step);

        step.activeCodeLine = 5;
        step.states.back() = NodeState::SUCCESS;
        steps.push_back(step);
    }
}

// ==========================================
// TÍNH NĂNG: XÓA NODE (DELETE)
// ==========================================
void SinglyLinkedList::Delete(int target) {
    steps.clear();
    currentStep = 0;

    std::vector<std::string> code = {
        "if (head == null) return;",
        "if (head->data == v) {",
        "    head = head->next;",
        "    return;",
        "}",
        "Node* curr = head;",
        "while (curr->next != null) {",
        "    if (curr->next->data == v) {",
        "        curr->next = curr->next->next;",
        "        return;",
        "    }",
        "    curr = curr->next;",
        "}"
    };

    AnimationStep step;
    step.values = data;
    step.states.assign(data.size(), NodeState::NORMAL);
    step.codeText = code;

    step.activeCodeLine = 0;
    steps.push_back(step);
    if (data.empty()) return;

    step.activeCodeLine = 1;
    step.states[0] = NodeState::HIGHLIGHT;
    steps.push_back(step);

    // Trường hợp xóa phần tử đầu tiên
    if (data[0] == target) {
        step.activeCodeLine = 2;
        step.states[0] = NodeState::ERROR; // Đổi màu Đỏ báo hiệu chuẩn bị xóa
        steps.push_back(step);

        data.erase(data.begin()); // Xóa thật khỏi bộ nhớ
        step.values = data;
        step.states.assign(data.size(), NodeState::NORMAL);
        step.activeCodeLine = 3; 
        steps.push_back(step);
        return;
    }

    step.states[0] = NodeState::NORMAL;
    step.activeCodeLine = 5; 
    step.states[0] = NodeState::HIGHLIGHT; // Trỏ curr
    steps.push_back(step);

    // Duyệt tìm phần tử
    for (int i = 0; i < data.size() - 1; i++) {
        step.activeCodeLine = 6;
        step.states.assign(data.size(), NodeState::NORMAL);
        step.states[i] = NodeState::HIGHLIGHT; // curr
        steps.push_back(step);

        step.activeCodeLine = 7;
        step.states[i+1] = NodeState::HIGHLIGHT; // curr->next
        steps.push_back(step);

        if (data[i+1] == target) {
            step.activeCodeLine = 8;
            step.states[i+1] = NodeState::ERROR; // Node bị xóa hiện màu Đỏ
            steps.push_back(step);

            data.erase(data.begin() + i + 1);
            step.values = data;
            step.states.assign(data.size(), NodeState::NORMAL);
            step.states[i] = NodeState::SUCCESS; // Nối thành công
            step.activeCodeLine = 9; 
            steps.push_back(step);
            return;
        } else {
            step.activeCodeLine = 11; 
            steps.push_back(step);
        }
    }
}

// ==========================================
// TÍNH NĂNG: TÌM KIẾM
// ==========================================
void SinglyLinkedList::Search(int target) {
    steps.clear();
    currentStep = 0;
    
    std::vector<std::string> code = {
        "Node* curr = head;",
        "while (curr != nullptr) {",
        "    if (curr->data == target)",
        "        return curr;",
        "    curr = curr->next;",
        "}",
        "return null;"
    };

    AnimationStep step;
    step.values = data;
    step.states.assign(data.size(), NodeState::NORMAL);
    step.codeText = code;

    if (data.empty()) {
        step.activeCodeLine = 0; steps.push_back(step);
        step.activeCodeLine = 6; steps.push_back(step);
        return;
    }

    bool found = false;
    for (int i = 0; i < data.size(); i++) {
        step.states.assign(data.size(), NodeState::NORMAL);
        step.states[i] = NodeState::HIGHLIGHT;
        
        step.activeCodeLine = 2; 
        steps.push_back(step);

        if (data[i] == target) {
            step.states[i] = NodeState::SUCCESS;
            step.activeCodeLine = 3; 
            steps.push_back(step);
            found = true;
            break;
        } else {
            step.activeCodeLine = 4; 
            steps.push_back(step);
        }
    }

    if (!found) {
        step.states.assign(data.size(), NodeState::NORMAL);
        step.activeCodeLine = 6; 
        steps.push_back(step);
    }
}

// ==========================================
// ĐIỀU KHIỂN & VẼ
// ==========================================
void SinglyLinkedList::Update() {
    if (steps.empty() || currentStep >= steps.size() - 1) return;

    if (!isStepByStep) {
        timer += GetFrameTime() * speedMultiplier;
        if (timer >= 1.0f) {
            timer = 0.0f;
            currentStep++;
        }
    }
}

void SinglyLinkedList::StepForward() {
    if (!steps.empty() && currentStep < steps.size() - 1) { currentStep++; timer = 0.0f; }
}

void SinglyLinkedList::StepBackward() {
    if (!steps.empty() && currentStep > 0) { currentStep--; timer = 0.0f; }
}

void SinglyLinkedList::Draw(Theme theme, Font uiFont, Font monoFont) {
    if (steps.empty()) return;
    AnimationStep& step = steps[currentStep];

    float nodeW = 80.0f, nodeH = 60.0f, spacing = 120.0f;
    int n = step.values.size();
    float totalWidth = n * spacing;
    float maxWidth = GetScreenWidth() - 450.0f; 
    float scale = (totalWidth > maxWidth && totalWidth > 0) ? (maxWidth / totalWidth) : 1.0f;

    float startX = 50.0f;
    float startY = GetScreenHeight() / 2.0f - (nodeH * scale) / 2.0f;

    for (int i = 0; i < n; i++) {
        float x = startX + i * (spacing * scale);
        float y = startY;
        
        Color boxColor = theme.panelBg;
        Color borderColor = theme.nodeNormal;

        if (step.states[i] == NodeState::HIGHLIGHT) {
            borderColor = theme.nodeHighlight; 
            boxColor = GetColor(0xF59E0B30); 
        } else if (step.states[i] == NodeState::SUCCESS) {
            borderColor = theme.nodeSuccess;   
            boxColor = GetColor(0x10B98130);
        } else if (step.states[i] == NodeState::ERROR) {
            borderColor = theme.nodeError; // Nút sắp bị xóa sẽ có viền Đỏ
            boxColor = GetColor(0xEF444430);
        }

        Rectangle nodeRec = {x, y, nodeW * scale, nodeH * scale};
        DrawRectangleRounded(nodeRec, 0.2f, 10, boxColor);
        DrawRectangleRoundedLines(nodeRec, 0.2f, 10, borderColor);

        const char* valText = TextFormat("%d", step.values[i]);
        Vector2 tSize = MeasureTextEx(monoFont, valText, 28 * scale, 1.0f);
        DrawTextEx(monoFont, valText, {x + (nodeW*scale - tSize.x)/2, y + (nodeH*scale - tSize.y)/2}, 28 * scale, 1.0f, theme.textMain);

        if (i < n - 1) {
            float nextX = startX + (i + 1) * (spacing * scale);
            DrawLineEx({x + nodeW * scale, y + (nodeH*scale)/2}, {nextX, y + (nodeH*scale)/2}, 3.0f * scale, theme.edgeNormal);
            DrawCircle(nextX, y + (nodeH*scale)/2, 4.0f * scale, theme.edgeNormal);
        }
    }

    if (!step.codeText.empty()) {
        float panelW = 350.0f;
        float panelH = step.codeText.size() * 35.0f + 40.0f;
        float panelX = GetScreenWidth() - panelW - 30.0f;
        float panelY = GetScreenHeight() / 2.0f - panelH / 2.0f;

        Rectangle codeRec = {panelX, panelY, panelW, panelH};
        DrawRectangleRounded(codeRec, 0.1f, 10, theme.panelBg);
        DrawRectangleRoundedLines(codeRec, 0.1f, 10, theme.panelBorder);

        for (int i = 0; i < step.codeText.size(); i++) {
            float lineY = panelY + 20.0f + i * 35.0f;
            if (i == step.activeCodeLine) {
                DrawRectangle(panelX + 2, lineY - 2, panelW - 4, 35, theme.btnHoverBase);
                DrawRectangle(panelX + 2, lineY - 2, 4, 35, theme.nodeHighlight); 
            }
            Color cColor = (i == step.activeCodeLine) ? theme.textMain : theme.textMuted;
            DrawTextEx(monoFont, step.codeText[i].c_str(), {panelX + 20.0f, lineY + 5.0f}, 22, 1.0f, cColor);
        }
    }
}