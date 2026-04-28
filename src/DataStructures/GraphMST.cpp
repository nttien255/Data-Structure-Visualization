#include "DataStructures/GraphMST.h"
#include <cmath>
#include <algorithm>

GraphMST::GraphMST() { currentStep = 0; timer = 0.0f; speedMultiplier = 1.0f; isStepByStep = false; selectedIndex = -1; historyIndex = -1; SaveState(); ForceStaticView(); }
void GraphMST::SaveState() { if (historyIndex < (int)history.size() - 1) history.erase(history.begin() + historyIndex + 1, history.end()); history.push_back({nodes, edges, nextNodeId}); historyIndex++; }
void GraphMST::Undo() { if (historyIndex > 0) { historyIndex--; nodes = history[historyIndex].n; edges = history[historyIndex].e; nextNodeId = history[historyIndex].nextId; selectedIndex = -1; ForceStaticView(); } }
void GraphMST::Redo() { if (historyIndex < (int)history.size() - 1) { historyIndex++; nodes = history[historyIndex].n; edges = history[historyIndex].e; nextNodeId = history[historyIndex].nextId; selectedIndex = -1; ForceStaticView(); } }
void GraphMST::ClearGraph() { nodes.clear(); edges.clear(); nextNodeId = 0; selectedIndex = -1; SaveState(); ForceStaticView(); }
void GraphMST::ForceStaticView() { steps.clear(); currentStep = 0; currentOpText = ""; TakeSnapshot(0, {"Graph is idle."}, std::vector<MSTNodeState>(nodes.size(), MSTNodeState::NORMAL), std::vector<MSTEdgeState>(edges.size(), MSTEdgeState::NORMAL), {}, -1); }

void GraphMST::CalculatePositions() {
    float cx = GetScreenWidth() / 2.0f + 100.0f; float cy = GetScreenHeight() / 2.0f + 80.0f; float r = std::max(100.0f, 280.0f - nodes.size()*5.0f);
    for (int i = 0; i < nodes.size(); i++) { float angle = i * (2 * PI / nodes.size()) - PI / 2; nodes[i].x = cx + r * cos(angle); nodes[i].y = cy + r * sin(angle); }
}

void GraphMST::TakeSnapshot(int activeLine, std::vector<std::string> code, std::vector<MSTNodeState> nStates, std::vector<MSTEdgeState> eStates, std::vector<MSTEdgeInfo> sEdges, int curEIdx) {
    CalculatePositions(); MSTAnimationStep step; step.activeCodeLine = activeLine; step.codeText = code; step.currentOperation = currentOpText; step.sortedEdges = sEdges; step.currentEdgeIdx = curEIdx;
    for (int i=0; i<nodes.size(); i++) step.nodes.push_back({nodes[i].value, nodes[i].x, nodes[i].y, nStates.empty() ? MSTNodeState::NORMAL : nStates[i]});
    for (int i=0; i<edges.size(); i++) {
        int uIdx = -1, vIdx = -1; for (int j=0; j<nodes.size(); j++) { if(nodes[j].id == edges[i].u_id) uIdx = j; if(nodes[j].id == edges[i].v_id) vIdx = j; }
        if (uIdx != -1 && vIdx != -1) step.edges.push_back({nodes[uIdx].x, nodes[uIdx].y, nodes[vIdx].x, nodes[vIdx].y, edges[i].weight, eStates.empty() ? MSTEdgeState::NORMAL : eStates[i]});
    }
    steps.push_back(step);
}

int GraphMST::FindNodeByValue(int val) { for (int i = 0; i < nodes.size(); i++) if (nodes[i].value == val) return i; return -1; }

void GraphMST::AddVertex(int val, bool clearSteps) {
    if (FindNodeByValue(val) != -1) return; if (clearSteps) { steps.clear(); currentStep = 0; selectedIndex = -1; }
    currentOpText = "Adding Vertex: " + std::to_string(val); nodes.push_back({nextNodeId++, val, 0, 0});
    if (clearSteps) SaveState(); TakeSnapshot(0, {"Add Vertex(v)"}, {}, {}, {}, -1);
}

void GraphMST::AddEdge(int u_val, int v_val, int w, bool clearSteps) {
    int uIdx = FindNodeByValue(u_val); if (uIdx == -1) { AddVertex(u_val, false); uIdx = nodes.size() - 1; }
    int vIdx = FindNodeByValue(v_val); if (vIdx == -1) { AddVertex(v_val, false); vIdx = nodes.size() - 1; }
    for (auto& e : edges) if ((e.u_id == nodes[uIdx].id && e.v_id == nodes[vIdx].id) || (e.u_id == nodes[vIdx].id && e.v_id == nodes[uIdx].id)) return;
    if (clearSteps) { steps.clear(); currentStep = 0; selectedIndex = -1; }
    currentOpText = "Adding Edge: (" + std::to_string(u_val) + ", " + std::to_string(v_val) + ") W:" + std::to_string(w);
    edges.push_back({nodes[uIdx].id, nodes[vIdx].id, w}); if (clearSteps) SaveState(); TakeSnapshot(0, {"Add Edge(u, v, w)"}, {}, {}, {}, -1);
}

