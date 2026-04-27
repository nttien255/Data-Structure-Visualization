#include "DataStructures/GraphBFS.h"
#include <cmath>
#include <queue>
#include <algorithm>

GraphBFS::GraphBFS() { currentStep = 0; timer = 0.0f; speedMultiplier = 1.0f; isStepByStep = false; selectedIndex = -1; historyIndex = -1; SaveState(); ForceStaticView(); }
void GraphBFS::SaveState() { if (historyIndex < (int)history.size() - 1) history.erase(history.begin() + historyIndex + 1, history.end()); history.push_back({nodes, edges, nextNodeId}); historyIndex++; }
void GraphBFS::Undo() { if (historyIndex > 0) { historyIndex--; nodes = history[historyIndex].n; edges = history[historyIndex].e; nextNodeId = history[historyIndex].nextId; selectedIndex = -1; ForceStaticView(); } }
void GraphBFS::Redo() { if (historyIndex < (int)history.size() - 1) { historyIndex++; nodes = history[historyIndex].n; edges = history[historyIndex].e; nextNodeId = history[historyIndex].nextId; selectedIndex = -1; ForceStaticView(); } }
void GraphBFS::ClearGraph() { nodes.clear(); edges.clear(); nextNodeId = 0; selectedIndex = -1; SaveState(); ForceStaticView(); }
void GraphBFS::ForceStaticView() { steps.clear(); currentStep = 0; currentOpText = ""; TakeSnapshot(0, {"Graph is idle."}, std::vector<GraphNodeState>(nodes.size(), GraphNodeState::NORMAL), std::vector<GraphEdgeState>(edges.size(), GraphEdgeState::NORMAL), {}, -1); }

void GraphBFS::CalculatePositions() {
    float cx = GetScreenWidth() / 2.0f + 100.0f; 
    float cy = GetScreenHeight() / 2.0f + 80.0f; 
    float r = std::max(100.0f, 280.0f - nodes.size()*5.0f);
    for (int i = 0; i < nodes.size(); i++) { float angle = i * (2 * PI / nodes.size()) - PI / 2; nodes[i].x = cx + r * cos(angle); nodes[i].y = cy + r * sin(angle); }
}

void GraphBFS::TakeSnapshot(int activeLine, std::vector<std::string> code, std::vector<GraphNodeState> nStates, std::vector<GraphEdgeState> eStates, std::map<int, int> dist, int sourceVal) {
    CalculatePositions(); BFSAnimationStep step; step.activeCodeLine = activeLine; step.codeText = code; step.currentOperation = currentOpText; step.distances = dist; step.sourceValue = sourceVal;
    for (int i=0; i<nodes.size(); i++) step.nodes.push_back({nodes[i].value, nodes[i].x, nodes[i].y, nStates.empty() ? GraphNodeState::NORMAL : nStates[i]});
    for (int i=0; i<edges.size(); i++) {
        int uIdx = -1, vIdx = -1; for (int j=0; j<nodes.size(); j++) { if(nodes[j].id == edges[i].u_id) uIdx = j; if(nodes[j].id == edges[i].v_id) vIdx = j; }
        if (uIdx != -1 && vIdx != -1) step.edges.push_back({nodes[uIdx].x, nodes[uIdx].y, nodes[vIdx].x, nodes[vIdx].y, eStates.empty() ? GraphEdgeState::NORMAL : eStates[i]});
    }
    steps.push_back(step);
}

int GraphBFS::FindNodeByValue(int val) { for (int i = 0; i < nodes.size(); i++) if (nodes[i].value == val) return i; return -1; }

void GraphBFS::AddVertex(int val, bool clearSteps) {
    if (FindNodeByValue(val) != -1) return; if (clearSteps) { steps.clear(); currentStep = 0; selectedIndex = -1; }
    currentOpText = "Adding Vertex: " + std::to_string(val); nodes.push_back({nextNodeId++, val, 0, 0});
    if (clearSteps) SaveState(); std::vector<GraphNodeState> ns(nodes.size(), GraphNodeState::NORMAL); ns.back() = GraphNodeState::SUCCESS; TakeSnapshot(0, {"Add Vertex(v)"}, ns, {}, {}, -1);
}

