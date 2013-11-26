#pragma once

#include "matrix.h"

void find_contour(matrix_t *source, matrix_t *dest, coord_t alignment);
char is_contour_closed(matrix_t *matrix);
