CC      := clang
CFLAGS  := -Wall -Wextra -Wpedantic -O3 -std=gnu11 -pthread -march=native -mtune=native
LDFLAGS := -pthread

BUILD_DIR := build
TARGET    := $(BUILD_DIR)/demo

SOURCES := demo.c tensor_base.c tensor_ops.c tensor_utils.c gen_buf.c tensor_threading.c tensor_tasks.c vectorised_ops.c

OBJS := $(patsubst %.c, $(BUILD_DIR)/%.o, $(SOURCES))

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -rf $(BUILD_DIR)

.PHONY: all clean
