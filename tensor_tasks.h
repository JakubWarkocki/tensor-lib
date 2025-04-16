#ifndef TNS_TASKS_H
#define TNS_TASKS_H

#include "tensor_base.h"


// TASK TYPES WITH SINGULAR FUNCTIONS

typedef enum {
    MATMUL_CC,
    MATMUL_CR,
    MATMUL_RC,
    MATMUL_RR
} TaskType;

// DOT PRODUCT CALCULATIONS FOR DIFFERENTLY ORIENTED MATRICES

void matmul_cc_task(float* v1, float* v2, float* dst, int depth, int stride_1);
void matmul_rc_task(float* v1, float* v2, float* dst, int depth);
void matmul_cr_task(float* v1, float* v2, float* dst, int depth, int stride_1, int stride_2);
void matmul_rr_task(float* v1, float* v2, float* dst, int depth, int stride_2);

// TASK BLOCKS FOR WORKER THREADS

typedef struct s_task_block {
    TaskType task_type;
    float* mat1_start;
    float* mat2_start;
    float* dst_start;
    int vector_length;
    int mat1_stride;
    int mat2_stride;
    int dst_stride_x;
    int dst_stride_y;
    int block_size_x;
    int block_size_y;
} TaskBlock;

void task_block_set_type(TaskBlock* tb, TaskType type);
void task_block_set_vector_length(TaskBlock* tb, int vlen);
void task_block_bind_startpoints(TaskBlock* tb, float* mat_1, float* mat_2, float* dst);
void task_block_set_dst_stride(TaskBlock* tb, int stride_x, int stride_y);
void task_block_set_oprand_stride(TaskBlock* tb, int stride_1, int stride_2);
void task_block_set_block_size(TaskBlock* tb, int block_size_x, int block_size_y);
void task_block_run(TaskBlock* tb);

#endif
