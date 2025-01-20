#include "gen_buf.h"

GenericBuffer* gen_buf_create(int e_size, int cap) {

  if( e_size<=0 || cap<=0 ) {
    return NULL;
  }

  GenericBuffer* gbf = (GenericBuffer*) malloc(sizeof(GenericBuffer));

  if(!gbf) {
    return NULL;
  }

  gbf->data = malloc(e_size*cap);
  
  if(!gbf->data) {
    free(gbf);
    return NULL;
  }

  if (pthread_mutex_init(gbf->access, NULL)) {
    free(gbf);
    free(gbf->data);
    return NULL;
  }
  
  sem_init(gbf->sem_insert, 0, gbf->capacity);
  sem_init(gbf->sem_remove, 0, 0);

  gbf->elem_size = e_size;
  gbf->capacity = cap;
  gbf->head = 0;
  gbf->tail = 0;
  return gbf;
}

void gen_buf_delete(GenericBuffer* gbf) {

  if(!gbf) {
    return;
  }

  pthread_mutex_lock(gbf->access);
  pthread_mutex_destroy(gbf->access);
  
  sem_destroy(gbf->sem_insert);
  sem_destroy(gbf->sem_remove);

  free(gbf->data);
  free(gbf);

}

void gen_buf_insert_elem(GenericBuffer* gbf, void* src) {

  sem_wait(gbf->sem_insert);
  pthread_mutex_lock(gbf->access);

  memcpy(gbf->data + (gbf->elem_size)*(gbf->head), src, gbf->elem_size);
  gbf->head = (gbf->head + 1) % gbf->capacity;

  pthread_mutex_unlock(gbf->access);
  sem_post(gbf->sem_remove);
}

void gen_buf_remove_elem(GenericBuffer* gbf, void* dst) {

  sem_wait(gbf->sem_remove);
  pthread_mutex_lock(gbf->access);

  memcpy(dst, gbf->data + (gbf->elem_size)*(gbf->tail), gbf->elem_size);
  gbf->tail = (gbf->tail + 1) % gbf->capacity;

  pthread_mutex_unlock(gbf->access);
  sem_post(gbf->sem_insert);

}
