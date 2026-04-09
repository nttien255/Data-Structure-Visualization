#ifndef TRIE_H
#define TRIE_H

#include "raylib.h"
#include "Core/Theme.h"
#include <vector>
#include <string>

enum class TrieNodeState { NORMAL, HIGHLIGHT, SUCCESS, ERROR };

// Cấu trúc Node thật dùng để chạy thuật toán
struct TrieNode {
    TrieNode* children[26];
    bool isEndOfWord;
    TrieNode() {
        isEndOfWord = false;
        for (int i = 0; i < 26; i++) children[i] = nullptr;
    }
};

// Cấu trúc Node dùng để vẽ
struct TrieDrawNode {
    std::string text;
    bool isEndOfWord;
    TrieNodeState state;
    float x, y;
};

// Cấu trúc Cạnh dùng để vẽ
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
};

// Struct trả về tọa độ để vẽ
struct CalculateResult {
    float actualX;
    int leafCount;
};

class Trie {
private:
    TrieNode* root;
    
    // Animation Engine
    std::vector<TrieAnimationStep> steps;
    int currentStep;
    float timer;

    // Trạng thái Node tạm thời để chụp ảnh
    TrieNode* currentNodeHighlight;
    TrieNode* currentSuccessNode;

    CalculateResult CalculatePositionsAndCapture(TrieNode* node, float x, float y, float horizontalGap, TrieAnimationStep& step, std::string currentPrefix);
    
    void TakeSnapshot(int activeLine, std::vector<std::string> code);

    // ĐÃ THÊM: Hàm đệ quy thực thi Hard Delete
    bool DeleteHelper(TrieNode* curr, std::string word, int depth, std::vector<std::string>& code);

public:
    float speedMultiplier;
    bool isStepByStep;

    Trie();

    void InitRandom();
    void Insert(std::string word);
    void Search(std::string word);
    void Delete(std::string word);
    void ClearList();

    void Update();
    void StepForward();
    void StepBackward();
    void Draw(Theme theme, Font uiFont, Font monoFont);
};

#endif