#include "tensor_utils.h"

void matrix_print(Matrix *mat) {
  if (!mat) {
    printf("NULL MATRIX\n");
    return;
  }
  for (int i = 0; i < mat->dim_y; i++) {
    for (int j = 0; j < mat->dim_x; j++) {
      printf("%.2e ", matrix_elem_get(mat, j, i));
    }
    printf("\n");
  }
}

void tensor_print(Tensor3 *tns) {
  if (!tns) {
    printf("NULL TENSOR\n");
    return;
  }

  for (int i = 0; i < tns->dim_z; i++) {
    matrix_print(tns->data[i]);
    printf("\n");
  }
}
