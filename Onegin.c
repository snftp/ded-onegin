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

void ConstructText(text_t *text) {
    FILE *onegin = fopen("Evgeniy_Onegin.txt", "r");
    assert(onegin != NULL);

    GetFileLength(&(text->text_size), onegin);

    text->text_buffer = (char*)calloc(text->text_size, sizeof(char)); // NOTE: free()
    assert(text->text_buffer != NULL);

    assert(fread(text->text_buffer, sizeof(char), text->text_size, onegin) == text->text_size);

    CountLinesReplaceBackspaces(text->text_buffer, &(text->lines_count));

    text->lines = (line_t*)calloc(text->lines_count, sizeof(line_t)); // NOTE: free()
    assert(text->lines != NULL);

    FillLines(text->text_buffer, text->lines, text->lines_count);

    fclose(onegin);
}

// void DestructText(text_t *text) {}

int main() {
    text_t text = { .lines = NULL, .lines_count = 0, .text_size = 0, .text_buffer = NULL};
    ConstructText(&text);
    return 0;
}
