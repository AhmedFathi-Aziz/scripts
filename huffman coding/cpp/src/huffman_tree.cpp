#include "huffman_tree.h"

Node::Node(char character, uint32_t frequency, std::unique_ptr<Node> left, std::unique_ptr<Node> right)
    : character(character), frequency(frequency), left(std::move(left)), right(std::move(right)) {}

bool compare::operator()(const std::unique_ptr<Node> &left, const std::unique_ptr<Node> &right) {
    return left->frequency > right->frequency;
}

bool HuffmanTree::isEmpty(std::ifstream &inputFile) const {
    assert(inputFile.is_open());
    inputFile.seekg(0, std::ios::end);
    if (inputFile.tellg() == 0) {
        return true;
    }
    inputFile.seekg(0, std::ios::beg);
    return false;
}

void HuffmanTree::buildFrequencyMap(std::unordered_map<char, uint32_t> &map, const std::string &inputFileName) const {
    std::ifstream inputFile(inputFileName, std::ios::in);
    assert(!isEmpty(inputFile));
    
    char character;
    while (inputFile.get(character)) {
        ++map[character];
    }
    inputFile.close();
}

void HuffmanTree::buildCodes(std::unique_ptr<Node> &root, std::string code) {
    if (root == nullptr) {
        return;
    }
    if (root->character != '\0') {
        huffmanCodes[root->character] = code;
        return;
    }
    code.push_back('0');
    buildCodes(root->left, code);
    code.pop_back();
    
    code.push_back('1');
    buildCodes(root->right, code);
    code.pop_back();
}

HuffmanTree::HuffmanTree() : root(nullptr) {}

HuffmanTree::HuffmanTree(const std::string &inputFileName) : root(nullptr) {
    std::unordered_map<char, uint32_t> map;
    std::ifstream inputFile(inputFileName, std::ios::in);
    assert(!isEmpty(inputFile));

    char character;
    while (inputFile.get(character)) {
        ++map[character];
    }
    std::priority_queue<std::unique_ptr<Node>, std::vector<std::unique_ptr<Node>>, compare> minHeap;
    for (auto &iterator : map) {
        minHeap.push(std::make_unique<Node>(iterator.first, iterator.second));
    }
    while (minHeap.size() != 1) {
        auto left = std::move(const_cast<std::unique_ptr<Node>&>(minHeap.top()));
        minHeap.pop();

        auto right = std::move(const_cast<std::unique_ptr<Node>&>(minHeap.top()));
        minHeap.pop();
        
        auto newNode = std::make_unique<Node>('\0', left->frequency + right->frequency, std::move(left), std::move(right));
        minHeap.push(std::move(newNode));
    }
    root = std::move(const_cast<std::unique_ptr<Node>&>(minHeap.top()));
    minHeap.pop();
    buildCodes(root, "");
}

void HuffmanTree::encode(const std::string &inputFileName, const std::string &outputFileName) {
    std::ifstream inputFile(inputFileName, std::ios::in);
    assert(!isEmpty(inputFile));

    std::ofstream outputFile(outputFileName, std::ios::binary | std::ios::out);
    assert(outputFile.is_open());
    serialization(root, outputFile); // todo [store the tree as a binary data]
    char character;
    std::string content;
    while (inputFile.get(character)) {
        content.append(huffmanCodes[character]);
    }
    assert(!content.empty());
    uint32_t padding_bits = ((8 - (content.size() % 8)) % 8);
    if (padding_bits)
        content += std::string(padding_bits, '0');
    outputFile.put(static_cast<char> (padding_bits));
    for (int i = 0; i < (int) content.size(); i += 8) {
        std::bitset<8> bits(content.substr(i, 8));
        outputFile.put(static_cast<char>(bits.to_ulong()));
    }
    inputFile.close();
    outputFile.close();
}

void HuffmanTree::decode(const std::string &inputFileName, const std::string &outputFileName) {
    std::ifstream inputFile(inputFileName, std::ios::binary | std::ios::in);
    assert(!isEmpty(inputFile));

    std::ofstream outputFile(outputFileName, std::ios::out);
    assert(outputFile.is_open());

    root = deSerialization(inputFile);
    assert(root != nullptr);
    char paddingBits;
    inputFile.get(paddingBits);
    uint32_t padding = static_cast<uint32_t>(paddingBits); 
    std::string content;
    char Byte;
    while (inputFile.get(Byte)) {
        std::bitset<8> bits(Byte);
        content += bits.to_string();
    }
    if (padding)
        content = content.substr(0, content.size() - padding);
    Node *current = root.get();
    for (char ch : content) {
        if (ch == '0') {
            current = current->left.get();
        } else if (ch == '1') {
            current = current->right.get();
        }
        if (!current->left && !current->right) {
            outputFile << current->character;
            current = root.get();
        }
    }
    inputFile.close();
    outputFile.close();
}

void HuffmanTree::serialization(const std::unique_ptr<Node> &root, std::ofstream &inputFile) const {
    if (root == nullptr) {
        return;
    }
    std::queue<const Node*> q;
    q.push(root.get());
    while (!q.empty()) {
        const Node *current = q.front();
        q.pop();

        if (current->character != '\0') {
            inputFile.put('1'); 
            inputFile.put(current->character);
        } else {
            inputFile.put('0');
            q.push(current->left.get());
            q.push(current->right.get());
        }
    }
}

std::unique_ptr<Node> HuffmanTree::deSerialization(std::ifstream &inputFile) {
    char character;
    inputFile.get(character);
    assert(character == '0' || character == '1');
    auto head = std::make_unique<Node>('\0', 0);
    std::queue<Node*> q;
    q.push(head.get());
    while (!q.empty()) {
        Node *current = q.front();
        q.pop();
        assert(inputFile.get(character));
        if (character == '1') {
            assert(inputFile.get(character));
            current->left = std::make_unique<Node>(character, 0);

        } else if (character == '0') {
            current->left = std::make_unique<Node>('\0', 0);
            q.push(current->left.get());
        }
        assert(inputFile.get(character));
        if (character == '1') {
            assert(inputFile.get(character));
            current->right = std::make_unique<Node>(character, 0);

        } else if (character == '0') {
            current->right = std::make_unique<Node>('\0', 0);
            q.push(current->right.get());
        }
    }   
    return head;
}

HuffmanTree::~HuffmanTree() {
}

void print(const std::string &inputFileName, const std::string &outputFileName) {
    try {
        std::uintmax_t originalSize = std::filesystem::file_size(inputFileName);
        std::uintmax_t compressedSize = std::filesystem::file_size(outputFileName);
        double compressionRatio = static_cast<double> (originalSize) / compressedSize;

        std::cout << "Original Size: " << originalSize << " bytes" << std::endl;
        std::cout << "Compressed Size: " << compressedSize << " bytes" << std::endl;

        std::cout << "Compression Ratio: " <<  compressionRatio << std::endl;

    } catch (const std::filesystem::filesystem_error &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

}