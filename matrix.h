#pragma once

typedef float pixel_t; 
typedef unsigned int coord_t;

typedef struct {
  pixel_t **bytes;
  coord_t width;
  coord_t height;
} matrix_t;

matrix_t new_matrix(coord_t w, coord_t h);
matrix_t copy_matrix(matrix_t *source);
matrix_t sameas(matrix_t *source);
matrix_t aligned_to(matrix_t *source, coord_t alignment);
void free_matrix(matrix_t*);

void print_matrix(matrix_t*);

void matrix_to_binary(matrix_t *matrix);
void normalize(matrix_t *source);

void scale_matrix(matrix_t *source, matrix_t *dest);
void crop_matrix(matrix_t *source, matrix_t *dest, int sx, int sy);
