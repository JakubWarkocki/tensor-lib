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
 
  fprintf(stderr, "MAIN THREAD Requesting threads to enter a new thread pool cycle \n");
  thread_pool_run(wtp);

  for (int i = 0; i < y; i++) {
    for (int j = 0; j < x; j++) {
      task_block_set_y_limits(&tb, i, i);
      task_block_set_x_limits(&tb, j, j);
      fprintf(stderr, "MAIN THREAD Scheduling task block %d %d\n", i, j);
      gen_buf_insert_elem(wtp->task_buffer, &tb);
    }
  }
  
  fprintf(stderr, "MAIN THREAD Waiting for thread pool to finish cycle \n");
  thread_pool_await(wtp);
  fprintf(stderr, "MAIN THREAD All threads have finished cycle \n");
  return mat;
}
