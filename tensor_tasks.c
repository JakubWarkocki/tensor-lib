#include "tensor_tasks.h"
#include "tensor_base.h"

void single_multiplication_task(Matrix *mat1, Matrix *mat2, float *result, int result_x, int result_y) {
      *result = (float)(0);
      for (int i = 0; i < mat1->dim_x; ++i) {
        *result += (*MATRIX_ELEM(mat1, i, result_y)) * (*MATRIX_ELEM(mat2, result_x, i));
      }
}

void single_convolution_task(Matrix *mat, Matrix *filter, float *result, int result_x, int result_y, int stride, int add_padding) {
  // NOT IMPLEMENTED
  return;
}

void do_task_block(MatrixTaskBlock *tb) {
  switch(tb->tt) {
    case MULTIPLICATION_TASK:
      for(int i=tb->start_x; i<=tb->end_x; i++) {
        for(int j=tb->start_y; j<=tb->end_y; j++) {
          single_multiplication_task(tb->mat1, tb->mat2, MATRIX_ELEM(tb->dst_mat, i, j), i, j); 
        }
      }
      break;
    case CONVOLUTION_TASK:
      break;
    case FUNCTION_TASK:
      for(int i=tb->start_x; i<=tb->end_x; i++) {
        for(int j=tb->start_y; j<=tb->end_y; j++) {
          tb->func(MATRIX_ELEM(tb->mat1, i, j));
        }
      }
      break;
  }
}
