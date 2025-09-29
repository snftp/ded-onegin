#include "Onegin.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

void FillLines(char *text_buffer, line_t *lines, size_t lines_count) {

    char *pointer_on_symbol = text_buffer;
    char *new_beginning = text_buffer;

    for (size_t i = 0; i < lines_count; i++) {
        pointer_on_symbol = strchr(pointer_on_symbol, '\0');
        lines[i] = { .begin = new_beginning, .length = (size_t)(pointer_on_symbol - new_beginning)};
        new_beginning = pointer_on_symbol + 1;
        pointer_on_symbol++;
    }
}

void CountLinesReplaceBackspaces(char *text_buffer, size_t *count) {
    char *pointer_on_symbol = text_buffer;
    while ((pointer_on_symbol = strchr(pointer_on_symbol, '\n')) != NULL) {
        (*count)++;
        *pointer_on_symbol = '\0';
        pointer_on_symbol++;
    }
}

void GetFileLength(size_t *file_size, FILE *file) {
    fseek(file, 0, SEEK_END);
    *file_size = (size_t)ftell(file);
    rewind(file);
}

int main() {

// NOTE: turn struct text_t into thing created and destroyed with functions (like stack) so that main would be more user-friendly

    FILE *onegin = fopen("Evgeniy_Onegin.txt", "r"); // NOTE: fclose()
    assert(onegin != NULL);
    size_t file_size = 0;
    size_t lines_count = 0;


    GetFileLength(&file_size, onegin);

    char *text_buffer = (char*)calloc(file_size, sizeof(char)); // NOTE: free()
    assert(text_buffer != NULL);
    assert(fread(text_buffer, 1, file_size, onegin) == file_size);

    CountLinesReplaceBackspaces(text_buffer, &lines_count);

    text_t text = { .lines = (line_t*)calloc(lines_count, sizeof(line_t)), .text_size = file_size}; // NOTE: free()
    assert(text.lines != NULL);

    FillLines(text_buffer, text.lines, lines_count);
    return 0;
}
