#ifndef TNS_THREAD_H
#define TNS_THREAD_H

#include "gen_buf.h"
#include "tensor_tasks.h"
#include <pthread.h>

typedef struct {
  pthread_t tid;
  GenericBuffer* task_buffer;
  pthread_barrier_t* await_barrier;
  pthread_mutex_t* guard;
  pthread_cond_t* exit_cond;
  int* stop_flag;
  int* n_threads;
  int* n_stopped_threads;
} WorkerThreadArgs;

void* worker_thread_routine(void* args);

typedef struct s_thread_pool {
  int n_threads;
  int n_stopped_threads;
  int stop_flag;
  WorkerThreadArgs* threads;
  GenericBuffer* task_buffer;
  pthread_barrier_t await_barrier;
  pthread_mutex_t tp_mutex;
  pthread_cond_t exit_cond;
} ThreadPool;

ThreadPool* thread_pool_create(int max_threads, int buffer_capacity);
void thread_pool_delete(ThreadPool* tp);
void thread_pool_run(ThreadPool* tp);
void thread_pool_await(ThreadPool* tp);

#endif
