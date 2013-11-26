#include <stdio.h>
#include <math.h>
#include "moravec.h"

void mrv_probabilitymap(matrix_t *source, matrix_t *dest, coord_t size) {
  coord_t x, y;
  int i, j, a, b, half = size >> 1;
  pixel_t min = 0, V;
  char minset = 0;

  for (y = size; y < source->height - size; ++y)
  for (x = size; x < source->width  - size; ++x) {
    minset = 0;

    for (i = -half; i < half; ++i)
    for (j = -half; j < half; ++j) {
      if (i == 0 && j == 0) continue;
      V = 0;

      for (a = -half; a < half; ++a)
      for (b = -half; b < half; ++b) {
        if (a == 0 && b == 0) continue;
        V += pow(source->bytes[y+a][x+b] - 
            source->bytes[y+a+i][x+b+j], 2);
      }

      if (!minset || V < min) {
        minset = 1;
        min = V;
      }
    }

    dest->bytes[y][x] = min;
  }
}
