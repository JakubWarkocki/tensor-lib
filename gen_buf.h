#ifndef GEN_BUF_H
#define GEN_BUF_H

#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  pthread_mutex_t access;
  pthread_cond_t cond_insert;
  pthread_cond_t cond_remove;
  size_t elem_size;
  size_t capacity;
  int count;
  int head;
  int tail;
  int stop_flag;
  void *data;
} GenericBuffer;

GenericBuffer *gen_buf_create(int e_size, int cap);
void gen_buf_delete(GenericBuffer *gbf);
void gen_buf_insert_elem(GenericBuffer *gbf, void *src);
void gen_buf_remove_elem(GenericBuffer *gbf, void *dst);
void gen_buf_start(GenericBuffer *gbf);
void gen_buf_stop(GenericBuffer *gbf);

#endif
