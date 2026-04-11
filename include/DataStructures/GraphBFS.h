#ifndef GRAPH_BFS_H
#define GRAPH_BFS_H

#include "raylib.h"
#include "Core/Theme.h"
#include <vector>
#include <string>
#include <map>

enum class GraphNodeState { NORMAL, HIGHLIGHT, SUCCESS, ERROR, VISITED };
enum class GraphEdgeState { NORMAL, HIGHLIGHT, SUCCESS };

struct BFSNode { int id, value; float x, y; };
struct BFSEdge { int u_id, v_id; };
struct BFSDrawNode { int value; float x, y; GraphNodeState state; };
struct BFSDrawEdge { float x1, y1, x2, y2; GraphEdgeState state; };

struct BFSAnimationStep {
    std::vector<BFSDrawNode> nodes;
    std::vector<BFSDrawEdge> edges;
    int activeCodeLine;
    std::vector<std::string> codeText;
    std::string currentOperation;
    std::map<int, int> distances;
    int sourceValue;
};

class GraphBFS {
private:
    std::vector<BFSNode> nodes; std::vector<BFSEdge> edges;
    int nextNodeId = 0; int currentStep; float timer; std::string currentOpText;
    struct GraphState { std::vector<BFSNode> n; std::vector<BFSEdge> e; int nextId; };
    std::vector<GraphState> history; int historyIndex;
    
    void SaveState(); void CalculatePositions();
    void TakeSnapshot(int activeLine, std::vector<std::string> code, std::vector<GraphNodeState> nStates, std::vector<GraphEdgeState> eStates, std::map<int, int> dist, int sourceVal);

public:
    std::vector<BFSAnimationStep> steps;
    float speedMultiplier; bool isStepByStep; int selectedIndex;

    GraphBFS();
    int FindNodeByValue(int val);
    void InitRandom(int count);
    void AddVertex(int val, bool clearSteps = true);
    void AddEdge(int u_val, int v_val, bool clearSteps = true);
    void DeleteVertex(int val);
    void UpdateVertex(int oldVal, int newVal);
    void RunBFS(int startValue);
    
    void InitFromEdges(const std::vector<std::pair<int, int>>& edgeList, const std::vector<int>& isolatedVertices);
    void ClearGraph(); void ForceStaticView(); void CheckNodeClick();
    void Undo(); void Redo(); void Update(); void StepForward(); void StepBackward();
    void Draw(Theme theme, Font uiFont, Font monoFont);
};
#endif