void GraphBFS::AddEdge(int u_val, int v_val, bool clearSteps) {
    int uIdx = FindNodeByValue(u_val); if (uIdx == -1) { AddVertex(u_val, false); uIdx = nodes.size() - 1; }
    int vIdx = FindNodeByValue(v_val); if (vIdx == -1) { AddVertex(v_val, false); vIdx = nodes.size() - 1; }
    for (auto& e : edges) if ((e.u_id == nodes[uIdx].id && e.v_id == nodes[vIdx].id) || (e.u_id == nodes[vIdx].id && e.v_id == nodes[uIdx].id)) return;
    if (clearSteps) { steps.clear(); currentStep = 0; selectedIndex = -1; }
    currentOpText = "Adding Edge: (" + std::to_string(u_val) + ", " + std::to_string(v_val) + ")";
    edges.push_back({nodes[uIdx].id, nodes[vIdx].id}); if (clearSteps) SaveState();
    std::vector<GraphEdgeState> es(edges.size(), GraphEdgeState::NORMAL); es.back() = GraphEdgeState::SUCCESS; TakeSnapshot(0, {"Add Edge(u, v)"}, std::vector<GraphNodeState>(nodes.size(), GraphNodeState::NORMAL), es, {}, -1);
}

void GraphBFS::DeleteVertex(int val) {
    int idx = FindNodeByValue(val); if (idx == -1) return;
    steps.clear(); currentStep = 0; selectedIndex = -1; currentOpText = "Deleting Vertex: " + std::to_string(val);
    int id = nodes[idx].id; edges.erase(std::remove_if(edges.begin(), edges.end(), [id](const BFSEdge& e){ return e.u_id == id || e.v_id == id; }), edges.end()); nodes.erase(nodes.begin() + idx);
    SaveState(); ForceStaticView();
}

void GraphBFS::UpdateVertex(int oldVal, int newVal) {
    int idx = FindNodeByValue(oldVal); if (idx == -1 || FindNodeByValue(newVal) != -1) return;
    steps.clear(); currentStep = 0; currentOpText = "Updating Vertex: " + std::to_string(oldVal) + " -> " + std::to_string(newVal);
    nodes[idx].value = newVal; SaveState(); ForceStaticView();
}

void GraphBFS::InitFromEdges(const std::vector<std::pair<int, int>>& edgeList, const std::vector<int>& isolatedVertices) {
    ClearGraph();
    for (int v : isolatedVertices) if (FindNodeByValue(v) == -1) nodes.push_back({nextNodeId++, v, 0, 0});
    for (auto& e : edgeList) {
        int uIdx = FindNodeByValue(e.first); if (uIdx == -1) { nodes.push_back({nextNodeId++, e.first, 0, 0}); uIdx = nodes.size() - 1; }
        int vIdx = FindNodeByValue(e.second); if (vIdx == -1) { nodes.push_back({nextNodeId++, e.second, 0, 0}); vIdx = nodes.size() - 1; }
        bool exists = false; for (auto& ex : edges) if ((ex.u_id == nodes[uIdx].id && ex.v_id == nodes[vIdx].id) || (ex.u_id == nodes[vIdx].id && ex.v_id == nodes[uIdx].id)) { exists = true; break; }
        if (!exists) edges.push_back({nodes[uIdx].id, nodes[vIdx].id});
    }
    currentOpText = ""; SaveState(); ForceStaticView();
}

