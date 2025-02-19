#include "gen_buf.h"
#include "tensor_base.h"
#include "tensor_ops.h"
#include "tensor_threading.h"
#include "tensor_utils.h"

int main(int argc, char *argv[]) {

  // Create thread pool
  
  ThreadPool* tp = thread_pool_create(24, 1024);

  // Create two matrices
  Matrix *mat1 = matrix_create(2, 3);
  Matrix *mat2 = matrix_create(3, 2);

  if (!mat1 || !mat2) {
    printf("Failed to create matrices\n");
    return 1;
  }

  // Populate matrices using safe functions
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

  // Create a result matrix
  Matrix *result = matrix_create(3, 3);

  if (!result) {
    printf("Failed to create result matrix\n");
    return 1;
  }

  // Multiply matrices and store result
  if (!matrix_multiply(mat1, mat2, result, tp)) {
    printf("Matrix multiplication failed\n");
    return 1;
  }

  // Print all three matrices
  printf("Matrix 1:\n");
  matrix_print(mat1);

  printf("\nMatrix 2:\n");
  matrix_print(mat2);

  printf("\nResultant Matrix:\n");
  matrix_print(result);

  // Clean up
  matrix_delete(mat1);
  matrix_delete(mat2);
  matrix_delete(result);
  thread_pool_delete(tp);

  GenericBuffer *intbuf = gen_buf_create(sizeof(int), 100);
  int a;

  for (int j = 0; j < 80; j++) {
    for (int i = 0; i < 10; i++) {
      gen_buf_insert_elem(intbuf, &i);
    }

    for (int i = 0; i < 9; i++) {
      gen_buf_remove_elem(intbuf, &a);
    }
  }
  for (int j = 0; j < 20; j++) {
    gen_buf_remove_elem(intbuf, &a);
    printf("%d ", a);
  }
  gen_buf_delete(intbuf);

  GenericBuffer *charbuf = gen_buf_create(sizeof(char), 100);
  char ch;

  // FILL
  for (int j = 0; j < 80; j++) {
    for (int i = 0; i < 10; i++) {
      ch = 'a' + i % ('z' - 'a' + 1);
      gen_buf_insert_elem(charbuf, &ch);
    }

    for (int i = 0; i < 9; i++) {
      gen_buf_remove_elem(charbuf, &ch);
    }
  }
  for (int j = 0; j < 20; j++) {
    gen_buf_remove_elem(charbuf, &ch);
    printf("%c ", ch);
  }
  gen_buf_delete(charbuf);

  return 0;
}
