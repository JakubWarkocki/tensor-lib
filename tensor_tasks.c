#include "vectorised_ops.h"
#include "tensor_tasks.h"
#include <immintrin.h>

void task_block_set_type(TaskBlock *tb, TaskType type) {
  tb->task_type = type;
}

void task_block_set_vector_length(TaskBlock *tb, int vlen) {
  tb->task_details.matmul.vector_length = vlen;
}

void task_block_bind_startpoints(TaskBlock* tb, float *mat1, float *mat2, float *dst) {
  tb->dst_start = dst;
  tb->mat1_start = mat1;
  tb->mat2_start = mat2;
}

void task_block_set_dst_stride(TaskBlock* tb, int stride_x, int stride_y) {
  tb->task_details.matmul.dst_stride_x = stride_x;
  tb->task_details.matmul.dst_stride_y = stride_y;
}

void task_block_set_oprand_stride(TaskBlock* tb, int stride_1, int stride_2) {
  tb->task_details.matmul.mat1_stride = stride_1;
  tb->task_details.matmul.mat2_stride = stride_2;
}

void task_block_set_block_size(TaskBlock* tb, int block_size_x, int block_size_y) {
  tb->task_details.matmul.block_size_x = block_size_x;
  tb->task_details.matmul.block_size_y = block_size_y;
}

void task_block_run(TaskBlock * restrict tb) {
  switch(tb->task_type) {
    case MATMUL:
      for (int i = 0; i < tb->task_details.matmul.block_size_x; i++) {
        for(int j = 0; j < tb->task_details.matmul.block_size_y; j++) {
            vector_dot_product(tb->mat1_start, tb->mat2_start, tb->dst_start, tb->task_details.matmul.vector_length);
            tb->dst_start += tb->task_details.matmul.dst_stride_y;
            tb->mat1_start += tb->task_details.matmul.mat1_stride;
          }
          tb->mat1_start -= tb->task_details.matmul.block_size_y * tb->task_details.matmul.mat1_stride;
          tb->mat2_start += tb->task_details.matmul.mat2_stride;
          tb->dst_start -= tb->task_details.matmul.dst_stride_y * tb->task_details.matmul.block_size_y;
          tb->dst_start += tb->task_details.matmul.dst_stride_x;
      }
      break;
    case CONVOLVE:
      break;
    case T_CONVOLVE:
      break;
  }
}
