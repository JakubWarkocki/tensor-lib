#ifndef TNS_BASE_H
#define TNS_BASE_H

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
 

// MATRIX ORIENTATION ENUM

typedef enum {
  ROW_FIRST,
  COLUMN_FIRST,
} MatrixOrientation;

// MATRIX AND TENSOR STRUCTS

typedef struct s_matr {
  MatrixOrientation orientation;
  int dim_x;
  int dim_y;
  float *data;
} Matrix;

typedef struct {
  int dim_x;
  int dim_y;
  int dim_z;
  Matrix **data;
} Tensor3;

// CONSTRUCTORS AND DESTRUCTORS

Matrix *matrix_create(int x, int y, MatrixOrientation orientation);
void matrix_delete(Matrix *mat);
Tensor3 *tensor_create(int x, int y, int z, int init);
void tensor_delete(Tensor3 *tns);

// MACROS FOR ELEMENT ACCESS

#define MATRIX_ELEM_ROW_FIRST(mat, x, y) ((mat->data + x + (mat->dim_x * y)))
#define MATRIX_ELEM_COL_FIRST(mat, x, y) ((mat->data + y + (mat->dim_y * x)))

// TYPE SAFE FUNCTIONS FOR ELEMENT ACCESS

float *matrix_elem(Matrix *mat, int x, int y);
float matrix_elem_get(Matrix *mat, int x, int y);
void matrix_elem_set(Matrix *mat, int x, int y, float val);
float *tensor_elem(Tensor3 *tns, int x, int y, int z);
float tensor_elem_get(Tensor3 *tns, int x, int y, int z);
void tensor_elem_set(Tensor3 *tns, int x, int y, int z, float val);

#endif