void GraphBFS::RunBFS(int startValue) {
    int startIdx = FindNodeByValue(startValue); if (startIdx == -1 || nodes.empty()) return;
    steps.clear(); currentStep = 0; currentOpText = "Running BFS from Source: " + std::to_string(startValue);
    std::vector<std::string> code = { "BFS(graph, start):", "  Queue q; q.push(start)", "  visited[start] = true", "  dist[start] = 0", "  while !q.empty():", "    u = q.front(); q.pop()", "    for v in adj[u]:", "      if !visited[v]:", "        visited[v] = true", "        dist[v] = dist[u] + 1", "        q.push(v)" };
    std::vector<GraphNodeState> nState(nodes.size(), GraphNodeState::NORMAL); std::vector<GraphEdgeState> eState(edges.size(), GraphEdgeState::NORMAL); std::map<int, int> dist;
    std::queue<int> q; std::vector<bool> vis(nodes.size(), false);
    q.push(startIdx); vis[startIdx] = true; dist[nodes[startIdx].value] = 0; nState[startIdx] = GraphNodeState::HIGHLIGHT; TakeSnapshot(1, code, nState, eState, dist, startValue); TakeSnapshot(3, code, nState, eState, dist, startValue);
    
    while (!q.empty()) {
        int u = q.front(); q.pop(); nState[u] = GraphNodeState::VISITED; TakeSnapshot(5, code, nState, eState, dist, startValue);
        for (int i=0; i<edges.size(); i++) {
            int uID = nodes[u].id; int vID = (edges[i].u_id == uID) ? edges[i].v_id : ((edges[i].v_id == uID) ? edges[i].u_id : -1);
            if (vID == -1) continue;
            int vIdx = -1; for(int j=0; j<nodes.size(); j++) if(nodes[j].id == vID) vIdx = j;
            eState[i] = GraphEdgeState::HIGHLIGHT; TakeSnapshot(6, code, nState, eState, dist, startValue);
            if (!vis[vIdx]) {
                vis[vIdx] = true; dist[nodes[vIdx].value] = dist[nodes[u].value] + 1; q.push(vIdx);
                nState[vIdx] = GraphNodeState::HIGHLIGHT; eState[i] = GraphEdgeState::SUCCESS; TakeSnapshot(8, code, nState, eState, dist, startValue);
            } else { eState[i] = GraphEdgeState::NORMAL; }
        }
    }
    
    currentOpText = ""; 
    TakeSnapshot(0, code, nState, eState, dist, startValue);
}

void GraphBFS::InitRandom(int count) {
    ClearGraph();
    // ĐÃ FIX CRASH: Tạo các đỉnh mang giá trị tuần tự từ 1 đến count để đảm bảo không trùng lặp
    for (int i = 1; i <= count; i++) AddVertex(i, false);
    // Đảm bảo đồ thị liên thông 1 phần
    for (int i = 1; i < count; i++) AddEdge(nodes[i-1].value, nodes[i].value, false);
    // Thêm cạnh ngẫu nhiên
    for (int i = 0; i < count; i++) { 
        int u = GetRandomValue(0, count-1); 
        int v = GetRandomValue(0, count-1); 
        if (u != v) AddEdge(nodes[u].value, nodes[v].value, false); 
    }
    currentOpText = ""; SaveState(); ForceStaticView();
}

void GraphBFS::CheckNodeClick() {
    if (steps.empty() && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        CalculatePositions(); Vector2 mousePos = GetMousePosition(); bool clicked = false;
        
        float r = std::max(30.0f, 60.0f - nodes.size() * 1.5f);
        
        for (int i = 0; i < nodes.size(); i++) { if (CheckCollisionPointCircle(mousePos, {nodes[i].x, nodes[i].y}, r)) { selectedIndex = i; clicked = true; break; } }
        if (!clicked && mousePos.y > 180 && mousePos.y < GetScreenHeight() - 100) selectedIndex = -1;
    }
}

void GraphBFS::Update() { CheckNodeClick(); if (steps.empty() || currentStep >= steps.size() - 1) { currentOpText = ""; return; } if (!isStepByStep) { timer += GetFrameTime() * speedMultiplier; if (timer >= 1.0f) { timer = 0.0f; currentStep++; } } }
void GraphBFS::StepForward() { if (!steps.empty() && currentStep < steps.size() - 1) { currentStep++; timer = 0.0f; } }
void GraphBFS::StepBackward() { if (!steps.empty() && currentStep > 0) { currentStep--; timer = 0.0f; } }

