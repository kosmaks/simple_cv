#pragma once

#include "matrix.h"

typedef void(*blob_iterator_t)(matrix_t *matrix, unsigned int id);

typedef struct {
  coord_t x, y;
} blob_position_t;

unsigned int find_blobs(matrix_t *matrix, matrix_t *dest);
void extract_blobs(matrix_t *source, blob_iterator_t iterator, unsigned int padding);
void extract_blob_positions(matrix_t *source, blob_position_t *pos, unsigned int max);
