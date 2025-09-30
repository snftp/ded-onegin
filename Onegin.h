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

void ConstructText(text_t *text);
void DestructText(text_t *text);

void WriteText(char *text_buffer, size_t lines_count, FILE *output);
void WritePointersArray(line_t *lines, size_t lines_count, FILE *output);
int ReversedComparison(const void *line_a, const void *line_b);
int DirectComparison(const void *line_a, const void *line_b);
void SortAndFillOutputText(line_t *lines, size_t lines_count, char *text_buffer);
void FillLines(char *text_buffer, line_t *lines, size_t lines_count);
void CountLinesReplaceBackspaces(char *buffer, size_t *count);
void GetFileLength(size_t *file_size, FILE *file);

#endif
