#ifndef TNS_TASKS_H
#define TNS_TASKS_H

#include "tensor_base.h"

void single_multiplication_task(Matrix* mat1, Matrix* mat2, float* result, int result_x, int result_y);
void single_convolution_task(Matrix* mat, Matrix* filter, float* result, int result_x, int result_y, int stride, int add_padding);

typedef enum {
    MULTIPLICATION_TASK,
    CONVOLUTION_TASK,
    FUNCTION_TASK,
} TaskType;

typedef struct s_task_block {
    TaskType task_type;
    Matrix* mat1;
    Matrix* mat2;
    Matrix* dst_mat;
    int start_x;
    int start_y;
    int end_x;
    int end_y;
    int conv_task_stride;
    int conv_task_add_padding;
    void (*func)(float*);
} TaskBlock;

void task_block_set_type(TaskBlock* tb, TaskType type);
void task_block_bind_matrices(TaskBlock* tb, Matrix* oprand_1, Matrix* oprand_2, Matrix* result);
void task_block_set_x_limits(TaskBlock* tb, int start_x, int end_x);
void task_block_set_y_limits(TaskBlock* tb, int start_y, int end_y);
void task_block_set_conv_params(TaskBlock* tb, int stride, int add_padding);
void task_block_set_function(TaskBlock* tb, void (*fun)(float*));
void task_block_run(TaskBlock* tb);

#endif
