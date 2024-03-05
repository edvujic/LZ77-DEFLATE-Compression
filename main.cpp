#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include "Huffman.cpp"
const size_t MAX_WINDOW_SIZE = 1024; // Maximum window size

struct LZ77Token {
    size_t offset;  // Offset from the start of the window
    size_t length;  // Match length
    char nextChar;  // Next character after the match

    LZ77Token(size_t offset, size_t length, char nextChar)
        : offset(offset), length(length), nextChar(nextChar) {}
};

std::vector<LZ77Token> lz77Compress(const std::string& text) {
    std::vector<LZ77Token> tokens;
    size_t i = 0;

    while (i < text.size()) {
        size_t maxMatchLength = 0;
        size_t maxMatchDistance = 0;
        size_t startWindow = (i < MAX_WINDOW_SIZE) ? 0 : i - MAX_WINDOW_SIZE;

        for (size_t j = startWindow; j < i; ++j) {
            size_t matchLength = 0;
            while (text[i + matchLength] == text[j + matchLength]) {
                matchLength++;
                if (i + matchLength >= text.size() || j + matchLength >= i) {
                    break;
                }
            }

            if (matchLength > maxMatchLength) {
                maxMatchLength = matchLength;
                maxMatchDistance = i - j;
            }
        }

        if (maxMatchLength == 0) {
            maxMatchDistance = 0;
            maxMatchLength = 0;
        }

        tokens.emplace_back(maxMatchDistance, maxMatchLength, text[i + maxMatchLength]);
        i += maxMatchLength + 1;
    }

    return tokens;
}

std::string compressTokensToString(const std::vector<LZ77Token>& tokens) {
    std::stringstream ss;
    for (const auto& token : tokens) {
        ss << "(" << token.offset << "," << token.length << "," << token.nextChar << ")";
    }
    return ss.str();
}

void writeCompressedToFile(const std::string& compressedData, const std::string& filename) {
    std::ofstream outFile(filename);
    outFile << compressedData;
    outFile.close();
}

int main() {
    std::ifstream sourceFile("source.txt");
    if (!sourceFile.is_open()) {
        std::cerr << "Error opening source.txt" << std::endl;
        return 1;
    }

    std::string sourceText((std::istreambuf_iterator<char>(sourceFile)), std::istreambuf_iterator<char>());
    sourceFile.close();

    std::vector<LZ77Token> compressedTokens = lz77Compress(sourceText);

    std::string compressedData = compressTokensToString(compressedTokens);

    writeCompressedToFile(compressedData, "LZ77_output.txt");

    std::cout << compressedData << std::endl;
    HuffmanCodes(compressedData);

    return 0;
}