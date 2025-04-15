#include "tensor_ops.h"
#include "gen_buf.h"
#include "tensor_tasks.h"
#include "tensor_threading.h"
#include <stdio.h>

Matrix* matrix_multiply(Matrix *mat1, Matrix *mat2, Matrix *mat, ThreadPool* wtp) {

  if (!mat1 || !mat2 || !mat || !wtp) {
    return NULL;
  }
  if (mat1->dim_x != mat2->dim_y) {
    return NULL;
  }
  if (mat->dim_x != mat2->dim_x || mat->dim_y != mat1->dim_y) {
    return NULL;
  }
  TaskBlock tb;
  task_block_set_type(&tb, MULTIPLICATION_TASK);
  task_block_bind_matrices(&tb, mat1, mat2, mat);

  int x, y;
  x = mat2->dim_x;
  y = mat1->dim_y;
 
  thread_pool_run(wtp);
  int i2, j2;
  for (int i = 0; i < y; i+=150) {
    for (int j = 0; j < x; j+=150) {
      i2 = i + 99;
      j2 = j + 99;
      if(i2 >= y) {
        i2 = y - 1;
      }
      if(j2 >= x) {
        j2 = x - 1;
      }
      task_block_set_y_limits(&tb, i, i2);
      task_block_set_x_limits(&tb, j, j2);
      gen_buf_insert_elem(wtp->task_buffer, &tb);
    }
  }
  
  thread_pool_await(wtp);
  return mat;
}
