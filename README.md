# LZ77 Compression and Decompression

This repository contains an implementation of the LZ77 compression and decompression algorithm in C++. LZ77 is a lossless data compression algorithm that forms the basis for several file compression formats, including DEFLATE, which is used in ZIP and GZIP file formats.

## Overview

LZ77 is a dictionary-based compression algorithm that replaces sequences of data that occur more than once (redundancies) with references to a single copy of that data existing earlier in the uncompressed data stream. A reference is represented by a pair of numbers `(distance, length)` which, along with a following literal character, form a token. The `distance` specifies how far back in the window to go to find the beginning of the match, and `length` specifies how many characters to copy from this position. The literal character `nextChar` is the next character in the sequence after the match.

## Features

- LZ77 Compression: Generate a compressed representation of the input data.
- LZ77 Decompression: Reconstruct the original data from the compressed format.
- Token Parsing: Convert a string of tokens in the format `(offset,length,nextChar)` into a vector of `LZ77Token` objects.
- Detailed Example: The repository includes a main program that demonstrates the compression and decompression processes.

## Getting Started

To get started with this LZ77 compression and decompression implementation, clone this repository to your local machine.

```bash
git clone https://github.com/your-username/lz77-compression.git
cd lz77-compression
```
## Prerequisites
This project requires a C++ compiler with support for C++11 or later. The code has been tested with GCC and Clang on Linux and macOS, and with MSVC on Windows.

## Building the Project

You can compile the project using the following command:
```bash
g++ -std=c++11 -o lz77 main.cpp LZ77.cpp
```
Replace main.cpp and LZ77.cpp with the actual file names in the project. If there are additional source files, include them in the compilation command.

### Usage

Run the compiled binary to see the LZ77 compression and decompression in action.

```bash
./lz77
```

### Code Structure
The codebase is structured as follows:

- Huffman.h and Huffman.cpp: These files contain the declarations and definitions for the Huffman encoding and decoding functions.
- main.cpp: This file contains the main program that demonstrates how to use the LZ77 functions to compress and decompress data.

### Pseudocode

#### LZ77 Compression
```pseudocode
FUNCTION lz77Compress(text)
    DECLARE tokens as vector of LZ77Token
    SET i to 0

    WHILE i is less than the size of text
        SET maxMatchLength to 0
        SET maxMatchDistance to 0
        SET startWindow to max(0, i - MAX_WINDOW_SIZE)

        FOR each j from startWindow to i - 1
            SET matchLength to 0
            WHILE text[i + matchLength] equals text[j + matchLength]
                INCREMENT matchLength
                IF i + matchLength is not within text OR j + matchLength reaches i
                    BREAK
            END WHILE

            IF matchLength is greater than maxMatchLength
                SET maxMatchLength to matchLength
                SET maxMatchDistance to i - j
            END IF
        END FOR

        APPEND a new LZ77Token with maxMatchDistance, maxMatchLength, and text[i + maxMatchLength] to tokens
        INCREMENT i by maxMatchLength + 1
    END WHILE

    RETURN tokens
END FUNCTION
```
#### LZ77 Decompression
```pseudocode
FUNCTION lz77Decompress(tokens)
    DECLARE decompressedText as empty string

    FOR each token in tokens
        IF token length is greater than 0
            SET startPos to size of decompressedText - token offset
            FOR i from 0 to token length - 1
                APPEND character at decompressedText[startPos + i] to decompressedText
            END FOR
        END IF
        APPEND token nextChar to decompressedText
    END FOR

    RETURN decompressedText
END FUNCTION
```
#### Huffman Encoding
```pseudocode
FUNCTION HuffmanCodes(data)
    DECLARE freq as an unordered_map to count character frequencies
    FOR each character ch in data
        INCREMENT freq[ch]
    END FOR

    DECLARE minHeap as a priority queue of HuffmanNode pointers

    FOR each pair in freq
        PUSH a new HuffmanNode of pair into minHeap
    END FOR

    WHILE the size of minHeap is not 1
        EXTRACT the two nodes with the lowest frequency from minHeap
        CREATE a new internal node with the sum of the two nodes' frequencies
        SET the new node's left child to the first extracted node
        SET the new node's right child to the second extracted node
        PUSH the new node back into minHeap
    END WHILE

    DECLARE huffmanCode as an unordered_map to store Huffman codes
    CALL printCodes with minHeap's top node, an empty string, and huffmanCode

    DECLARE encodedData as empty string
    SET encodedData to the result of encodeData with data and huffmanCode

    RETURN encodedData
END FUNCTION
```
#### Huffman Decoding
```pseudocode
FUNCTION HuffmanDecompress(encodedData, root)
    DECLARE decodedData as empty string
    SET currentNode to root

    FOR each bit in encodedData
        IF bit equals '0'
            SET currentNode to currentNode's left child
        ELSE IF bit equals '1'
            SET currentNode to currentNode's right child
        END IF

        IF currentNode is a leaf node (no left or right children)
            APPEND currentNode's data to decodedData
            RESET currentNode to root
        END IF
    END FOR

    RETURN decodedData
END FUNCTION
```

## Contributing
Contributions are welcome! Feel free to open a pull request or an issue if you have suggestions or find a bug.

## Acknowledgements

- The LZ77 algorithm was originally developed by Abraham Lempel and Jacob Ziv.
- This implementation was inspired by the descriptions of the LZ77 algorithm in various compression literature and online resources.