void GraphBFS::Draw(Theme theme, Font uiFont, Font monoFont) {
    if (steps.empty()) ForceStaticView(); if (steps.empty()) return;
    BFSAnimationStep& step = steps[currentStep];

    // ==========================================
    // ĐÃ CHỈNH KÍCH THƯỚC NODE TẠI ĐÂY
    // ==========================================
    float r = std::max(30.0f, 60.0f - step.nodes.size() * 1.5f);

    for (auto& edge : step.edges) {
        Color ec = theme.edgeNormal; if (edge.state == GraphEdgeState::HIGHLIGHT) ec = theme.nodeHighlight; else if (edge.state == GraphEdgeState::SUCCESS) ec = theme.nodeSuccess;
        DrawLineEx({edge.x1, edge.y1}, {edge.x2, edge.y2}, 4.0f, ec);
    }
    
    for (int i=0; i<step.nodes.size(); i++) {
        auto& node = step.nodes[i]; Color bg = theme.panelBg; Color border = theme.nodeNormal;
        if (i == selectedIndex && steps.size() <= 1) { border = theme.btnGradStart; bg = Fade(theme.btnGradStart, 0.2f); }
        else if (node.state == GraphNodeState::HIGHLIGHT) { border = theme.nodeHighlight; bg = Fade(theme.nodeHighlight, 0.4f); }
        else if (node.state == GraphNodeState::VISITED) { border = theme.nodeSuccess; bg = Fade(theme.nodeSuccess, 0.2f); }
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

    // Bảng Distances
    if (step.sourceValue != -1 && !step.distances.empty()) {
        float pX = 30.0f; 
        float pY = 260.0f; 
        float panelW = 250.0f;
        float itemH = 30.0f;
        float maxVisible = 5.0f;
        float panelH = 50.0f + maxVisible * itemH;

        Rectangle panelRec = {pX, pY, panelW, panelH};
        DrawRectangleRounded(panelRec, 0.1f, 8, theme.panelBg); 
        DrawRectangleRoundedLines(panelRec, 0.1f, 8, theme.nodeSuccess);
        DrawTextEx(uiFont, TextFormat("Distances from %d", step.sourceValue), {pX+15, pY+10}, 20, 1.0f, theme.textMain);

        // Nút tắt [X]
        Rectangle closeBtn = {pX + panelW - 35, pY + 8, 24, 24};
        bool isHovered = CheckCollisionPointRec(GetMousePosition(), closeBtn);
        DrawRectangleRounded(closeBtn, 0.2f, 4, isHovered ? GetColor(0xEF4444FF) : theme.panelBorder);
        DrawTextEx(monoFont, "X", {closeBtn.x + 6, closeBtn.y + 2}, 20, 1.0f, WHITE);
        
        if (isHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            step.distances.clear(); // Tắt bảng
            step.sourceValue = -1;
        }

        static float scrollY = 0.0f;
        float maxScroll = std::max(0.0f, step.distances.size() * itemH - maxVisible * itemH);
        
        if (CheckCollisionPointRec(GetMousePosition(), panelRec)) scrollY -= GetMouseWheelMove() * 20.0f;
        if (scrollY < 0) scrollY = 0; if (scrollY > maxScroll) scrollY = maxScroll;

        if (maxScroll > 0) {
            float scrollPct = scrollY / maxScroll;
            DrawRectangle(pX + panelW - 10, pY + 50, 6, panelH - 60, GetColor(0xFFFFFF20));
            DrawRectangle(pX + panelW - 10, pY + 50 + scrollPct * (panelH - 60 - 20), 6, 20, theme.btnGradEnd);
        }

        BeginScissorMode(pX, pY + 45, panelW, panelH - 50);
        int dy = 0; 
        for (auto const& [val, d] : step.distances) { 
            float itemY = pY + 50.0f + dy * itemH - scrollY;
            DrawTextEx(monoFont, TextFormat("Node %d : %d", val, d), {pX+20, itemY}, 20, 1.0f, theme.textMuted); 
            dy++; 
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