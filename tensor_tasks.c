#include "tensor_tasks.h"

inline void matmul_cc_task(float* v1, float* v2, float* restrict dst, int depth, int stride_1) {
  for (int i = 0; i < depth; i++) {
    *(dst) += *(v1) * *(v2);
    v1 += stride_1;
    v2++;
  }
}

inline void matmul_rc_task(float* v1, float* v2, float* restrict dst, int depth) {
  for (int i = 0; i < depth; i++) {
    *(dst) += *(v1) * *(v2);
    v1++;
    v2++;
  }
}

inline void matmul_cr_task(float* v1, float* v2, float* restrict dst, int depth, int stride_1, int stride_2) {
  for (int i = 0; i < depth; i++) {
    *(dst) += *(v1) * *(v2);
    v1 += stride_1;
    v2 += stride_2;
  }
}

inline void matmul_rr_task(float* v1, float* v2, float* restrict dst, int depth, int stride_2) {
  for (int i = 0; i < depth; i++) {
    *(dst) += *(v1) * *(v2);
    v1++;
    v2 += stride_2;
  }
}

void task_block_set_type(TaskBlock *tb, TaskType type) {
  tb->task_type = type;
}

void task_block_set_vector_length(TaskBlock *tb, int vlen) {
  tb->vector_length = vlen;
}

void task_block_bind_startpoints(TaskBlock* tb, float *mat1, float *mat2, float *dst) {
  tb->dst_start = dst;
  tb->mat1_start = mat1;
  tb->mat2_start = mat2;
}

void task_block_set_dst_stride(TaskBlock* tb, int stride_x, int stride_y) {
  tb->dst_stride_x = stride_x;
  tb->dst_stride_y = stride_y;
}

void task_block_set_oprand_stride(TaskBlock* tb, int stride_1, int stride_2) {
  tb->mat1_stride = stride_1;
  tb->mat2_stride = stride_2;
}

void task_block_set_block_size(TaskBlock* tb, int block_size_x, int block_size_y) {
  tb->block_size_x = block_size_x;
  tb->block_size_y = block_size_y;
}

void task_block_run(TaskBlock * restrict tb) {
  switch(tb->task_type) {
    case MATMUL_CC:
      for (int i = 0; i < tb->block_size_x; i++) {
        for(int j = 0; j < tb->block_size_y; j++) {
          matmul_cc_task(tb->mat1_start, tb->mat2_start, tb->dst_start, tb->vector_length, tb->mat1_stride);
          tb->dst_start += tb->dst_stride_y;
          tb->mat1_start++;
        }
          tb->mat1_start -= tb->block_size_y;
          tb->mat2_start += tb->mat2_stride;
          tb->dst_start -= tb->dst_stride_y * tb->block_size_y;
          tb->dst_start += tb->dst_stride_x;
      }
      break;
    case MATMUL_CR:

      for (int i = 0; i < tb->block_size_x; i++) {
        for(int j = 0; j < tb->block_size_y; j++) {
          matmul_cr_task(tb->mat1_start, tb->mat2_start, tb->dst_start, tb->vector_length, tb->mat1_stride, tb->mat2_stride);
          tb->dst_start += tb->dst_stride_y;
          tb->mat1_start++;
        }
          tb->mat1_start -= tb->block_size_y;
          tb->mat2_start++;
          tb->dst_start -= tb->dst_stride_y * tb->block_size_y;
          tb->dst_start += tb->dst_stride_x;
      }
      break;
    case MATMUL_RC:
      for (int i = 0; i < tb->block_size_x; i++) {
        for(int j = 0; j < tb->block_size_y; j++) {
          matmul_rc_task(tb->mat1_start, tb->mat2_start, tb->dst_start, tb->vector_length);
          tb->dst_start += tb->dst_stride_y;
          tb->mat1_start += tb->mat1_stride;
        }
          tb->mat1_start -= tb->block_size_y * tb->mat1_stride;
          tb->mat2_start += tb->mat2_stride;
          tb->dst_start -= tb->dst_stride_y * tb->block_size_y;
          tb->dst_start += tb->dst_stride_x;
      }
      break;
    case MATMUL_RR:
      for (int i = 0; i < tb->block_size_x; i++) {
        for(int j = 0; j < tb->block_size_y; j++) {
          matmul_rr_task(tb->mat1_start, tb->mat2_start, tb->dst_start, tb->vector_length, tb->mat2_stride);
          tb->dst_start += tb->dst_stride_y;
          tb->mat1_start += tb->mat1_stride;
        }
          tb->mat1_start -= tb->block_size_y * tb->mat1_stride;
          tb->mat2_start++;
          tb->dst_start -= tb->dst_stride_y * tb->block_size_y;
          tb->dst_start += tb->dst_stride_x;
      }
      break;
  }
}
