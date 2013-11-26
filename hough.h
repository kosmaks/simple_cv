#pragma once

#include "matrix.h"

typedef struct {
  coord_t x, y;
  coord_t radius;
} hough_circle_t;

char hough_circles(matrix_t *source, hough_circle_t *circle, unsigned int threshold);
