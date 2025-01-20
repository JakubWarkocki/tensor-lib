#include "tensor_base.h"

// CONSTRUCTORS AND DESTRUCTORS

Matrix *matrix_create(int x, int y) {
  if (x <= 0 || y <= 0) {
    return NULL;
  }
  Matrix *mat = (Matrix *)malloc(sizeof(Matrix));
  if (!mat) {
    return NULL;
  }

  mat->dim_x = x;
  mat->dim_y = y;
  mat->data = (float *)malloc(x * y * sizeof(float));
  if (!mat->data) {
    free(mat);
    return NULL;
  }
  return mat;
}

void matrix_delete(Matrix *mat) {
  free(mat->data);
  free(mat);
}

Tensor3 *tensor_create(int x, int y, int z, int init) {
  if (x <= 0 || y <= 0 || z <= 0) {
    return NULL;
  }
  Tensor3 *tns = (Tensor3 *)malloc(sizeof(Tensor3));
  tns->dim_x = x;
  tns->dim_y = y;
  tns->dim_z = z;
  tns->data = (Matrix **)malloc(z * sizeof(Matrix *));
  if (!tns->data) {
    free(tns);
    return NULL;
  }
  if (init) {
    for (int i = 0; i < z; i++) {
      tns->data[i] = matrix_create(x, y);
      if (!tns->data[i]) {
        for (int j = 0; j < i; j++) {
          matrix_delete(tns->data[j]);
          free(tns->data);
          free(tns);
        }
        return NULL;
      }
    }
  }
  return tns;
}

void tensor_delete(Tensor3 *tns) {
  for (int i = 0; i < tns->dim_z; i++) {
    matrix_delete(tns->data[i]);
  }
  free(tns->data);
  free(tns);
}

// TYPE SAFE FUNCTIONS FOR ELEMENT ACCESS

float *matrix_elem(Matrix *mat, int x, int y) {
  if (!mat) {
    return NULL;
  }

  if (x < 0 || y < 0 || x >= mat->dim_x || y >= mat->dim_y) {
    return NULL;
  }

  return MATRIX_ELEM(mat, x, y);
}

float matrix_elem_get(Matrix *mat, int x, int y) {
  float *e = matrix_elem(mat, x, y);
  if (!e) {
    return 0;
  }
  return *(e);
}

void matrix_elem_set(Matrix *mat, int x, int y, float val) {
  float *e = matrix_elem(mat, x, y);
  if (!e) {
    return;
  }
  *(e) = val;
  return;
}

float *tensor_elem(Tensor3 *tns, int x, int y, int z) {
  if (!tns) {
    return NULL;
  }
  if (z < 0 || z >= tns->dim_z) {
    return NULL;
  }

  float *e = matrix_elem(tns->data[z], x, y);
  return e;
}

float tensor_elem_get(Tensor3 *tns, int x, int y, int z) {
  float *e = tensor_elem(tns, x, y, z);
  if (!e) {
    return 0;
  }
  return *(e);
}

void tensor_elem_set(Tensor3 *tns, int x, int y, int z, float val) {
  float *e = tensor_elem(tns, x, y, z);
  if (!e) {
    return;
  }
  *(e) = val;
  return;
}
