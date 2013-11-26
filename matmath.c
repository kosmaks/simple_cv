#include <stdio.h>
#include <math.h>
#include "matmath.h"

void gradient(matrix_t *source, matrix_t *gx, matrix_t *gy, matrix_t *gxy) {
  coord_t x, y;
  pixel_t k, v;

  for (y = 1; y < source->height - 1; ++y)
  for (x = 1; x < source->width - 1; ++x) {
    k = source->bytes[y][x+1] -
        source->bytes[y][x-1];
    v = source->bytes[y+1][x] -
        source->bytes[y-1][x];
    if (gx) gx->bytes[y][x] = k * k;
    if (gy) gy->bytes[y][x] = v * v;
    if (gxy) gxy->bytes[y][x] = k * v;
  }
}

void add_matrices(matrix_t *left, matrix_t *right) {
  coord_t y, x;
  for (y = 0; y < left->height; ++y)
  for (x = 0; x < left->width; ++x) {
    right->bytes[y][x] += left->bytes[y][x];
  }
}
 
void sub_matrices(matrix_t *left, matrix_t *right) {
  coord_t y, x;
  for (y = 0; y < left->height; ++y)
  for (x = 0; x < left->width; ++x) {
    right->bytes[y][x] -= left->bytes[y][x];
  }
}

void local_minmax(matrix_t *matrix, local_minmax_t *points, unsigned int maxcount) {
  coord_t x, y, i, j;
  pixel_t neighbour, current;
  char ismax, ismin;
  unsigned int ptrc = 0;
  local_minmax_t *ptr;

  for (y = 1; y < matrix->height - 1; ++y)
  for (x = 1; x < matrix->width - 1; ++x) {
    ismax = ismin = 1;
    current = matrix->bytes[y][x];

    for (i = -1; i <= 1; ++i)
    for (j = -1; j <= 1; ++j) {
      if (i == 0 && j == 0) continue;
      neighbour = matrix->bytes[y+i][x+j];
      if (neighbour <= current) ismin = 0;
      if (neighbour >= current) ismax = 0;
    }

    if (ptrc < maxcount && (ismin || ismax)) {
      ptr = &points[ptrc++];
      ptr->x = x;
      ptr->y = y;
      ptr->type = (ismin) ? 0 : 1;
    }
  }
}

void matrix_minmax(matrix_t *matrix, pixel_t *min, pixel_t *max) {
  coord_t x, y;
  char minset = 0;
  char maxset = 0;

  for (y = 0; y < matrix->height; ++y) 
  for (x = 0; x < matrix->width; ++x) 
    if (!maxset || matrix->bytes[y][x] > *max) {
      maxset = 1;
      *max = matrix->bytes[y][x]; 
    }

  for (y = 0; y < matrix->height; ++y) 
  for (x = 0; x < matrix->width; ++x) 
    if (!minset || matrix->bytes[y][x] < *min) {
      minset = 1;
      *min = matrix->bytes[y][x];
    }
}

pixel_t matrix_square(matrix_t *matrix) {
  coord_t x, y;
  pixel_t *row, square = 0;

  for (y = 0; y < matrix->height; ++y) {
    row = matrix->bytes[y];
    for (x = 0; x < matrix->width; ++x) {
      if (((int)row[x]) > 0) square += 1;
    }
  }

  return square;
}

coord_t align_to(coord_t source, coord_t alignment) {
  return round(source / alignment);
}
