#ifndef RB_TREE_H
#define RB_TREE_H

#include "raylib.h"
#include "Core/Theme.h"
#include <vector>
#include <string>

enum class RBColor { RB_RED, RB_BLACK };
enum class RBNodeState { NORMAL, HIGHLIGHT, SUCCESS, ERROR };

struct RBNode {
    int data;
    RBColor color;
    RBNode* left;
    RBNode* right;
    RBNode* parent;
    RBNode(int val) : data(val), color(RBColor::RB_RED), left(nullptr), right(nullptr), parent(nullptr) {}
};

struct RBDrawNode {
    int data;
    float x, y;
    RBColor color;
    RBNodeState state;
};

struct RBDrawEdge {
    float x1, y1, x2, y2;
    Color color;
};

struct RBAnimationStep {
    std::vector<RBDrawNode> nodes;
    std::vector<RBDrawEdge> edges;
    int activeCodeLine;
    std::vector<std::string> codeText;
};

class RBTree {
private:
    RBNode* root;
    RBNode* TNULL; // Sentinel node

    int currentStep;
    float timer;
    RBNode* currentNodeHighlight;

    std::vector<RBNode*> history;
    int historyIndex;
    RBNode* CloneTree(RBNode* node, RBNode* parent, RBNode* oldTNULL);
    void SaveState();
    void ClearTreeMemory(RBNode* node);

    void LeftRotate(RBNode* x, std::vector<std::string>& code);
    void RightRotate(RBNode* x, std::vector<std::string>& code);
    void InsertFixup(RBNode* k, std::vector<std::string>& code);

    // CẬP NHẬT: Trả về float để vẽ Edges chính xác 100%
    float CalculatePositions(RBNode* node, int depth, float& currentX, float horizontalGap, float verticalGap, RBAnimationStep& step);
    void TakeSnapshot(int activeLine, std::vector<std::string> code);

public:
    std::vector<RBAnimationStep> steps; // Chuyển ra public để Screen có thể reset
    float speedMultiplier;
    bool isStepByStep;

    RBTree();
    void Insert(int key);
    void InitRandom(int count);
    void ClearTree();
    void ForceStaticView(); // Hàm mới: Ép vẽ cây tĩnh khi không có animation

    void Undo();
    void Redo();

    void Update();
    void StepForward();
    void StepBackward();
    void Draw(Theme theme, Font uiFont, Font monoFont);
};

#endif