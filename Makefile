all: demo

demo: demo.c build/tensor_base build/tensor_ops build/tensor_utils build/gen_buf build/tensor_threading 
	gcc -Wall -o build/demo demo.c build/tensor_base build/tensor_ops build/tensor_utils build/gen_buf build/tensor_threading build/tensor_tasks

build/tensor_base: tensor_base.c tensor_base.h
	gcc -Wall -c -O3 -Ofast -o build/tensor_base tensor_base.c

build/tensor_utils: tensor_utils.c tensor_utils.h build/tensor_base
	gcc -Wall -c -O3 -Ofast -o build/tensor_utils tensor_utils.c

build/tensor_ops: tensor_ops.c tensor_ops.h build/tensor_base build/tensor_tasks build/tensor_threading
	gcc -Wall -c -O3 -Ofast -o build/tensor_ops tensor_ops.c

build/tensor_tasks: tensor_tasks.c tensor_tasks.h build/tensor_base
	gcc -Wall -c -O3 -Ofast -o build/tensor_tasks tensor_tasks.c

build/gen_buf: gen_buf.c gen_buf.h
	gcc -Wall -c -pthread -O3 -Ofast -o build/gen_buf gen_buf.c

build/tensor_threading: tensor_threading.c tensor_threading.h build/gen_buf build/tensor_tasks
	gcc -Wall -c -pthread -O3 -Ofast -o build/tensor_threading tensor_threading.c
	
.PHONY: clean all

clean:
	rm -f build/*
