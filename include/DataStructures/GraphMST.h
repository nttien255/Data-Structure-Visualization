#ifndef GRAPH_MST_H
#define GRAPH_MST_H

#include "raylib.h"
#include "Core/Theme.h"
#include <vector>
#include <string>

enum class MSTNodeState { NORMAL, HIGHLIGHT, FIND };
enum class MSTEdgeState { NORMAL, HIGHLIGHT, SUCCESS };

struct MSTNode { int id, value; float x, y; };
struct MSTEdge { int u_id, v_id, weight; };

struct MSTDrawNode { int value; float x, y; MSTNodeState state; };
struct MSTDrawEdge { float x1, y1, x2, y2; int weight; MSTEdgeState state; };
struct MSTEdgeInfo { int u_val, v_val, w; bool isSelected; };

struct MSTAnimationStep {
    std::vector<MSTDrawNode> nodes; std::vector<MSTDrawEdge> edges;
    int activeCodeLine; std::vector<std::string> codeText; std::string currentOperation;
    std::vector<MSTEdgeInfo> sortedEdges; int currentEdgeIdx;
};

struct InitEdge { int u, v, w; };

class GraphMST {
private:
    std::vector<MSTNode> nodes; std::vector<MSTEdge> edges;
    int nextNodeId = 0; int currentStep; float timer; std::string currentOpText;
    struct GraphState { std::vector<MSTNode> n; std::vector<MSTEdge> e; int nextId; };
    std::vector<GraphState> history; int historyIndex;
    
    void SaveState(); void CalculatePositions();
    void TakeSnapshot(int activeLine, std::vector<std::string> code, std::vector<MSTNodeState> nStates, std::vector<MSTEdgeState> eStates, std::vector<MSTEdgeInfo> sEdges, int curEIdx);
    int DSUFind(int i, std::vector<int>& parent, std::vector<std::string>& code, std::vector<MSTNodeState>& nState, std::vector<MSTEdgeState>& eState, std::vector<MSTEdgeInfo>& sEdges, int curEIdx);

public:
    std::vector<MSTAnimationStep> steps;
    float speedMultiplier; bool isStepByStep; int selectedIndex;

    GraphMST();
    int FindNodeByValue(int val);
    void InitRandom(int count);
    void AddVertex(int val, bool clearSteps = true);
    void AddEdge(int u_val, int v_val, int w, bool clearSteps = true);
    void DeleteVertex(int val);
    void UpdateVertex(int oldVal, int newVal);
    void RunKruskal();
    
    void InitFromEdges(const std::vector<InitEdge>& edgeList, const std::vector<int>& isolatedVertices);
    void ClearGraph(); void ForceStaticView(); void CheckNodeClick();
    void Undo(); void Redo(); void Update(); void StepForward(); void StepBackward();
    void Draw(Theme theme, Font uiFont, Font monoFont);
};
#endif