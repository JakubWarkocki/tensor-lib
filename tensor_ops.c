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
  
  int mode = 0;
  TaskType task_type;
  TaskBlock tb;

  int mat1_stride, mat2_stride, dst_stride_x, dst_stride_y, result_x, result_y;

  result_x = mat2->dim_x;
  result_y = mat1->dim_y;

  

  if(mat1->orientation == COLUMN_FIRST) {
    mode++;
    mat1_stride = mat1->dim_y;
  }
  else{
    mat1_stride = mat1->dim_x;
  }

  if(mat2->orientation == COLUMN_FIRST) {
    mode += 2;
    mat2_stride = mat2->dim_y;
  }
  else{
    mat2_stride = mat2->dim_x;
  }


  if(mat->orientation == COLUMN_FIRST) {
      dst_stride_x = mat->dim_y;
      dst_stride_y = 1;
  }
  else{
      dst_stride_x = 1;
      dst_stride_y = mat->dim_x;
  }

  switch(mode) {
    case 0:
      task_type = MATMUL_RR;
      break;
    case 1:
      task_type = MATMUL_CR;
      break;
    case 2:
      task_type = MATMUL_RC;
      break;
    case 3:
      task_type = MATMUL_CC;
      break;
  }

  task_block_set_type(&tb, task_type);
  task_block_set_vector_length(&tb, mat1->dim_x);
  task_block_set_oprand_stride(&tb, mat1_stride, mat2_stride);
  task_block_set_dst_stride(&tb, dst_stride_x, dst_stride_y);

 
  thread_pool_run(wtp);
  int i2, j2;
  for (int i = 0; i < result_y; i+=150) {
    for (int j = 0; j < result_x; j+=150) {
      i2 = i + 149;
      j2 = j + 149;
      if(i2 >= result_y) {
        i2 = result_y - 1;
      }
      if(j2 >= result_x) {
        j2 = result_x - 1;
      }
      task_block_set_block_size(&tb, i2 - i +1, j2 - j+1);
      task_block_bind_startpoints(&tb, matrix_elem(mat1, 0, j), matrix_elem(mat2, i, 0), matrix_elem(mat, i, j)); 
      gen_buf_insert_elem(wtp->task_buffer, &tb);
    }
  }
  
  thread_pool_await(wtp);
  return mat;
}
