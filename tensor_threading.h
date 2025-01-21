#ifndef TNS_THREAD_H
#define TNS_THREAD_H

#include "gen_buf.h"
#include "tensor_tasks.h"
#include <pthread.h>

typedef struct {
  GenericBuffer* task_buffer;
  pthread_cond_t* start_cond;
} WorkerThreadArgs;

typedef struct {
  int n_threads;
  pthread_mutex_t* cond_block;
  pthread_cond_t* start_cond;
  pthread_barrier_t* await_barrier;
} ThreadPool;

ThreadPool* thread_pool_create(int n_thr);
void thread_pool_delete(ThreadPool* tp);
void thread_pool_start(ThreadPool* tp);
void thread_pool_stop(ThreadPool* tp);
void thread_pool_await(ThreadPool* tp);

#endif
