#include <memory.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include "matrix.h"
#include "matmath.h"
#include "filters.h"
#include "pgm.h"
#include "moravec.h"
#include "harris.h"
#include "hough.h"
#include "blobs.h"
#include "contours.h"

int dots_count = 0;
int circles_count = 0;
int lines_count = 0;
int triangles_count = 0;
int rectangles_count = 0;
int unknown_figures = 0;

void each_blob(matrix_t *matrix, unsigned int id) {

  char outf[50], isclosed, circle;
  unsigned int blob_count;
  matrix_t vertices = sameas(matrix);
  matrix_t blobs = sameas(matrix);
  matrix_t dx = sameas(matrix);
  matrix_t contour = aligned_to(matrix, 1);

  // find vertices
  har_detection(matrix, &vertices, 2, 0.1);
  matrix_to_binary(&vertices);
  blob_count = find_blobs(&vertices, &blobs);

  // find contours
  find_contour(matrix, &contour, 1);
  isclosed = is_contour_closed(&contour);
  gradient(matrix, NULL, NULL, &dx);

#ifdef DEBUG
  printf("[%d]\tVertices: %d\tClosed: %s\tFigure: ", id, blob_count, (isclosed) ? "true" : "false");
#else
  printf("[%d]\tFigure: ", id);
#endif

  switch (blob_count) {
    case 0: 
      circle = hough_circles(&contour, NULL, 100);
      if (isclosed && circle) {
        circles_count++;
        printf("Circle\n");
      } else {
        unknown_figures++;
        printf("Unknown figure\n");
      }
      break;
    case 1:
      dots_count++;
      printf("Dot\n");
      break;
    case 2:
      if (isclosed) {
        unknown_figures++;
        printf("Unkonwn figure\n");
      } else {
        lines_count++;
        printf("Line\n");
      }
      break;
    case 3:
      if (isclosed) {
        triangles_count++;
        printf("Triangle\n");
      } else {
        unknown_figures++;
        printf("Unknown figure\n");
      }
      break;
    case 4:
      if (isclosed) {
        rectangles_count++;
        printf("Rectangle\n");
      } else {
        unknown_figures++;
        printf("Unknown figure\n");
      }
      break;
    default:
      unknown_figures++;
      printf("Complex figure\n");
      break;
  }

  normalize(matrix);
  sprintf(outf, "res/blobs/orig_%d.pgm", id);
  pgmsave(outf, matrix);

#ifdef DEBUG

  normalize(&dx);
  sprintf(outf, "res/blobs/dx_%d.pgm", id);
  pgmsave(outf, &dx);

  normalize(&contour);
  sprintf(outf, "res/blobs/contour_%d.pgm", id);
  pgmsave(outf, &contour);

  normalize(&blobs);
  sprintf(outf, "res/blobs/blobs_%d.pgm", id);
  pgmsave(outf, &blobs);

  normalize(&vertices);
  sprintf(outf, "res/blobs/vertices_%d.pgm", id);
  pgmsave(outf, &vertices);

#endif

  free_matrix(&vertices);
  free_matrix(&contour);
  free_matrix(&dx);
  free_matrix(&blobs);
}

int main(int argc, char *argv[]) {
  
  if (argc < 2) {
    printf("Usage: %s <source file>\n", (argc == 1) ? argv[0] : "main");
    return -1;
  }

  matrix_t source, result;

  pgmread(argv[1], &source);
  invert(&source);
  normalize(&source);
  matrix_to_binary(&source);

  unsigned int blobs = find_blobs(&source, &result);
  printf("found %d blobs\n", blobs);
  extract_blobs(&result, each_blob, 10);

  normalize(&source);
  pgmsave("res/out.pgm", &source);

  printf("\nSummary:\n");
  printf("\tDots found: \t\t%d\n", dots_count);
  printf("\tCircles found: \t\t%d\n", circles_count);
  printf("\tLines found: \t\t%d\n", lines_count);
  printf("\tTriangles found: \t%d\n", triangles_count);
  printf("\tRectangles found: \t%d\n", rectangles_count);
  printf("\tUnrecognized figures: \t%d\n", unknown_figures);

  free_matrix(&source);
  free_matrix(&result);
  return 0;
}
