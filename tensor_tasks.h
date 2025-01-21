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

typedef struct {
    TaskType tt;
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
} MatrixTaskBlock;

void do_task_block(MatrixTaskBlock* tb);

#endif
