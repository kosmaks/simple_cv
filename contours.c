#include <stdio.h>
#include "contours.h"
#include "matmath.h"

void find_contour(matrix_t *source, matrix_t *dest, coord_t alignment) {
  coord_t x, y;
  int i, j;
  unsigned int val;
  char near;

  for (y = 1; y < source->height - 1; ++y)
  for (x = 1; x < source->width - 1; ++x) {
    near = 0;
    val = source->bytes[y][x];
    if (!val) continue;

    for (i = -1; i <= 1; ++i) {
      for (j = -1; j <= 1; ++j) {
        val = source->bytes[y+i][x+j];
        if (val) continue;
        near = 1;
        break;
      }
      if (near) break;
    }

    dest->bytes[align_to(y, alignment)][align_to(x, alignment)] = near;
  }
}

void clear_contour(matrix_t *matrix, coord_t x, coord_t y, pixel_t weight) {
  if (x >= matrix->width ||
      y >= matrix->height) return;

  unsigned int val = matrix->bytes[y][x];
  if (val != 1) return;

  matrix->bytes[y][x] = weight;

  int i, j;
  for (i = -1; i <= 1; ++i)
  for (j = -1; j <= 1; ++j) {
    if ((i != 0 && j != 0)  || 
        (i == 0 && j == 0)  ||
        (i == -1 && y == 0) ||
        (j == -1 && x == 0)) continue;
    clear_contour(matrix, x+j, y+i, weight + 1);
  }

  for (i = -1; i <= 1; ++i)
  for (j = -1; j <= 1; ++j) {
    if ((i == 0 || j == 0)  || 
        (i == 0 && j == 0)  ||
        (i == -1 && y == 0) ||
        (j == -1 && x == 0)) continue;
    clear_contour(matrix, x+j, y+i, weight + 1);
  }
}

char is_contour_closed(matrix_t *matrix) {
  coord_t x, y;
  pixel_t fmin, fmax;
  unsigned int min = 2, max, val;
  int i, j;

  char closed = 0, br = 0;
  matrix_t temp = copy_matrix(matrix);
  /*matrix_t temp = *matrix;*/

  for (y = 0; y < temp.height; ++y) {
    for (x = 0; x < temp.width; ++x) {
      if (temp.bytes[y][x] > 0) {
        clear_contour(&temp, x, y, min);
        br = 1;
        break;
      }
    }
    if (br) break;
  }
  if (!br) return 0;

  matrix_minmax(&temp, &fmin, &fmax);
  max = fmax;

  for (y = 0; y < temp.height; ++y) {
    for (x = 0; x < temp.width; ++x) {
      val = temp.bytes[y][x];
      if (val >= max) {
        for (i = -1; i <= 1; ++i) {
          for (j = -1; j <= 1; ++j) {
            if (i == 0 && j == 0) continue;
            if ((i == -1 && y == 0) ||
                (j == -1 && x == 0) ||
                y + i >= temp.height ||
                x + j >= temp.width) continue;

            val = temp.bytes[y+i][x+j];
            if (val >= min && val <= min+2) {
              closed = 1;
              break;
            }
          }
          if (closed) break;
        }

      }
      if (closed) break;
    }
    if (closed) break;
  }

  free_matrix(&temp);
  return closed;
}
