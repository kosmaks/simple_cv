#pragma once

#include "matrix.h"

char pgmread(const char *path, matrix_t *dest);
void pgmsave(const char *path, matrix_t *source);
