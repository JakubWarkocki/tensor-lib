#include "tensor_base.h"
#include "tensor_ops.h"
#include "tensor_threading.h"
#include "tensor_utils.h"

int main(void) {
  
  // Create thread pool
  
  ThreadPool* tp = thread_pool_create(32, 1024);
    
  Matrix *mat1 = matrix_create(3000, 3000, ROW_FIRST);
  Matrix *mat2 = matrix_create(3000, 3000, COLUMN_FIRST);
  Matrix *result = matrix_create(3000, 3000, ROW_FIRST);
  if (!mat1 || !mat2) {
    printf("Failed to create matrices\n");
    return 1;
  }
  printf("Multiplying two 3000x3000 matrices...\n");

  for (int i = 0; i < mat1->dim_y; i++) {
    for (int j = 0; j < mat1->dim_x; j++) {
      matrix_elem_set(mat1, j, i, (float)(1 + i * j));
    }
  }

  for (int i = 0; i < mat2->dim_y; i++) {
    for (int j = 0; j < mat2->dim_x; j++) {
      matrix_elem_set(mat2, j, i, (float)(1 + i + j));
    }
  }

  if (!result) {
    printf("Failed to create result matrix\n");
    return 1;
  }

  struct timespec start, end;
  
  const int n_trials = 1;
  clock_gettime(CLOCK_MONOTONIC, &start);
  
  for(int i = 0; i<n_trials; i++) {
    if (!matrix_multiply(mat1, mat2, result, tp)) {
        printf("Matrix multiplication failed\n");
        return 1;
    } 
  }
  clock_gettime(CLOCK_MONOTONIC, &end);
  
  long seconds = end.tv_sec - start.tv_sec;
  long ms = (seconds * 1000) + (end.tv_nsec - start.tv_nsec) / 1000000;
  printf("Matrix multiplication completed in average time %ld ms\n", ms/n_trials);
  matrix_delete(mat1);
  matrix_delete(mat2);
  matrix_delete(result);

  printf("\nMultiplying two small matrices...\n");

  mat1 = matrix_create(10, 10, ROW_FIRST);
  mat2 = matrix_create(10, 10, COLUMN_FIRST);

  if (!mat1 || !mat2) {
    printf("Failed to create matrices\n");
    return 1;
  }

  for (int i = 0; i < mat1->dim_y; i++) {
    for (int j = 0; j < mat1->dim_x; j++) {
      matrix_elem_set(mat1, j, i, (float)(i));
    }
  }

  for (int i = 0; i < mat2->dim_y; i++) {
    for (int j = 0; j < mat2->dim_x; j++) {
      matrix_elem_set(mat2, j, i, (float)(j+1));
    }
  }


  result = matrix_create(10, 10, ROW_FIRST);

  if (!result) {
    printf("Failed to create result matrix\n");
    return 1;
  }

  // Multiply matrices and store result
  if (!matrix_multiply(mat1, mat2, result, tp)) {
    printf("Matrix multiplication failed\n");
    return 1;
  }

  printf("Matrix 1:\n");
  matrix_print(mat1);

  printf("\nMatrix 2:\n");
  matrix_print(mat2);

  printf("\nResultant Matrix:\n");
  matrix_print(result);


  matrix_delete(mat1);
  matrix_delete(mat2);
  matrix_delete(result);

  thread_pool_delete(tp);


  return 0; 
}
