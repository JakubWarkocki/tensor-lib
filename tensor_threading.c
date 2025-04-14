#include "tensor_threading.h"
#include <pthread.h>
#include <stdio.h>

void* worker_thread_routine(void* void_args) {
  WorkerThreadArgs* args = (WorkerThreadArgs*) void_args;
  TaskBlock current_task;
  while(1) {
    pthread_barrier_wait(args->await_barrier);
    pthread_mutex_lock(args->cond_block);
    fprintf(stderr, "%lu Waiting for request to enter a new thread pool cycle  \n", args->tid);
    pthread_cond_wait(args->start_cond, args->cond_block);
    fprintf(stderr, "%lu Entered thread pool cycle \n", args->tid);
    pthread_mutex_unlock(args->cond_block);
    while(gen_buf_remove_elem(args->task_buffer, &current_task)) {
      fprintf(stderr, "%lu Running a task block \n", args->tid);
      task_block_run(&current_task);
      fprintf(stderr, "%lu Finished a task block \n", args->tid);
    }
    fprintf(stderr, "%lu Finished thread pool cycle \n", args->tid);
    pthread_barrier_wait(args->await_barrier);
  }

}

ThreadPool* thread_pool_create(int max_threads, int buffer_capacity) {

  ThreadPool* new_tp = (ThreadPool*) malloc(sizeof(ThreadPool));
  if(!new_tp) {
    return NULL;
  }
  
  new_tp->n_threads = max_threads;
  new_tp->is_running = 0;

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

  for (int i = 0; i < max_threads; i++) {
    if(pthread_create(&((new_tp->threads+i)->tid), NULL, worker_thread_routine, (void*)(new_tp->threads+i))) {
      for(int j = 0; j < i; j++) {
        pthread_cancel((new_tp->threads+i)->tid);
      }
      gen_buf_delete(new_tp->task_buffer);
      pthread_cond_destroy(&new_tp->start_cond);
      pthread_barrier_destroy(&new_tp->await_barrier);
      pthread_mutex_destroy(&new_tp->cond_block);
      free(new_tp->threads);
      free(new_tp);
      return NULL;
    }
    pthread_detach((new_tp->threads+i)->tid);
  }
  return new_tp;
}

void thread_pool_delete(ThreadPool *tp) {
  pthread_mutex_lock(&tp->cond_block);
  gen_buf_stop(tp->task_buffer);
  for(int i = 0; i < tp->n_threads; i++) {
    pthread_cancel((tp->threads+i)->tid);
  }
  gen_buf_delete(tp->task_buffer);
  pthread_cond_destroy(&tp->start_cond);
  pthread_barrier_destroy(&tp->await_barrier);
  pthread_mutex_destroy(&tp->cond_block);
  free(tp->threads);
  free(tp);
}

void thread_pool_run(ThreadPool *tp) {
  pthread_mutex_lock(&tp->cond_block);
  gen_buf_start(tp->task_buffer);
  pthread_mutex_unlock(&tp->cond_block);
  pthread_barrier_wait(&tp->await_barrier);
  pthread_cond_broadcast(&tp->start_cond);
}

void thread_pool_await(ThreadPool *tp) {
  pthread_mutex_lock(&tp->cond_block);
  gen_buf_stop(tp->task_buffer);
  pthread_mutex_unlock(&tp->cond_block);
  pthread_barrier_wait(&tp->await_barrier);
}
