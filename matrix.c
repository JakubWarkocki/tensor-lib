#include "tensor_base.h"
#include "tensor_utils.h"
#include "tensor_ops.h"

int main(int argc, char* argv[]) {
    // Create two matrices
    Matrix* mat1 = matrix_create(2, 3);
    Matrix* mat2 = matrix_create(3, 2);

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
    Matrix* result = matrix_create(3, 3);

    if (!result) {
        printf("Failed to create result matrix\n");
        return 1;
    }

    // Multiply matrices and store result
    if (!matrix_multiply(mat1, mat2, result)) {
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

    return 0;
}
