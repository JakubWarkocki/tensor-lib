#include "gen_buf.h"
#include <pthread.h>
#include <semaphore.h>

GenericBuffer *gen_buf_create(int e_size, int cap) {

  if (e_size <= 0 || cap <= 0) {
    return NULL;
  }

  GenericBuffer *gbf = (GenericBuffer *)malloc(sizeof(GenericBuffer));

  if (!gbf) {
    return NULL;
  }

  gbf->data = malloc(e_size * cap);

  if (!gbf->data) {
    free(gbf);
    return NULL;
  }

  if (pthread_mutex_init(&gbf->access, NULL)) {
    free(gbf->data);
    free(gbf);
    return NULL;
  }

  if (pthread_cond_init(&gbf->cond_insert, NULL)){
    pthread_mutex_destroy(&gbf->access);
    free(gbf->data);
    free(gbf);
    return NULL;
  }

  if (pthread_cond_init(&gbf->cond_remove, NULL)) {
    pthread_cond_destroy(&gbf->cond_insert);
    pthread_mutex_destroy(&gbf->access);
    sem_destroy(&gbf->sem_insert);
    free(gbf->data);
    free(gbf);
    return NULL;
  }

  gbf->elem_size = e_size;
  gbf->capacity = cap;
  gbf->count = 0;
  gbf->head = 0;
  gbf->tail = 0;
  gbf->stop_flag = 1;
  return gbf;
}

void gen_buf_delete(GenericBuffer *gbf) {
  if (!gbf) {
    return;
  }
  
  pthread_mutex_lock(&gbf->access);
  pthread_mutex_destroy(&gbf->access);
  pthread_cond_destroy(&gbf->cond_insert);
  pthread_cond_destroy(&gbf->cond_remove);

  free(gbf->data);
  free(gbf);
}

void gen_buf_insert_elem(GenericBuffer *gbf, void *src) {

  pthread_mutex_lock(&gbf->access);
  if(gbf->count == gbf->capacity) {
    pthread_cond_wait(&gbf->cond_insert, &gbf->access);
  }
  memcpy(gbf->data + (gbf->elem_size) * (gbf->head), src, gbf->elem_size);
  gbf->head = (gbf->head + 1) % gbf->capacity;
  if(gbf->count == 1) {
    pthread_cond_signal(&gbf->cond_remove);
  }
  pthread_mutex_unlock(&gbf->access);
}

void gen_buf_remove_elem(GenericBuffer *gbf, void *dst) {
  
  pthread_mutex_lock(&gbf->access);
  if(gbf->count == 0) {
    pthread_cond_wait(&gbf->cond_remove, &gbf->access);
  }

  memcpy(dst, gbf->data + (gbf->elem_size) * (gbf->tail), gbf->elem_size);
  gbf->tail = (gbf->tail + 1) % gbf->capacity;
  
  if(gbf->count == gbf->capacity-1)
  pthread_mutex_unlock(&gbf->access);
}

void gen_buf_start(GenericBuffer *gbf) {
  pthread_mutex_lock(&gbf->access);
  gbf->stop_flag = 0;
  pthread_mutex_unlock(&gbf->access);
}

void gen_buf_stop(GenericBuffer* gbf) {
  pthread_mutex_lock(&gbf->access);
  gbf->stop_flag = 1;
  pthread_mutex_unlock(&gbf->access);
}
