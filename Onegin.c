#include "Onegin.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void WriteText(char *text_buffer, size_t lines_count, FILE *output) {
    char *pointer_on_symbol = text_buffer;
    char *new_beginning = text_buffer;

    for (size_t i = 0; i < lines_count; i++) {
        pointer_on_symbol = strchr(pointer_on_symbol, '\0');
        fwrite(new_beginning, sizeof(char), (size_t)(pointer_on_symbol - new_beginning), output);
        fputc('\n', output);
        new_beginning = pointer_on_symbol + 1;
        pointer_on_symbol++;
    }
}

void WritePointersArray(line_t *lines, size_t lines_count, FILE *output) {
    for (size_t i = 0; i < lines_count; i++) {
        assert(lines[i].begin != NULL);
        assert(lines[i].length > 0);
        fwrite(lines[i].begin, sizeof(char), lines[i].length, output);
        fputc('\n', output);
    }
}

int ReversedComparison(const void *line_a, const void *line_b) {
    const line_t *a = (const line_t *)line_a;
    const line_t *b = (const line_t *)line_b;
    long line_a_index = (long)(a->length - 1), line_b_index = (long)(b->length - 1);
    for ( ; line_a_index >= 0 && line_b_index >= 0; line_a_index--, line_b_index--) {
        while (line_a_index >= 0 && !isalpha(a->begin[line_a_index])) {
            line_a_index--;
        }
        while (line_b_index >= 0 && !isalpha(b->begin[line_b_index])) {
            line_b_index--;
        }
        if (line_a_index < 0 || line_b_index < 0) {
            break;
        }

        char line_a_symbol = (char)tolower(a->begin[line_a_index]);
        char line_b_symbol = (char)tolower(b->begin[line_b_index]);

        if (line_a_symbol < line_b_symbol) {
            return -1;
        }
        if (line_a_symbol > line_b_symbol) {
            return  1;
        }
    }
    if (line_a_index < 0 && line_b_index < 0) {
        return 0;
    }
    if (line_a_index < 0) {
        return -1;
    }
    return 1;
}

int DirectComparison(const void *line_a, const void *line_b) {
    const line_t *a = (const line_t *)line_a;
    const line_t *b = (const line_t *)line_b;

    for (size_t line_a_index = 0, line_b_index = 0; line_a_index < a->length && line_b_index < b->length; line_a_index++, line_b_index++) {
        while (line_a_index < a->length && !isalpha(a->begin[line_a_index])) {
            line_a_index++;
        }
        while (line_b_index < b->length && !isalpha(b->begin[line_b_index])) {
            line_b_index++;
        }
        if (line_a_index >= a->length || line_b_index >= b->length) {
            break;
        }

        char line_a_symbol = (char)tolower(a->begin[line_a_index]);
        char line_b_symbol = (char)tolower(b->begin[line_b_index]);

        if (line_a_symbol < line_b_symbol) {
            return -1;
        }
        if (line_a_symbol > line_b_symbol) {
            return  1;
        }
    }
    if (a->length < b->length) {
        return -1;
    }
    else if (a->length > b->length) {
        return  1;
    }
    else {
        return 0;
    }
}

void SortAndFillOutputText(line_t *lines, size_t lines_count, char *text_buffer) {
    assert(lines != NULL);
    FILE *output = fopen("output.txt", "w");
    assert(output != NULL);

    qsort(lines, lines_count, sizeof(line_t), DirectComparison);
    WritePointersArray(lines, lines_count, output);

    qsort(lines, lines_count, sizeof(line_t), ReversedComparison);
    WritePointersArray(lines, lines_count, output);

    WriteText(text_buffer, lines_count, output);

    fclose(output);
}

void FillLines(char *text_buffer, line_t *lines, size_t lines_count) { // NOTE: may use getline
    char *pointer_on_symbol = text_buffer;
    char *new_beginning = text_buffer;

    for (size_t i = 0; i < lines_count; i++) {
        pointer_on_symbol = strchr(pointer_on_symbol, '\0');
        lines[i] = { .begin = new_beginning, .length = (size_t)(pointer_on_symbol - new_beginning)};
        new_beginning = pointer_on_symbol + 1;
        pointer_on_symbol++;
    }
}

void CountLinesReplaceBackspaces(char *buffer, size_t *count) {
    char *pointer_on_symbol = buffer;

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

    text->text_buffer = (char*)calloc(text->text_size, sizeof(char));
    assert(text->text_buffer != NULL);

    assert(fread(text->text_buffer, sizeof(char), text->text_size, onegin) == text->text_size);

    CountLinesReplaceBackspaces(text->text_buffer, &(text->lines_count));

    text->lines = (line_t*)calloc(text->lines_count, sizeof(line_t));
    assert(text->lines != NULL);

    FillLines(text->text_buffer, text->lines, text->lines_count);

    fclose(onegin);
}

void DestructText(text_t *text) {
    free(text->text_buffer);
    free(text->lines);
    text->lines_count = 0;
    text->text_size = 0;
}

int main() {
    text_t text = { .lines = NULL, .lines_count = 0, .text_size = 0, .text_buffer = NULL};
    ConstructText(&text);
    SortAndFillOutputText(text.lines, text.lines_count, text.text_buffer);
    DestructText(&text);
    return 0;
}
