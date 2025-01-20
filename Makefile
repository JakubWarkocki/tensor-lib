all: matrix

matrix: matrix.c build/tensor_base build/tensor_ops build/tensor_utils build/gen_buf
	gcc -Wall -o build/matrix matrix.c build/tensor_base build/tensor_ops build/tensor_utils

build/tensor_base: tensor_base.c tensor_base.h
	gcc -Wall -c -O3 -Ofast -o build/tensor_base tensor_base.c

build/tensor_utils: tensor_utils.c tensor_utils.h build/tensor_base
	gcc -Wall -c -O3 -Ofast -o build/tensor_utils tensor_utils.c

build/tensor_ops: tensor_ops.c tensor_ops.h build/tensor_base
	gcc -Wall -c -O3 -Ofast -o build/tensor_ops tensor_ops.c

build/gen_buf: gen_buf.c gen_buf.h
	gcc -Wall -c -pthread -O3 -Ofast -o build/gen_buf gen_buf.c
	
.PHONY: clean all

clean:
	rm -f build/*
