#include <stdlib.h>
#include <stdio.h>

#include "pgm.h"
#define MAX_TERM 24

void skipline(FILE *file) {
  while (!feof(file) && fgetc(file) != '\r') {}
}

void readuntil(FILE *file, char *dest, int max) {
  if (feof(file)) {
    dest[0] = 0;
    return;
  }

  char ch = 0;
  int i = 0;
  while (i < max &&
         ch != ' ' &&
         ch != '\t' &&
         ch != '\r' &&
         ch != '\n') {
    if (ch > 0) { dest[i++] = ch; }
    ch = fgetc(file);
  }
  dest[i] = 0;

  if (i == 0) readuntil(file, dest, max);
}

char pgmread(const char *path, matrix_t *dest) {
  char result[MAX_TERM];
  coord_t x, y, w, h;

  FILE *file = fopen(path, "r");
  if (!file) return 0;

  readuntil(file, result, MAX_TERM); // identifier

  readuntil(file, result, MAX_TERM); // width
  w = atoi(result);
  readuntil(file, result, MAX_TERM); // height
  h = atoi(result);
  readuntil(file, result, MAX_TERM); // max gray
  pixel_t maxgray = atoi(result);

  *dest = new_matrix(w, h);
  dest->width = w;
  dest->height = h;

  for (y = 0; y < dest->height; ++y)
  for (x = 0; x < dest->width; ++x) {
    readuntil(file, result, MAX_TERM);
    dest->bytes[y][x] = 255 * atoi(result) / maxgray;
  }

  fclose(file);
  return 1;
}

void pgmsave(const char *path, matrix_t *src) {
  coord_t x, y;
  int breaker = 1;
  FILE *file = fopen(path, "w");
  fprintf(file, "P2\n");
  fprintf(file, "%d %d 255\n", src->width, src->height);

  for (y = 0; y < src->height; ++y) {
    for (x = 0; x < src->width; ++x) {
      fprintf(file, "%d ", (int)src->bytes[y][x]);
      if (breaker++ % 9 == 0) {
        breaker = 1;
        fputc('\n', file);
      }
    }
  }

  fclose(file);
}