void GraphMST::DeleteVertex(int val) {
    int idx = FindNodeByValue(val); if (idx == -1) return;
    steps.clear(); currentStep = 0; selectedIndex = -1; currentOpText = "Deleting Vertex: " + std::to_string(val);
    int id = nodes[idx].id; edges.erase(std::remove_if(edges.begin(), edges.end(), [id](const MSTEdge& e){ return e.u_id == id || e.v_id == id; }), edges.end()); nodes.erase(nodes.begin() + idx);
    SaveState(); ForceStaticView();
}

void GraphMST::UpdateVertex(int oldVal, int newVal) {
    int idx = FindNodeByValue(oldVal); if (idx == -1 || FindNodeByValue(newVal) != -1) return;
    steps.clear(); currentStep = 0; currentOpText = "Updating Vertex: " + std::to_string(oldVal) + " -> " + std::to_string(newVal); nodes[idx].value = newVal; SaveState(); ForceStaticView();
}

void GraphMST::InitFromEdges(const std::vector<InitEdge>& edgeList, const std::vector<int>& isolatedVertices) {
    ClearGraph();
    for (int v : isolatedVertices) if (FindNodeByValue(v) == -1) nodes.push_back({nextNodeId++, v, 0, 0});
    for (auto& e : edgeList) {
        int uIdx = FindNodeByValue(e.u); if (uIdx == -1) { nodes.push_back({nextNodeId++, e.u, 0, 0}); uIdx = nodes.size() - 1; }
        int vIdx = FindNodeByValue(e.v); if (vIdx == -1) { nodes.push_back({nextNodeId++, e.v, 0, 0}); vIdx = nodes.size() - 1; }
        bool exists = false; for (auto& ex : edges) if ((ex.u_id == nodes[uIdx].id && ex.v_id == nodes[vIdx].id) || (ex.u_id == nodes[vIdx].id && ex.v_id == nodes[uIdx].id)) { exists = true; break; }
        if (!exists) edges.push_back({nodes[uIdx].id, nodes[vIdx].id, e.w});
    }
    currentOpText = ""; SaveState(); ForceStaticView();
}

int GraphMST::DSUFind(int i, std::vector<int>& parent, std::vector<std::string>& code, std::vector<MSTNodeState>& nState, std::vector<MSTEdgeState>& eState, std::vector<MSTEdgeInfo>& sEdges, int curEIdx) {
    nState[i] = MSTNodeState::FIND; TakeSnapshot(3, code, nState, eState, sEdges, curEIdx);
    if (parent[i] == i) { nState[i] = MSTNodeState::NORMAL; return i; }
    int root = DSUFind(parent[i], parent, code, nState, eState, sEdges, curEIdx);
    parent[i] = root; nState[i] = MSTNodeState::NORMAL; return root;
}

void GraphMST::RunKruskal() {
    if (nodes.empty() || edges.empty()) return;
    steps.clear(); currentStep = 0; currentOpText = "Running Kruskal's MST Algorithm";
    std::vector<std::string> code = { "Kruskal():", "  Sort edges by weight", "  for edge(u, v) in edges:", "    rootU = find(u)", "    rootV = find(v)", "    if rootU != rootV:", "      union(u, v)", "      MST.add(edge)" };
    std::vector<MSTNodeState> nState(nodes.size(), MSTNodeState::NORMAL); std::vector<MSTEdgeState> eState(edges.size(), MSTEdgeState::NORMAL);
    std::vector<std::pair<int, MSTEdge>> sorted; for (int i=0; i<edges.size(); i++) sorted.push_back({i, edges[i]});
    std::sort(sorted.begin(), sorted.end(), [](const auto& a, const auto& b){ return a.second.weight < b.second.weight; });
    
    std::vector<MSTEdgeInfo> sEdgesUI;
    for (auto& s : sorted) {
        int uIdx=-1, vIdx=-1; for(int j=0;j<nodes.size();j++){ if(nodes[j].id == s.second.u_id) uIdx=j; if(nodes[j].id == s.second.v_id) vIdx=j; }
        sEdgesUI.push_back({nodes[uIdx].value, nodes[vIdx].value, s.second.weight, false});
    }
    TakeSnapshot(1, code, nState, eState, sEdgesUI, -1);
    
    std::vector<int> parent(nodes.size()); for (int i=0; i<nodes.size(); i++) parent[i] = i;
    for (int idx = 0; idx < sorted.size(); idx++) {
        int origIdx = sorted[idx].first; MSTEdge edge = sorted[idx].second;
        int uIdx = -1, vIdx = -1; for(int j=0; j<nodes.size(); j++){ if(nodes[j].id == edge.u_id) uIdx=j; if(nodes[j].id == edge.v_id) vIdx=j; }
        eState[origIdx] = MSTEdgeState::HIGHLIGHT; TakeSnapshot(2, code, nState, eState, sEdgesUI, idx);
        
        int rootU = DSUFind(uIdx, parent, code, nState, eState, sEdgesUI, idx);
        int rootV = DSUFind(vIdx, parent, code, nState, eState, sEdgesUI, idx);
        
        TakeSnapshot(5, code, nState, eState, sEdgesUI, idx);
        if (rootU != rootV) {
            parent[rootU] = rootV; eState[origIdx] = MSTEdgeState::SUCCESS; sEdgesUI[idx].isSelected = true; TakeSnapshot(7, code, nState, eState, sEdgesUI, idx);
        } else { eState[origIdx] = MSTEdgeState::NORMAL; TakeSnapshot(5, code, nState, eState, sEdgesUI, idx); }
    }
    
    // ĐÃ SỬA: Tắt chữ Current Operation
    currentOpText = ""; 
    TakeSnapshot(0, code, nState, eState, sEdgesUI, -1);
}

