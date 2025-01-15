#include <iostream>
#include <fstream>
#include <unordered_map>
#include <cstdint>
#include <queue>
#include <vector>
#include <cassert>
#include <bitset>
#include <memory>
#include <optional>
#include <filesystem>
#include <unistd.h>

struct Node {
    char character;
    uint32_t frequency;
    std::unique_ptr <Node> left;
    std::unique_ptr<Node> right;

    Node(char character, uint32_t frequency, std::unique_ptr<Node> left = nullptr,
    std::unique_ptr<Node> right = nullptr);
};

struct compare {
    bool operator()(const std::unique_ptr<Node> &left, const std::unique_ptr<Node> &right);
};

class HuffmanTree {
private:
    std::unique_ptr<Node> root;
    std::unordered_map<char, std::string> huffmanCodes;

    bool isEmpty(std::ifstream &inputFile) const;
    void buildFrequencyMap(std::unordered_map<char, uint32_t> &map, const std::string &inputFileName) const;
    void buildCodes(std::unique_ptr<Node> &root, std::string code);

    void serialization(const std::unique_ptr<Node> &root, std::ofstream &inputFile) const;
    std::unique_ptr<Node> deSerialization(std::ifstream &inputFile);
public:
    HuffmanTree();
    HuffmanTree(const std::string &inputFileName);
    void encode(const std::string &inputFileName, const std::string &outputFileName);
    void decode(const std::string &inputFileName, const std::string &outputFileName);
    ~HuffmanTree();
};

void print(const std::string &inputFileName, const std::string &outputFileName);