#include "tensor_ops.h"

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
