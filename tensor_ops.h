#ifndef TNS_OPS_H
#define TNS_OPS_H

#include "tensor_base.h"
#include "tensor_threading.h"
#include "tensor_tasks.h"

// HIGH LEVEL OPERATIONS

// ALLOWS TO REORIENT A MATRIX
Matrix* matrix_copy(Matrix *src, Matrix *dst, ThreadPool *wtp);

// NEEDS ALIGNED MATRICES
Matrix* matrix_sum(Matrix *mat1, Matrix *mat2, Matrix *dst, ThreadPool *wtp);

// MAT1 NEEDS TO BE ROW_FIRST AND MAT2 COLUMN_FIRST, DST CAN BE ROW_FIRST OR COLUMN_FIRST
Matrix *matrix_multiply(Matrix *mat1, Matrix *mat2, Matrix *dst, ThreadPool *wtp);


#endif
