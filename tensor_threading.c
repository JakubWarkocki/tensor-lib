#include "tensor_threading.h"
#include <pthread.h>

ThreadPool* thread_pool_create(int max_threads) {

  ThreadPool* new_tp = (ThreadPool*) malloc(sizeof(ThreadPool));
  if(!new_tp) {
    return NULL;
  }

  new_tp->n_threads = max_threads;
  
  new_tp->await_barrier = (pthread_barrier_t*) malloc(sizeof(pthread_barrier_t));

  if (!new_tp->await_barrier) {
    free(new_tp);
    return NULL;
  }

  if (pthread_barrier_init(new_tp->await_barrier, NULL, max_threads+1)) {
    free(new_tp->await_barrier);
    free(new_tp);
    return NULL;
  }
  
  new_tp->start_cond = (pthread_cond_t*) malloc(sizeof(pthread_cond_t));

  if (!new_tp->start_cond) {
    pthread_barrier_destroy(new_tp->await_barrier);
    free(new_tp->await_barrier);
    free(new_tp);
    return NULL;
  }

  if (pthread_cond_init(new_tp->start_cond, NULL)) {
    pthread_barrier_destroy(new_tp->await_barrier);
    free(new_tp->start_cond);
    free(new_tp->await_barrier);
    free(new_tp);
    return NULL;
  }

  new_tp->cond_block = (pthread_mutex_t*) malloc(sizeof(pthread_mutex_t));

  if (!new_tp->cond_block) {
    pthread_cond_destroy(new_tp->start_cond);
    pthread_barrier_destroy(new_tp->await_barrier);
    free(new_tp->start_cond);
    free(new_tp->await_barrier);
    free(new_tp);
    return NULL;
  }
  
  if (pthread_mutex_init(new_tp->cond_block, NULL)) {
    pthread_cond_destroy(new_tp->start_cond);
    pthread_barrier_destroy(new_tp->await_barrier);
    free(new_tp->cond_block);
    free(new_tp->start_cond);
    free(new_tp->await_barrier);
    free(new_tp);
    return NULL;
  }

  new_tp->tid;
}
