#include <stdio.h>
#include <math.h>

#include "filters.h"
#include "matmath.h"

float FILTER_EDGES_X[9] = {
  1, 0, -1,
  2, 0, -2,
  1, 0, -1
};

float FILTER_EDGES_Y[9] = {
   1,  2,  1,
   0,  0,  0,
  -1, -2, -1
};

float BOX_FILTER_3[9] = {
  1, 1, 1,
  1, 1, 1,
  1, 1, 1,
};

void gaussian_filter(float *dest, coord_t size, float sigma) {
  int x, y, fx, fy, middle = size >> 1;
  float tss = 2 * pow(sigma, 2);

  for (x = 0; x < size; x++)
  for (y = 0; y < size; y++) {
      fx = x - middle;
      fy = y - middle;
      dest[y * size + x] = (1.0f / sqrt(3.14f * tss)) * 
        exp(-(fx * fx + fy * fy) / tss);
  }
}

void apply_filter(matrix_t *source, matrix_t *dest, float *filter, coord_t size) {
  coord_t x, y, i, j;
  coord_t middle = size >> 1;
  pixel_t color;

  for (y = 0; y < source->height - size; ++y)
  for (x = 0; x < source->width - size; ++x) {
    color = 0;
    for (i = 0; i < size; ++i)
    for (j = 0; j < size; ++j) {
      color += source->bytes[y+i][x+j] * filter[i * size + j];
    }
    dest->bytes[y + middle][x + middle] = color;
  }
}

void find_edges(matrix_t *source, matrix_t *dest) {
  coord_t y, x;
  pixel_t *rowdest, *rowtemp;
  matrix_t temp = new_matrix(source->width, source->height);
  apply_filter(source, dest, FILTER_EDGES_X, FILTER_EDGES_X_SIZE);
  apply_filter(source, &temp, FILTER_EDGES_Y, FILTER_EDGES_Y_SIZE);

  for (y = 0; y < dest->height; ++y) {
    rowdest = dest->bytes[y];
    rowtemp = temp.bytes[y];
    for (x = 0; x < dest->width; ++x) {
      rowdest[x] = sqrt(pow(rowdest[x], 2) + pow(rowtemp[x], 2));
    }
  }
  free_matrix(&temp);
}

void threshold(matrix_t *matrix, float percent, threshold_type type) {
  pixel_t min, max, val = 1, unval = 0;
  coord_t x, y;
  matrix_minmax(matrix, &min, &max);
  max -= min;

  for (x = 0; x < matrix->width; ++x)
  for (y = 0; y < matrix->height; ++y) {
    switch (type) {
      case BINARY: 
        val = matrix->bytes[y][x];
        unval = 0;
        break;
      case INVERT_BINARY:
        val = 0;
        unval = matrix->bytes[y][x];
        break;
      case CUTOFF:
        val = 1;
        unval = 0;
        break;
      case INVERT_CUTOFF:
        val = 0;
        unval = 1;
        break;
    }
    
    matrix->bytes[y][x] = ((100.0f * matrix->bytes[y][x] / max) > percent) ? val : unval;
  }
}

void invert(matrix_t *matrix) {
  pixel_t val;
  coord_t x, y;

  for (y = 0; y < matrix->height; ++y)
  for (x = 0; x < matrix->width; ++x) {
    val = matrix->bytes[y][x];
    matrix->bytes[y][x] = -val;
  }
}

void erode(matrix_t *source, matrix_t *dest, int amount) {
  pixel_t val;
  coord_t x, y;
  int i, j;

  for (y = 0; y < source->height; ++y)
  for (x = 0; x < source->width; ++x) {
    val = source->bytes[y][x];

    for (i = -amount; i <= amount; ++i)
    for (j = -amount; j <= amount; ++j) {
      if ((x == 0 && j < 0) ||
          (y == 0 && i < 0) ||
          x + j >= source->width ||
          y + i >= source->height) continue;
      if (val > 0)
        dest->bytes[y+i][x+j] = val;// + pow(source->bytes[y+i][x+j], 2));
    }
  }
}
