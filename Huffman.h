#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <string>
#include <unordered_map>

class HuffmanNode {
public:
    char data;
    unsigned freq;
    HuffmanNode *left, *right;

    HuffmanNode(char data, unsigned freq);
};

struct compare {
    bool operator()(HuffmanNode* l, HuffmanNode* r);
};

void printCodes(HuffmanNode* root, std::string str, std::unordered_map<char, std::string>& huffmanCode);
void HuffmanCodes(std::string data);
void HuffmanDecompress(const std::string& encodedData, HuffmanNode* root);


#endif // HUFFMAN_H