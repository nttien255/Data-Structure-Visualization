#ifndef HEAP_H
#define HEAP_H

#include "raylib.h"
#include "Core/Theme.h"
#include <vector>
#include <string>

enum class HeapNodeState { NORMAL, HIGHLIGHT, SUCCESS, ERROR };

struct HeapDrawNode {
    int data;
    float x, y;
    HeapNodeState state;
};

struct HeapDrawEdge {
    float x1, y1, x2, y2;
    Color color;
};

struct HeapAnimationStep {
    std::vector<HeapDrawNode> nodes;
    std::vector<HeapDrawEdge> edges;
    int activeCodeLine;
    std::vector<std::string> codeText;
    std::string currentOperation;
};

class Heap {
private:
    std::vector<int> data;
    bool isMaxHeap; // true = Max Heap, false = Min Heap

    int currentStep;
    float timer;
    std::string currentOpText;

    struct HeapState { std::vector<int> d; bool isMax; };
    std::vector<HeapState> history;
    int historyIndex;

    void SaveState();
    bool Compare(int a, int b); // Hàm so sánh động dựa trên isMaxHeap
    
    void CalculatePositions(int index, float left, float right, float y, float verticalGap, HeapAnimationStep& step, std::vector<HeapNodeState>& states);
    void TakeSnapshot(int activeLine, std::vector<std::string> code, std::vector<HeapNodeState> states);

    void HeapifyUp(int index, std::vector<std::string>& code);
    void HeapifyDown(int index, std::vector<std::string>& code);

public:
    std::vector<HeapAnimationStep> steps;
    float speedMultiplier;
    bool isStepByStep;

    Heap();

    void ToggleHeapType(); // Chuyển đổi Min/Max
    bool IsMaxHeap() const { return isMaxHeap; }

    void Insert(int val, bool clearSteps = true);
    void ExtractTop(); // Thay cho Delete/Search
    
    void InitFromArray(std::vector<int> arr);
    void InitRandom(int count);
    void ClearHeap();
    void ForceStaticView();

    void Undo();
    void Redo();

    void Update();
    void StepForward();
    void StepBackward();
    void Draw(Theme theme, Font uiFont, Font monoFont);
};

#endif