void GraphMST::InitRandom(int count) {
    ClearGraph();
    // ĐÃ FIX CRASH: Tạo các đỉnh mang giá trị tuần tự từ 1 đến count
    for (int i = 1; i <= count; i++) AddVertex(i, false);
    // Đảm bảo đồ thị liên thông
    for (int i = 1; i < count; i++) AddEdge(nodes[i-1].value, nodes[i].value, GetRandomValue(1, 20), false);
    // Thêm cạnh ngẫu nhiên
    for (int i = 0; i < count; i++) { 
        int u = GetRandomValue(0, count-1); 
        int v = GetRandomValue(0, count-1); 
        if (u != v) AddEdge(nodes[u].value, nodes[v].value, GetRandomValue(1, 20), false); 
    }
    currentOpText = ""; SaveState(); ForceStaticView();
}

void GraphMST::CheckNodeClick() {
    if (steps.empty() && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        CalculatePositions(); Vector2 mousePos = GetMousePosition(); bool clicked = false;
    float r = std::max(30.0f, 60.0f - nodes.size() * 1.5f);
        for (int i = 0; i < nodes.size(); i++) { if (CheckCollisionPointCircle(mousePos, {nodes[i].x, nodes[i].y}, r)) { selectedIndex = i; clicked = true; break; } }
        if (!clicked && mousePos.y > 180 && mousePos.y < GetScreenHeight() - 100) selectedIndex = -1;
    }
}

void GraphMST::Update() { CheckNodeClick(); if (steps.empty() || currentStep >= steps.size() - 1) { currentOpText = ""; return; } if (!isStepByStep) { timer += GetFrameTime() * speedMultiplier; if (timer >= 1.0f) { timer = 0.0f; currentStep++; } } }
void GraphMST::StepForward() { if (!steps.empty() && currentStep < steps.size() - 1) { currentStep++; timer = 0.0f; } }
void GraphMST::StepBackward() { if (!steps.empty() && currentStep > 0) { currentStep--; timer = 0.0f; } }

