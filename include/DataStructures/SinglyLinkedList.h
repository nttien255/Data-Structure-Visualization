#ifndef SINGLY_LINKED_LIST_H
#define SINGLY_LINKED_LIST_H

#include "raylib.h"
#include "Core/Theme.h"
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
    
public:
    float speedMultiplier; 
    bool isStepByStep; 

    SinglyLinkedList();

    void InitRandom(int size);
    void InitFromArray(std::vector<int> arr);
    void AddTail(int val);
    void Search(int val);
    void Delete(int val); // <--- Bổ sung tính năng Xóa
    void ClearList();

    void Update();
    void StepForward();  
    void StepBackward(); 
    
    void Draw(Theme theme, Font uiFont, Font monoFont);
};

#endif