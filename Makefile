all: matrix

matrix: matrix.c build/tensor_base build/tensor_ops build/tensor_utils
	gcc -Wall -o build/matrix matrix.c build/tensor_base build/tensor_ops build/tensor_utils

build/tensor_base: tensor_base.c tensor_base.h
	gcc -Wall -c -o -O3 -OFast build/tensor_base tensor_base.c

build/tensor_utils: tensor_utils.c tensor_utils.h build/tensor_base
	gcc -Wall -c -o -O3 -OFast build/tensor_utils tensor_utils.c

build/tensor_ops: tensor_ops.c tensor_ops.h build/tensor_base
	gcc -Wall -c -o -O3 -OFast build/tensor_ops tensor_ops.c

.PHONY: clean all

clean:
	rm -f build/*
