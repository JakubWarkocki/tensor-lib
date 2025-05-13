#ifndef GEN_BUF_H
#define GEN_BUF_H

#include <pthread.h>
#include <stdlib.h>
#include <string.h>

// GENERIC CIRCULAR BUFFER DEDICATED FOR SINGLE PRODUCER MULTIPLE CONSUMER MODEL

typedef struct s_generic_buffer {
  pthread_mutex_t access;
  pthread_cond_t cond_insert;
  pthread_cond_t cond_remove;
  size_t elem_size;
  size_t capacity;
  size_t count;
  int head;
  int tail;
  int stop_flag;
  void *data;
} GenericBuffer;

// CONSTRUCTORS AND DESTRUCTORS

GenericBuffer *gen_buf_create(int e_size, int cap);
void gen_buf_delete(GenericBuffer *gbf);

// INSERTING AND REMOVING ELEMENTS
void gen_buf_insert_elem(GenericBuffer *gbf, void *src);

// RETURNS 1 ON SUCCESS, 0 WHEN BUFFER IS EMPTY AND STOP_FLAG IS SET
int gen_buf_remove_elem(GenericBuffer *gbf, void *dst);

// STARTING AND STOPPING THE BUFFER, STOPPING WAKES UP ALL THREADS WAITING FOR ELEMENTS 
void gen_buf_start(GenericBuffer *gbf);
void gen_buf_stop(GenericBuffer *gbf);

#endif
