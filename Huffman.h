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
std::string HuffmanCodes(std::string data);
HuffmanNode* buildHuffmanTree(std::string data);
std::string HuffmanDecompress(const std::string& encodedData, HuffmanNode* root);
std::string encodeData(const std::string& data, const std::unordered_map<char, std::string>& huffmanCode);



#endif // HUFFMAN_H