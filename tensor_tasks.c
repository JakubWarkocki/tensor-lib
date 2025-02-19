#include "tensor_tasks.h"

void single_multiplication_task(Matrix *mat1, Matrix *mat2, float *result, int *result_x, int *result_y) {
      *result = (float)(0);
      for (int i = 0; i < mat1->dim_x; ++i) {
        *result += (*MATRIX_ELEM(mat1, i, *result_y)) * (*MATRIX_ELEM(mat2, *result_x, i));
      }
}

void single_convolution_task(Matrix *mat, Matrix *filter, float *result, int *result_x, int *result_y, int *stride, int *add_padding) {
  // NOT IMPLEMENTED
  return;
}

void task_block_set_type(TaskBlock *tb, TaskType type) {
  tb->task_type = type;
}

void task_block_bind_matrices(TaskBlock *tb, Matrix *oprand_1, Matrix *oprand_2, Matrix *result) {
  tb->mat1 = oprand_1;
  tb->mat2 = oprand_2;
  tb->dst_mat = result;
}

void task_block_set_function(TaskBlock *tb, void (*fun)(float *)) {
  tb->func = fun;
}

void task_block_set_x_limits(TaskBlock *tb, int start_x, int end_x) {
  tb->start_x = start_x;
  tb->end_x = end_x;
}

void task_block_set_y_limits(TaskBlock *tb, int start_y, int end_y) {
  tb->start_y = start_y;
  tb->end_y = end_y;
}

void task_block_set_conv_params(TaskBlock* tb, int stride, int add_padding) {
  tb->conv_task_stride = stride;
  tb->conv_task_add_padding = add_padding;
}

void do_task_block(TaskBlock *tb) {
  switch(tb->task_type) {
    case MULTIPLICATION_TASK:
      for(int i=tb->start_x; i<=tb->end_x; i++) {
        for(int j=tb->start_y; j<=tb->end_y; j++) {
          single_multiplication_task(tb->mat1, tb->mat2, MATRIX_ELEM(tb->dst_mat, i, j), &i, &j); 
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
