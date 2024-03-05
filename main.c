#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WINDOW_SIZE 20

typedef struct {
    int offset;
    int length;
    char next_char;
} lz77_token;

int find_longest_match(char *buffer, int current_position, int end_of_buffer, int *match_position) {
    int i, j;
    int max_length = 0;
    *match_position = 0;

    for (i = current_position - WINDOW_SIZE; i < current_position; i++) {
        if (i >= 0) {
            for (j = 0; (i + j < current_position) && (current_position + j < end_of_buffer) && (buffer[i + j] == buffer[current_position + j]); j++) {
                // Match found
            }
            if (j > max_length) {
                max_length = j;
                *match_position = i;
            }
        }
    }
    return max_length;
}

void write_compressed_output(FILE *file, lz77_token token) {
    if (token.length > 0) {
        // Write the matched string's offset and length
        fprintf(file, "%d,%d ", token.offset, token.length);
    }
    // Write the next character
    fputc(token.next_char, file);
    fputc(' ', file);
}

int main() {
    FILE *input_file = fopen("source.txt", "rb");
    if (input_file == NULL) {
        perror("Error opening file");
        return 1;
    }
    
    // Read file into buffer
    fseek(input_file, 0, SEEK_END);
    long input_size = ftell(input_file);
    fseek(input_file, 0, SEEK_SET);
    char *buffer = malloc(input_size + 1);
    fread(buffer, 1, input_size, input_file);
    fclose(input_file);
    buffer[input_size] = '\0';

    FILE *output_file = fopen("LZ77.txt", "w");
    if (output_file == NULL) {
        perror("Error opening file");
        free(buffer);
        return 1;
    }

    int current_position = 0;
    while (current_position < input_size) {
        int match_position;
        int length = find_longest_match(buffer, current_position, input_size, &match_position);
        lz77_token token;

        if (length > 0) {
            token.offset = current_position - match_position;
            token.length = length;
            token.next_char = buffer[current_position + length];
            current_position += length + 1;
        } else {
            token.offset = 0;
            token.length = 0;
            token.next_char = buffer[current_position];
            current_position++;
        }
        write_compressed_output(output_file, token);
    }

    fclose(output_file);
    free(buffer);
    
    printf("Compression complete. Output written to LZ77.txt\n");
    return 0;
}