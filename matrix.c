#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

// MATRIX AND TENSOR STRUCTS

typedef struct {
	int dim_x;
	int dim_y;
	float* data;
} Matrix;

typedef struct {
	int dim_x;
	int dim_y;
	int dim_z;
	Matrix** data;
} Tensor3;

// CONSTRUCTORS AND DESTRUCTORS

Matrix* matrix_create(int x, int y) {
	if( x<=0 || y<=0 ) {
		return NULL;
	}
	Matrix* mat = (Matrix*) malloc(sizeof(Matrix));
	if(!mat) {
		return NULL;
	}

	mat->dim_x = x;
	mat->dim_y = y;
	mat->data = (float*) malloc(x*y*sizeof(float));
	if(!mat->data) {
		free(mat);
		return NULL;
	}
	return mat;
}

void matrix_delete(Matrix* mat){
	free(mat->data);
	free(mat);
}

Tensor3* tensor_create(int x, int y, int z, int init) {
	if( x<=0 || y<=0 || z<=0) {
		return NULL;
	}
	Tensor3* tns = (Tensor3*) malloc(sizeof(Tensor3));
	tns->dim_x = x;
	tns->dim_y = y;
	tns->dim_z = z;
	tns->data = (Matrix**) malloc(z*sizeof(Matrix*));
	if(!tns->data){
		free(tns);
		return NULL;
	}
	if(init){	
		for(int i = 0; i < z; i++){
			tns->data[i] = matrix_create(x, y);
			if(!tns->data[i]) {
				for(int j=0; j<i; j++){
					matrix_delete(tns->data[j]);
					free(tns->data);
					free(tns);
				}
				return NULL;
			}
		}
	}
	return tns;
}

void tensor_delete(Tensor3* tns) {
	for(int i = 0; i < tns->dim_z; i++) {
		matrix_delete(tns->data[i]);
	}
	free(tns->data);
	free(tns);
}

// MACROS FOR ELEMENT ACCESS

#define MATRIX_ELEM(mat, x, y) ((mat->data + x + mat->dim_x*y))
#define TENSOR_ELEM(tns, x, y, z) (MATRIX_ELEM(*(tns->data + z), x, y)

// TYPE SAFE FUNCTIONS FOR ELEMENT ACCESS

float* matrix_elem(Matrix* mat, int x, int y) {
	if(!mat) {
		return NULL;
	}

	if(x < 0 || y < 0 || x >= mat->dim_x || y >= mat->dim_y) {
		return NULL;
	}

	return MATRIX_ELEM(mat, x, y);
}

float matrix_elem_get(Matrix* mat, int x, int y) {
	float* e = matrix_elem(mat, x, y);
	if(!e) {
		return 0;
	}
	return *(e);
}

void matrix_elem_set(Matrix* mat, int x, int y, float val) {
	float* e = matrix_elem(mat, x, y);
	if(!e) {
		return;
	}
	*(e) = val;
	return;
}

float* tensor_elem(Tensor3* tns, int x, int y, int z) {
	if(!tns) {
		return NULL;
	}
	if( z < 0 || z >= tns->dim_z ) {
		return NULL;
	}

	float* e = matrix_elem(tns->data[z], x, y);
	return e;
}

float tensor_elem_get(Tensor3* tns, int x, int y, int z) {
	float* e = tensor_elem(tns, x, y, z);
	if(!e) {
		return 0;
	}
	return *(e);
}

void tensor_elem_set(Tensor3* tns, int x, int y, int z, float val) {
	float* e = tensor_elem(tns, x, y, z);
	if(!e) {
		return;
	}
	*(e) = val;
	return;
}

// UTILITIES

void matrix_print(Matrix* mat) {
	if(!mat) {
		printf("NULL MATRIX\n");
		return;
	}

	for(int i = 0; i < mat->dim_y; i++) {
		for(int j = 0; j < mat->dim_x; j++) {
			printf("%.2e ", *(MATRIX_ELEM(mat, j, i)));	
		}
		printf("\n");
	}
}

void tensor_print(Tensor3* tns) {
	if(!tns) {
		printf("NULL TENSOR\n");
		return;
	}

	for(int i = 0; i < tns->dim_z; i++) {
		matrix_print(tns->data[i]);
		printf("\n");
	}
}

// BASIC MATRIX OPERATIONS

Matrix* matrix_multiply(Matrix* mat1, Matrix* mat2, Matrix* mat) {
	
	if(!mat1 || !mat2 || !mat) {
		return NULL;
	}
	if(mat1->dim_x != mat2->dim_y) {
		return NULL;
	}
	if(mat->dim_x!=mat2->dim_x || mat->dim_y!=mat1->dim_y) {
		return NULL;
	}
	
	int x, y, l;
	x = mat2->dim_x;
	y = mat1->dim_y;
	l = mat1->dim_x;

	for(int i = 0; i < y; i++) {
		for(int j = 0; j < x; j++) {

			//  TODO: turn this code block into a task for threads
			
			*MATRIX_ELEM(mat, j, i) = (float)(0);
			for(int k = 0; k < l; k++){
				*MATRIX_ELEM(mat, j, i) += (*MATRIX_ELEM(mat1, k, i))*(*MATRIX_ELEM(mat2, j, k));
			}
		}
	}
	return mat;
}

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

