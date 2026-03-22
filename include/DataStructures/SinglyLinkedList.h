#ifndef SINGLY_LINKED_LIST_H
#define SINGLY_LINKED_LIST_H

#include "raylib.h"
#include "Core/Theme.h"
#include "Core/UILayout.h"
#include <vector>
#include <string>

enum class NodeState { NORMAL, HIGHLIGHT, SUCCESS, ERROR };

struct AnimationStep {
    std::vector<int> values;
    std::vector<NodeState> states;
    int activeCodeLine;
    std::vector<std::string> codeText;
};

class SinglyLinkedList {
private:
    std::vector<int> data;
    std::vector<AnimationStep> steps;
    int currentStep;
    float timer;
    float completionTimer; // BỘ ĐẾM 3 GIÂY
    
    // HISTORY CHO UNDO/REDO
    std::vector<std::vector<int>> history;
    int historyIndex;
    void SaveState();
    
    // LƯU TỌA ĐỘ NODE ĐỂ CLICK
    std::vector<Rectangle> nodeRects;

public:
    float speedMultiplier;
    bool isStepByStep;
    int selectedIndex;

    SinglyLinkedList();

    void InitRandom(int size);
    void InitFromArray(std::vector<int> arr);
    void AddTail(int val);
    void Search(int val);
    void Delete(int val);
    void ClearList();
    void UpdateNode(int newVal);
    
    void Undo();
    void Redo();

    void Update();
    void StepForward();
    void StepBackward();
    void CheckNodeClick();

    void Draw(Theme theme, Font uiFont, Font monoFont, UILayout layout);
};

#endif