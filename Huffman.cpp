#include "Huffman.h"
#include <queue>
#include <vector>
#include <iostream>

HuffmanNode::HuffmanNode(char data, unsigned freq)
{
    this->data = data;
    this->freq = freq;
    left = right = nullptr;
}

bool compare::operator()(HuffmanNode *l, HuffmanNode *r)
{
    return (l->freq > r->freq);
}

void printCodes(HuffmanNode *root, std::string str, std::unordered_map<char, std::string> &huffmanCode)
{
    if (!root)
        return;

    if (root->data != '$')
    {
        huffmanCode[root->data] = str;
    }

    printCodes(root->left, str + "0", huffmanCode);
    printCodes(root->right, str + "1", huffmanCode);
}

/**
 * @brief Generates Huffman codes for the given data.
 *
 * This function takes a string of data and generates Huffman codes for each character in the data.
 * Huffman coding is a lossless data compression algorithm that assigns variable-length codes to different characters
 * based on their frequencies of occurrence in the input data. The generated Huffman codes can be used to compress
 * the data by replacing each character with its corresponding Huffman code.
 *
 * @param data The input data for which Huffman codes need to be generated.
 * @return A string containing the Huffman codes for each character in the input data.
 */
std::string HuffmanCodes(std::string data) {
    // Count frequency of each character
    std::unordered_map<char, unsigned> freq;
    for (char ch : data) {
        freq[ch]++;
    }

    // Create a priority queue to store nodes of the Huffman tree
    std::priority_queue<HuffmanNode *, std::vector<HuffmanNode *>, compare> minHeap;

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
        HuffmanNode *top = new HuffmanNode('$', left->freq + right->freq);
        top->left = left;
        top->right = right;

        minHeap.push(top);
    }

    // Generate Huffman codes
    std::unordered_map<char, std::string> huffmanCode;
    printCodes(minHeap.top(), "", huffmanCode);

    // Encode the input data using the Huffman codes
    std::string encodedData = encodeData(data, huffmanCode);

    // Return the encoded data
    return encodedData;
}

/**
 * @brief Decompresses the encoded data using the Huffman algorithm.
 *
 * This function takes in the encoded data and a pointer to the root of the Huffman tree.
 * It performs the decompression process by traversing the Huffman tree based on the encoded data.
 * The decompressed data is returned as a string.
 *
 * @param encodedData The encoded data to be decompressed.
 * @param root Pointer to the root of the Huffman tree.
 * @return The decompressed data as a string.
 */
std::string HuffmanDecompress(const std::string &encodedData, HuffmanNode *root)
{
    std::string decodedData;
    HuffmanNode *currentNode = root;

    for (char bit : encodedData)
    {
        if (bit == '0')
        {
            currentNode = currentNode->left;
        }
        else if (bit == '1')
        {
            currentNode = currentNode->right;
        }

        // When reaching a leaf node
        if (!currentNode->left && !currentNode->right)
        {
            decodedData += currentNode->data;
            currentNode = root; // Go back to the root for the next character
        }
    }

    return decodedData;
}

std::string encodeData(const std::string &data, const std::unordered_map<char, std::string> &huffmanCode)
{
    std::string encodedData;
    for (char ch : data)
    {
        encodedData += huffmanCode.at(ch);
    }
    return encodedData;
}

/**
 * @brief Builds a Huffman tree based on the given data.
 *
 * This function takes a string of data and constructs a Huffman tree using the Huffman algorithm.
 * The Huffman tree is used for data compression and decompression.
 *
 * @param data The input data for constructing the Huffman tree.
 * @return A pointer to the root node of the constructed Huffman tree.
 */
HuffmanNode *buildHuffmanTree(std::string data)
{
    // Count frequency of each character
    std::unordered_map<char, unsigned> freq;
    for (char ch : data)
    {
        freq[ch]++;
    }

    // Create a priority queue to store nodes of the Huffman tree
    std::priority_queue<HuffmanNode *, std::vector<HuffmanNode *>, compare> minHeap;

    // Create a leaf node for each character and add it to the priority queue.
    for (auto pair : freq)
    {
        minHeap.push(new HuffmanNode(pair.first, pair.second));
    }

    // Iterate while size of heap doesn't become 1
    while (minHeap.size() != 1)
    {
        // Extract the two minimum frequency nodes from min heap
        HuffmanNode *left = minHeap.top();
        minHeap.pop();

        HuffmanNode *right = minHeap.top();
        minHeap.pop();

        // Create a new internal node with frequency equal to the sum of the two nodes frequencies.
        HuffmanNode *top = new HuffmanNode('$', left->freq + right->freq);
        top->left = left;
        top->right = right;

        minHeap.push(top);
    }

    // The root of the tree is the last remaining node
    return minHeap.top();
}