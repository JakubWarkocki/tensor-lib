all: matrix
matrix: matrix.c tensor_base tensor_ops tensor_utils
	gcc -Wall -o build/matrix matrix.c
tensor_base: tensor_base.c tensor_base.h	
	gcc -Wall -c -o build/tensor_base tensor_base.c
tensor_utils: tensor_utils.c tensor_utils.h tensor_base
	gcc -Wall -c -o build/tensor_utils tensor_utils.c
tensor_ops: tensor_ops.c tensor_ops.h tensor_base
	gcc -Wall -c -o build/tensor_ops tensor_ops.c
.PHONY: clean all
clean:
	rm matrix
