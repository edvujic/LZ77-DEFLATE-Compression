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

/**
 * @brief Compresses the given text using the LZ77 algorithm.
 *
 * @param text The input text to be compressed.
 * @return A vector of LZ77Token representing the compressed text.
 */
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

std::vector<LZ77Token> parseTokensFromString(const std::string& tokenStr) {
    std::vector<LZ77Token> tokens;
    std::istringstream tokenStream(tokenStr);
    char delimiter;

    // Expected format: "(offset,length,nextChar)"
    size_t offset, length;
    char nextChar;

    while (tokenStream >> delimiter) {
        if (delimiter != '(') {
            throw std::runtime_error("Invalid token format");
        }
        if (!(tokenStream >> offset >> delimiter) || delimiter != ',') {
            throw std::runtime_error("Invalid token format");
        }
        if (!(tokenStream >> length >> delimiter) || delimiter != ',') {
            throw std::runtime_error("Invalid token format");
        }
        if (!(tokenStream >> nextChar >> delimiter) || delimiter != ')') {
            throw std::runtime_error("Invalid token format");
        }
        tokens.emplace_back(offset, length, nextChar);
    }
    return tokens;
}

/**
 * @brief Decompresses a sequence of LZ77 tokens and returns the decompressed string.
 *
 * This function takes a vector of LZ77 tokens as input and performs the decompression
 * process to reconstruct the original string. The LZ77 algorithm is a lossless data
 * compression algorithm that replaces repeated occurrences of data with references to
 * a dictionary window.
 *
 * @param tokens The vector of LZ77 tokens representing the compressed data.
 * @return The decompressed string.
 */
std::string lz77Decompress(const std::vector<LZ77Token>& tokens) {
    std::string decompressedText;
    for (const LZ77Token& token : tokens) {
        if (token.length > 0) {
            // Copy the matching string from the window
            size_t startPos = decompressedText.size() - token.offset;
            for (size_t i = 0; i < token.length; ++i) {
                decompressedText += decompressedText[startPos + i];
            }
        }
        // Append the next character
        decompressedText += token.nextChar;
    }
    return decompressedText;
}

std::string lz77DecompressFromString(const std::string& tokenStr) {
    std::vector<LZ77Token> tokens = parseTokensFromString(tokenStr);
    return lz77Decompress(tokens);
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

    std::cout << "Compressed data has been written to 'LZ77_output.txt'" << std::endl;
    std::cout << "Compressed Data with LZ77:\n " << compressedData << std::endl;

    HuffmanNode* root = buildHuffmanTree(compressedData);
    std::string encodedData = HuffmanCodes(compressedData);

    std::cout << "Encoded Data with Huffman:\n" << encodedData << std::endl;

    // Open a file in write mode.
    std::ofstream outFile("DEFLATE_output");
    std::cout << "Writing encoded data to 'DEFLATE_output'..." << std::endl;
    // Check if the file stream is good to go before writing to it.
    if (outFile.good()) {
        outFile << encodedData;
        outFile.close();
        std::cout << "Encoded Data has been written to 'DEFLATE_output'." << std::endl;
    } else {
        std::cerr << "Error: Could not open file 'DEFLATE_output' for writing." << std::endl;
    }

    // Decompress using the Huffman tree
    std::string decodedData = HuffmanDecompress(encodedData, root);
    std::cout << "Decoded Data with Huffman:\n" << decodedData << std::endl;
    std::string decodedLZ77 = lz77DecompressFromString(decodedData);
    std::cout << "Decompressed Data with LZ77:" << decodedLZ77 << std::endl;

    return 0;
}