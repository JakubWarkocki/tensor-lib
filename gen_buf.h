#ifndef GEN_BUF_H
#define GEN_BUF_H

#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <semaphore.h>

typedef struct {
  pthread_mutex_t* access;
  sem_t* sem_insert;
  sem_t* sem_remove;
  size_t elem_size;
  size_t capacity;
  int head;
  int tail;
  void* data;
} GenericBuffer;

GenericBuffer* gen_buf_create(int e_size, int cap);
void gen_buf_delete(GenericBuffer* gbf);
void gen_buf_insert_elem(GenericBuffer* gbf, void* src);
void gen_buf_remove_elem(GenericBuffer* gbf, void* dst);

#endif