void GraphMST::Draw(Theme theme, Font uiFont, Font monoFont) {
    if (steps.empty()) ForceStaticView(); if (steps.empty()) return;
    MSTAnimationStep& step = steps[currentStep];
    float r = std::max(30.0f, 60.0f - step.nodes.size() * 1.5f);

    for (auto& edge : step.edges) {
        Color ec = theme.edgeNormal; if (edge.state == MSTEdgeState::HIGHLIGHT) ec = theme.nodeHighlight; else if (edge.state == MSTEdgeState::SUCCESS) ec = theme.nodeError; 
        DrawLineEx({edge.x1, edge.y1}, {edge.x2, edge.y2}, edge.state == MSTEdgeState::SUCCESS ? 6.0f : 4.0f, ec);
        float mx = (edge.x1 + edge.x2)/2; float my = (edge.y1 + edge.y2)/2;
        DrawCircle(mx, my, 14, theme.panelBg); DrawTextEx(monoFont, TextFormat("%d", edge.weight), {mx-8, my-10}, 20, 1.0f, theme.textMain);
    }
    
    for (int i=0; i<step.nodes.size(); i++) {
        auto& node = step.nodes[i]; Color bg = theme.panelBg; Color border = theme.nodeNormal;
        if (i == selectedIndex && steps.size() <= 1) { border = theme.btnGradStart; bg = Fade(theme.btnGradStart, 0.2f); }
        else if (node.state == MSTNodeState::HIGHLIGHT) { border = theme.nodeHighlight; bg = Fade(theme.nodeHighlight, 0.4f); }
        else if (node.state == MSTNodeState::FIND) { border = YELLOW; bg = Fade(YELLOW, 0.6f); }
        DrawCircle(node.x, node.y, r, bg); DrawCircleLines(node.x, node.y, r, border);
        if (i == selectedIndex && steps.size() <= 1) DrawCircleLines(node.x, node.y, r+2.0f, theme.textHighlighted);
        
        const char* text = TextFormat("%d", node.value); float fs = r * 0.8f; Vector2 ts = MeasureTextEx(monoFont, text, fs, 1.0f);
        DrawTextEx(monoFont, text, {node.x - ts.x/2, node.y - ts.y/2}, fs, 1.0f, theme.textMain);
    }

    if (!step.currentOperation.empty()) {
        float boxW = MeasureTextEx(uiFont, step.currentOperation.c_str(), 28, 1.0f).x + 40.0f; float boxX = GetScreenWidth() / 2.0f - boxW / 2.0f;
        DrawRectangleRounded({boxX, 190.0f, boxW, 50.0f}, 0.5f, 10, Fade(theme.btnGradStart, 0.9f)); DrawRectangleRoundedLines({boxX, 190.0f, boxW, 50.0f}, 0.5f, 10, theme.nodeHighlight);
        DrawTextEx(uiFont, step.currentOperation.c_str(), {boxX + 20.0f, 190.0f + 11.0f}, 28, 1.0f, WHITE);
    }

    // ĐÃ FIX: Hạ thấp bảng, thêm nút Close [X]
    if (!step.sortedEdges.empty()) {
        float pX = 30.0f; 
        float pY = 260.0f; // ĐÃ HẠ THẤP KHỎI INPUT
        float panelW = 280.0f;
        float itemH = 35.0f;
        float maxVisible = 5.0f;
        float panelH = 50.0f + maxVisible * itemH;

        Rectangle panelRec = {pX, pY, panelW, panelH};
        DrawRectangleRounded(panelRec, 0.1f, 8, theme.panelBg); 
        DrawRectangleRoundedLines(panelRec, 0.1f, 8, theme.nodeHighlight);
        DrawTextEx(uiFont, "Sorted Edges", {pX+15, pY+10}, 24, 1.0f, theme.textMain);

        // Nút tắt [X]
        Rectangle closeBtn = {pX + panelW - 35, pY + 8, 24, 24};
        bool isHovered = CheckCollisionPointRec(GetMousePosition(), closeBtn);
        DrawRectangleRounded(closeBtn, 0.2f, 4, isHovered ? GetColor(0xEF4444FF) : theme.panelBorder);
        DrawTextEx(monoFont, "X", {closeBtn.x + 6, closeBtn.y + 2}, 20, 1.0f, WHITE);
        
        if (isHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            step.sortedEdges.clear(); // Tắt bảng
        }
        
        static float scrollY = 0.0f;
        float maxScroll = std::max(0.0f, step.sortedEdges.size() * itemH - maxVisible * itemH);

        // AUTO SCROLL bám theo cạnh
        if (step.currentEdgeIdx != -1 && !CheckCollisionPointRec(GetMousePosition(), panelRec)) {
            float targetScroll = step.currentEdgeIdx * itemH - (maxVisible / 2.0f) * itemH;
            scrollY = targetScroll; 
        }

        if (CheckCollisionPointRec(GetMousePosition(), panelRec)) scrollY -= GetMouseWheelMove() * 20.0f;
        if (scrollY < 0) scrollY = 0; if (scrollY > maxScroll) scrollY = maxScroll;

        if (maxScroll > 0) {
            float scrollPct = scrollY / maxScroll;
            DrawRectangle(pX + panelW - 10, pY + 50, 6, panelH - 60, GetColor(0xFFFFFF20));
            DrawRectangle(pX + panelW - 10, pY + 50 + scrollPct * (panelH - 60 - 20), 6, 20, theme.btnGradEnd);
        }

        BeginScissorMode(pX, pY + 45, panelW, panelH - 50);
        for (int i = 0; i < step.sortedEdges.size(); i++) {
            float itemY = pY + 50.0f + i * itemH - scrollY;
            Color tColor = theme.textMuted;
            if (i == step.currentEdgeIdx) { DrawRectangle(pX+5, itemY - 2, panelW - 20, itemH, theme.btnHoverBase); tColor = theme.textMain; }
            if (step.sortedEdges[i].isSelected) tColor = theme.nodeError; 
            
            DrawTextEx(monoFont, TextFormat("(%d, %d) w: %d", step.sortedEdges[i].u_val, step.sortedEdges[i].v_val, step.sortedEdges[i].w), {pX+20, itemY + 5}, 20, 1.0f, tColor); 
        }
        EndScissorMode();
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