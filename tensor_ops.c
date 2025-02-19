#include "tensor_ops.h"
#include "gen_buf.h"
#include "tensor_tasks.h"
#include "tensor_threading.h"

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

  int x, y, l;
  x = mat2->dim_x;
  y = mat1->dim_y;
  l = mat1->dim_x;
  
  thread_pool_run(wtp);

  for (int i = 0; i < y; i++) {
    for (int j = 0; j < x; j++) {

      //  TODO: turn this code block into a task for threads
      task_block_set_y_limits(&tb, i, i);
      task_block_set_x_limits(&tb, j, j);
      gen_buf_insert_elem(wtp->task_buffer, &tb);
      }
    }

  thread_pool_await(wtp);
  return mat;
}
