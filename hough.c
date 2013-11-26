#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "hough.h"

#define MAX_HOUGH_RADIUS (400)

char hough_circles(matrix_t *source, hough_circle_t *circle, unsigned int threshold) {
  int y, x, b, a, R, val,
      max = 0, maxx = 0, maxy = 0, maxr = 0;
  int pixel;
  char found;
  coord_t *H = malloc(source->width * source->height * MAX_HOUGH_RADIUS * sizeof(coord_t));

  for (y = 0; y < source->height; ++y)
  for (x = 0; x < source->width; ++x) {
    pixel = source->bytes[y][x];
    if (pixel == 0) continue;

    for (b = 0; b < source->height; ++b)
    for (a = 0; a < source->width; ++a) {
      R = sqrt(pow(((int)x) - a, 2) + pow(((int)y) - b, 2));
      if (R >= MAX_HOUGH_RADIUS) continue;
      H[R * source->height * source->width + b * source->width + a]++;
    }
  }

  for (R = 0; R < MAX_HOUGH_RADIUS; ++R)
  for (y = 0; y < source->height; ++y)
  for (x = 0; x < source->width; ++x) {
    val = H[R * source->height * source->width + y * source->width + x];
    if (val > max) {
      max = val;
      maxx = x;
      maxy = y;
      maxr = R;
    }
  }

  found = max >= threshold;
  if (found && circle) {
    circle->x = maxx;
    circle->y = maxy;
    circle->radius = maxr;
  }

  free(H);
  return found;
}
