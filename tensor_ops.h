#ifndef TNS_OPS_H
#define TNS_OPS_H

#include "tensor_base.h"
#include "tensor_threading.h"

Matrix *matrix_multiply(Matrix *mat1, Matrix *mat2, Matrix *mat, ThreadPool *working_thread_pool);

#endif
