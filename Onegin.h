#ifndef ONEGIN_H
#define ONEGIN_H

#include <stddef.h>
#include <stdio.h>


typedef struct {
    char *begin;
    size_t length;
} line_t;

typedef struct {
    line_t *lines;
    size_t lines_count;
    size_t text_size;
    char *text_buffer;
} text_t;

void FillLines(char *text_buffer, line_t *lines, size_t lines_count);
void CountLinesReplaceBackspaces(char *text_buffer, size_t *count);
void GetFileLength(size_t *file_size, FILE *file);
void ConstructText(text_t *text);
// void DestructText(text_t *text);

#endif
