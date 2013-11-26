#include <memory.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include "filters.h"
#include "matrix.h"
#include "matmath.h"

int get_index(matrix_t* matrix, int x, int y) {
  return y * matrix->height + x;
}

matrix_t new_matrix(coord_t w, coord_t h) {
  coord_t y;
  matrix_t matrix;
  matrix.width = w;
  matrix.height = h;
  matrix.bytes = calloc(h, sizeof(pixel_t*));
  for (y = 0; y < h; ++y) matrix.bytes[y] = calloc(w, sizeof(pixel_t));
  return matrix;
}

matrix_t copy_matrix(matrix_t *source) {
  matrix_t mat = new_matrix(source->width, source->height);
  coord_t x, y;

  for (y = 0; y < source->height; ++y)
  for (x = 0; x < source->width; ++x) {
    mat.bytes[y][x] = source->bytes[y][x];
  }

  return mat;
}

matrix_t sameas(matrix_t *source) {
  return new_matrix(source->width, source->height);
}

matrix_t aligned_to(matrix_t *source, coord_t alignment) {
  return new_matrix(align_to(source->width, alignment),
      align_to(source->height, alignment));
}

void free_matrix(matrix_t *matrix) {
  coord_t y;
  for (y = 0; y < matrix->height; ++y) free(matrix->bytes[y]);
  free(matrix->bytes);
}

void print_matrix(matrix_t *matrix) {
  coord_t x, y;
  pixel_t **bytes = matrix->bytes;
  
  for (y = 0; y < matrix->height; ++y) {
    for (x = 0; x < matrix->width; ++x) {
      printf("%f\t", bytes[y][x]);
    }
    putchar('\n');
  }
}

void matrix_to_binary(matrix_t *matrix) {
  coord_t x, y;
  pixel_t val;

  for (y = 0; y < matrix->height; ++y)
  for (x = 0; x < matrix->width; ++x) {
    val = matrix->bytes[y][x];
    matrix->bytes[y][x] = (val > 0) ? 1 : 0;
  }
}

void normalize(matrix_t *matrix) {
  coord_t x, y;
  pixel_t val, min, max;

  matrix_minmax(matrix, &min, &max);
  max -= min;
  if (max == 0) return;

  for (y = 0; y < matrix->height; ++y)
  for (x = 0; x < matrix->width; ++x) {
    val = matrix->bytes[y][x];
    val = 255 * (val - min) / max;
    matrix->bytes[y][x] = val;
  }
}

void scale_matrix(matrix_t *source, matrix_t *dest) {
  coord_t x, y, index, ss = source->width * source->height;
  coord_t scaleX = ((float)source->width)  / dest->width;
  coord_t scaleY = ((float)source->height) / dest->height;

  for (y = 0; y < dest->height; ++y)
  for (x = 0; x < dest->width; ++x) {
    index = round(y * scaleY) * source->width + (x * scaleX);
    dest->bytes[y * dest->width + x] = source->bytes[(index > ss) ? ss - 1 : index];
  }
}

void crop_matrix(matrix_t *source, matrix_t *dest, int sx, int sy) {
  coord_t x, y;
  for (y = 0; y < dest->height; ++y)
  for (x = 0; x < dest->width; ++x) {
    dest->bytes[y][x] = source->bytes[y+sy][x+sx];
  }
}
