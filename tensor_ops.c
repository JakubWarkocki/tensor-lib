#include "tensor_ops.h"
#include "gen_buf.h"
#include "tensor_tasks.h"
#include "tensor_threading.h"

Matrix* matrix_multiply(Matrix *mat1, Matrix *mat2, Matrix *dst, ThreadPool* wtp) {

  if (!mat1 || !mat2 || !dst || !wtp) {
    return NULL;
  }
  if (mat1->dim_x != mat2->dim_y) {
    return NULL;
  }
  if (dst->dim_x != mat2->dim_x || dst->dim_y != mat1->dim_y) {
    return NULL;
  }
  if(mat1->orientation == COLUMN_FIRST && mat2->orientation == ROW_FIRST) {
    return NULL;
  }

  TaskBlock tb;

  int mat1_stride, mat2_stride, dst_stride_x, dst_stride_y, result_x, result_y;

  result_x = mat2->dim_x;
  result_y = mat1->dim_y;

  mat1_stride = mat1->dim_x;
  mat2_stride = mat2->dim_y;

  if(dst->orientation == COLUMN_FIRST) {
      dst_stride_x = dst->dim_y;
      dst_stride_y = 1;
  }
  else{
      dst_stride_x = 1;
      dst_stride_y = dst->dim_x;
  }

  task_block_set_type(&tb, MATMUL);
  task_block_set_vector_length(&tb, mat1->dim_x);
  task_block_set_oprand_stride(&tb, mat1_stride, mat2_stride);
  task_block_set_dst_stride(&tb, dst_stride_x, dst_stride_y);

 
  const int block_size = 32;

  thread_pool_run(wtp);
  int i2, j2;
  for (int i = 0; i < result_y; i+=block_size) {
    for (int j = 0; j < result_x; j+=block_size) {
      i2 = i + block_size - 1;
      j2 = j + block_size - 1;
      if(i2 >= result_y) {
        i2 = result_y - 1;
      }
      if(j2 >= result_x) {
        j2 = result_x - 1;
      }
      task_block_set_block_size(&tb, i2 - i +1, i2 - i+1);
      task_block_bind_startpoints(&tb, matrix_elem(mat1, 0, i), matrix_elem(mat2, j, 0), matrix_elem(dst, j, i)); 
      gen_buf_insert_elem(wtp->task_buffer, &tb);
    }
  }
  
  thread_pool_await(wtp);
  return dst;
}

