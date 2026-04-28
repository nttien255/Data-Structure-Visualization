#ifndef TRIE_H
#define TRIE_H

#include "raylib.h"
#include "Core/Theme.h"
#include <vector>
#include <string>

enum class TrieNodeState { NORMAL, HIGHLIGHT, SUCCESS, ERROR };

struct TrieNode {
    TrieNode* children[26];
    bool isEndOfWord;
    TrieNode() { isEndOfWord = false; for (int i = 0; i < 26; i++) children[i] = nullptr; }
};

struct TrieDrawNode {
    std::string text;
    bool isEndOfWord;
    TrieNodeState state;
    float x, y;
};

struct TrieDrawEdge {
    float x1, y1, x2, y2;
    Color color;
    char letter;
};

struct TrieAnimationStep {
    std::vector<TrieDrawNode> nodes;
    std::vector<TrieDrawEdge> edges;
    int activeCodeLine;
    std::vector<std::string> codeText;
    std::string currentOperation; // LƯU TRẠNG THÁI HIỂN THỊ
};

struct CalculateResult { float actualX; int leafCount; };

class Trie {
private:
    TrieNode* root;
    int currentStep;
    float timer;
    TrieNode* currentNodeHighlight;
    TrieNode* currentSuccessNode;
    std::string currentOpText; // TEXT TẠM THỜI
    std::vector<TrieNode*> history;
    int historyIndex;

    CalculateResult CalculatePositionsAndCapture(TrieNode* node, float x, float y, float horizontalGap, TrieAnimationStep& step, std::string currentPrefix);
    void TakeSnapshot(int activeLine, std::vector<std::string> code);
    bool DeleteHelper(TrieNode* curr, std::string word, int depth, std::vector<std::string>& code);

public:
    std::vector<TrieAnimationStep> steps;
    float speedMultiplier;
    bool isStepByStep;

    Trie();

    void InitRandom();
    void InitFromArray(std::vector<std::string> arr); // CHẠY ANIMATION
    void Insert(std::string word, bool clearSteps = true); // CỜ CLEAR
    void Search(std::string word);
    void Delete(std::string word);
    void ClearList();
    void SaveState();
    void Undo();
    void Redo();
    void ForceStaticView();
    TrieNode* CloneTrie(TrieNode* node);
    void ClearTrieMemory(TrieNode* node);
    void StepForward();
    void StepBackward();
    void Update();
    void Draw(Theme theme, Font uiFont, Font monoFont);
};

#endif