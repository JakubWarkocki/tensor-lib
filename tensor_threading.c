#include "tensor_threading.h"
#include "gen_buf.h"
#include "tensor_tasks.h"
#include <pthread.h>


void* worker_thread_routine(void* void_args) {
  WorkerThreadArgs* args = (WorkerThreadArgs*) void_args;

  TaskBlock current_task;

  while(1) {
    pthread_mutex_lock(args->cond_block);
    pthread_cond_wait(args->start_cond, args->cond_block);
    pthread_mutex_unlock(args->cond_block);
  }

}

ThreadPool* thread_pool_create(int max_threads, int buffer_capacity) {

  ThreadPool* new_tp = (ThreadPool*) malloc(sizeof(ThreadPool));
  if(!new_tp) {
    return NULL;
  }

  new_tp->n_threads = max_threads;

  if (pthread_barrier_init(&new_tp->await_barrier, NULL, max_threads+1)) {
    free(new_tp);
    return NULL;
  }

  if (pthread_cond_init(&new_tp->start_cond, NULL)) {
    pthread_barrier_destroy(&new_tp->await_barrier);
    free(new_tp);
    return NULL;
  }
  
  if (pthread_mutex_init(&new_tp->cond_block, NULL)) {
    pthread_cond_destroy(&new_tp->start_cond);
    pthread_barrier_destroy(&new_tp->await_barrier);
    free(new_tp);
    return NULL;
  }

  new_tp->task_buffer = gen_buf_create(sizeof(TaskBlock), buffer_capacity);

  if (!new_tp->task_buffer) {
    pthread_cond_destroy(&new_tp->start_cond);
    pthread_barrier_destroy(&new_tp->await_barrier);
    pthread_mutex_destroy(&new_tp->cond_block);
    free(new_tp);
    return NULL;
  }

  new_tp->threads = (WorkerThreadArgs*) malloc(max_threads*sizeof(WorkerThreadArgs));

  if (!new_tp->threads) {
    gen_buf_delete(new_tp->task_buffer);
    pthread_cond_destroy(&new_tp->start_cond);
    pthread_barrier_destroy(&new_tp->await_barrier);
    pthread_mutex_destroy(&new_tp->cond_block);
    free(new_tp);
    return NULL;
  }

  for (int i = 0; i<max_threads; i++) {
    (new_tp->threads+i)->task_buffer = new_tp->task_buffer;
    (new_tp->threads+i)->start_cond = &new_tp->start_cond;
    (new_tp->threads+i)->await_barrier = &new_tp->await_barrier;
    (new_tp->threads+i)->cond_block = &new_tp->cond_block;
  }

}
