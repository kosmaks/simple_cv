#pragma once

#include "matrix.h"

extern float FILTER_EDGES_X[9];
#define FILTER_EDGES_X_SIZE 3

extern float FILTER_EDGES_Y[9];
#define FILTER_EDGES_Y_SIZE 3

extern float BOX_FILTER_3[9];

typedef enum {
  BINARY,
  INVERT_BINARY,
  CUTOFF,
  INVERT_CUTOFF
} threshold_type;

void gaussian_filter(float *dest, coord_t size, float sigma);
void find_edges(matrix_t *source, matrix_t *dest);

void apply_filter(matrix_t *source, matrix_t *dest, float *filter, coord_t size);
void threshold(matrix_t *matrix, float percent, threshold_type type);
void invert(matrix_t *matrix);
void erode(matrix_t *source, matrix_t *dest, int amount);
