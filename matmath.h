#pragma once

#include "matrix.h"

typedef struct {
  int x, y;
  char type; // 0 - min, 1 - max
} local_minmax_t;

void add_matrices(matrix_t *left, matrix_t *right);
void sub_matrices(matrix_t *left, matrix_t *right);

void gradient(matrix_t *source, matrix_t *gx, matrix_t *gy, matrix_t *gxy);

void local_minmax(matrix_t *matrix, local_minmax_t *points, unsigned int maxcount);
void matrix_minmax(matrix_t *matrix, pixel_t *min, pixel_t *max);
pixel_t matrix_square(matrix_t *matrix);
coord_t align_to(coord_t source, coord_t alignment);
