#include <stdio.h>
#include <math.h>
#include "harris.h"
#include "matmath.h"
#include "filters.h"

void har_detection(matrix_t *source, matrix_t *dest, float sigma, float k) {
  int kernelsize = 5;
  coord_t y, x;
  float A, B, C;

  matrix_t dx = sameas(source);
  matrix_t dy = sameas(source);
  matrix_t dxy = sameas(source);
  matrix_t gdx = sameas(source);
  matrix_t gdy = sameas(source);
  matrix_t gdxy = sameas(source);
  float kernel[kernelsize * kernelsize];

  gaussian_filter(kernel, kernelsize, sigma);
  gradient(source, &dx, &dy, &dxy);
  apply_filter(&dx, &gdx, kernel, kernelsize);
  apply_filter(&dy, &gdy, kernel, kernelsize);
  apply_filter(&dxy, &gdxy, kernel, kernelsize);

  for (y = 0; y < source->height; ++y)
  for (x = 0; x < source->width; ++x) {
    A = gdx.bytes[y][x];
    B = gdy.bytes[y][x];
    C = gdxy.bytes[y][x];
    dest->bytes[y][x] = (A * B - C * C) - (A + B) * (A + B) * k;
  }

  threshold(dest, 25, CUTOFF);

  free_matrix(&dx);
  free_matrix(&dy);
  free_matrix(&dxy);
  free_matrix(&gdx);
  free_matrix(&gdy);
  free_matrix(&gdxy);
}
