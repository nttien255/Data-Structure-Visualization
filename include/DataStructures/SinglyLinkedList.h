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
    std::string currentOperation; // LƯU TRẠNG THÁI HIỂN THỊ
};

class SinglyLinkedList {
private:
    std::vector<int> data;
    std::vector<AnimationStep> steps;
    int currentStep;
    float timer;
    float completionTimer; 
    std::string currentOpText; // TEXT TẠM THỜI
    
    std::vector<std::vector<int>> history;
    int historyIndex;
    void SaveState();
    std::vector<Rectangle> nodeRects;

public:
    float speedMultiplier;
    bool isStepByStep;
    int selectedIndex;

    SinglyLinkedList();

    void InitRandom(int size);
    void InitFromArray(std::vector<int> arr); // CHẠY ANIMATION
    void AddTail(int val, bool clearSteps = true); // CỜ CLEAR
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