#ifndef TNS_TASKS_H
#define TNS_TASKS_H

#include "tensor_base.h"


// TASK TYPES

typedef enum {
    MATMUL,
    CONVOLVE,
    T_CONVOLVE,
} TaskType;

// TASK TYPE SPECIFIC DETAILS

// MATRIX MULTIPLICATION

typedef struct s_matmul_details {
    int vector_length;
    int mat1_stride;
    int mat2_stride;
    int dst_stride_x;
    int dst_stride_y;
    int block_size_x;
    int block_size_y;
} MatMulDetails;

// CONVOLUTION

typedef struct s_convolve_details {
    int vector_length;
    int mat1_stride;
    int mat2_stride;
    int block_length;
} ConvolveDetails;

// TRASPOSE CONVOLUTION

typedef struct s_t_convolve_details {
    int placeholder;
} TConvolveDetails;

// TASK BLOCK

typedef union u_task_details {
    MatMulDetails matmul;
    ConvolveDetails convolve;
    TConvolveDetails t_convolve;
} TaskDetails;

typedef struct s_task_block {
    TaskType task_type;
    float* mat1_start;
    float* mat2_start;
    float* dst_start;
    TaskDetails task_details;
} TaskBlock;

void task_block_set_type(TaskBlock* tb, TaskType type);
void task_block_set_vector_length(TaskBlock* tb, int vlen);
void task_block_bind_startpoints(TaskBlock* tb, float* mat_1, float* mat_2, float* dst);
void task_block_set_dst_stride(TaskBlock* tb, int stride_x, int stride_y);
void task_block_set_oprand_stride(TaskBlock* tb, int stride_1, int stride_2);
void task_block_set_block_size(TaskBlock* tb, int block_size_x, int block_size_y);
void task_block_run(TaskBlock* tb);

#endif
