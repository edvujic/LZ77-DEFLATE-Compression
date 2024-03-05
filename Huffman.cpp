#include "Huffman.h"
#include <queue>
#include <vector>
#include <iostream>

HuffmanNode::HuffmanNode(char data, unsigned freq) {
    this->data = data;
    this->freq = freq;
    left = right = nullptr;
}

bool compare::operator()(HuffmanNode* l, HuffmanNode* r) {
    return (l->freq > r->freq);
}

void printCodes(HuffmanNode* root, std::string str, std::unordered_map<char, std::string>& huffmanCode) {
    if (!root) return;

    if (root->data != '$') {
        huffmanCode[root->data] = str;
    }

    printCodes(root->left, str + "0", huffmanCode);
    printCodes(root->right, str + "1", huffmanCode);
}

void HuffmanCodes(std::string data) {
    // Count frequency of each character
    std::unordered_map<char, unsigned> freq;
    for (char ch : data) {
        freq[ch]++;
    }

    // Create a priority queue to store nodes of the Huffman tree
    std::priority_queue<HuffmanNode*, std::vector<HuffmanNode*>, compare> minHeap;

    // Create a leaf node for each character and add it to the priority queue.
    for (auto pair : freq) {
        minHeap.push(new HuffmanNode(pair.first, pair.second));
    }

    // Iterate while size of heap doesn't become 1
    while (minHeap.size() != 1) {
        // Extract the two minimum frequency nodes from min heap
        HuffmanNode *left = minHeap.top();
        minHeap.pop();

        HuffmanNode *right = minHeap.top();
        minHeap.pop();

        // Create a new internal node with frequency equal to the sum of the two nodes frequencies.
        // Make the two extracted nodes as left and right children of this new node.
        // Add this node to the min heap '$' is a special value for internal nodes, not used
        HuffmanNode *top = new HuffmanNode('$', left->freq + right->freq);
        top->left = left;
        top->right = right;

        minHeap.push(top);
    }

    // Print Huffman codes using the Huffman tree built above
    std::unordered_map<char, std::string> huffmanCode;
    printCodes(minHeap.top(), "", huffmanCode);

    std::cout << "Character With Its Huffman Code:\n";
    for (auto pair : huffmanCode) {
        std::cout << pair.first << ": " << pair.second << "\n";
    }
}

void HuffmanDecompress(const std::string& encodedData, HuffmanNode* root) {
    std::string decodedData;
    HuffmanNode* currentNode = root;
    
    for (char bit : encodedData) {
        if (bit == '0') {
            currentNode = currentNode->left;
        } else if (bit == '1') {
            currentNode = currentNode->right;
        }

        // When reaching a leaf node
        if (!currentNode->left && !currentNode->right) {
            decodedData += currentNode->data;
            currentNode = root; // Go back to the root for the next character
        }
    }

    std::cout << "Decoded Data: " << decodedData << std::endl;
}