#include <stdio.h>
#include "blobs.h"

char blob_finder(matrix_t *matrix, unsigned int fill, coord_t x, coord_t y) {
  int i, j;

  if (x >= matrix->width ||
      y >= matrix->height) return 0;
  if (((int)matrix->bytes[y][x]) != 1) return 0;

  matrix->bytes[y][x] = fill;

  for (i = -1; i <= 1; ++i) 
  for (j = -1; j <= 1; ++j) {
    if ((x == 0 && j == -1) || (y == 0 && i == -1)) continue;
    blob_finder(matrix, fill, x+j, y+i);
  }

  return 1;
}

unsigned int find_blobs(matrix_t *matrix, matrix_t *dest) {
  coord_t x, y;
  unsigned int val, fill = 2;

  matrix_t binary = copy_matrix(matrix);
  matrix_to_binary(&binary);
  dest->width = binary.width;
  dest->height = binary.height;
  dest->bytes = binary.bytes;

  for (y = 0; y < dest->height; ++y) 
  for (x = 0; x < dest->width; ++x) {
    val = dest->bytes[y][x];
    if (val > 0 && val < fill) {
      if (blob_finder(dest, fill, x, y))
        ++fill;
    }
  }

  return fill-2;
}

void extract_blobs(matrix_t *source, blob_iterator_t iterator, unsigned int padding) {
  coord_t x, y, minx, maxx, miny, maxy, w, h;
  matrix_t temp;
  pixel_t val;
  unsigned int id = 1;
  unsigned int found = 0;
  ++padding;

  do {
    minx = source->width;
    miny = source->height;
    maxx = maxy = 0;
    id++;
    found = 0;

    for (y = 0; y < source->height; ++y) 
    for (x = 0; x < source->width; ++x) {
      val = source->bytes[y][x];
      if (val == id) {
        found = val;
        if (x < minx) minx = x;
        if (y < miny) miny = y;
        if (x > maxx) maxx = x;
        if (y > maxy) maxy = y;
      }
    }

    if (found) {
      w = maxx - minx + (padding << 1);
      h = maxy - miny + (padding << 1);
      temp = new_matrix(w, h);

      for (y = 0; y < h; ++y) 
      for (x = 0; x < w; ++x) {
        if (padding > y+miny || 
            padding > x+minx ||
            y+miny-padding >= source->height ||
            x+minx-padding >= source->width) {
          temp.bytes[y][x] = 0;
        } else {
          val = source->bytes[y+miny-padding][x+minx-padding];
          temp.bytes[y][x] = ((int)val == found) ? 1 : 0;
        }
      }
      iterator(&temp, found - 1);
      free_matrix(&temp);
    }
  } while (found);
}

void extract_blob_positions(matrix_t *source, blob_position_t *pos, unsigned int max) {
  coord_t x, y;
  unsigned int found = 0, id = 1;
  blob_position_t *local;

  do {
    found = 0;
    id++;
    for (y = 0; y < source->height; ++y) {
      for (x = 0; x < source->width; ++x) {
        if (((unsigned int)source->bytes[y][x]) == id) {
          found = 1;
          break;
        }
      }
      if (found) break;
    }

    if (found && (id-2 < max)) {
      local = &pos[id-2];
      local->x = x;
      local->y = y;
    }

  } while (found);
}
