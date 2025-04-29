#include "tensor_threading.h"
#include <pthread.h>

void worker_thread_exit(WorkerThreadArgs* args) {
  pthread_mutex_lock(args->guard);
  (*args->n_stopped_threads)++;
  if(*args->n_stopped_threads == *args->n_threads) {
    pthread_mutex_unlock(args->guard);
    pthread_cond_signal(args->exit_cond);
  }
  else {
  pthread_mutex_unlock(args->guard);
  }
}


void* worker_thread_routine(void* void_args) {
  WorkerThreadArgs* args = (WorkerThreadArgs*) void_args;
  TaskBlock current_task;
  while(1) {
    pthread_barrier_wait(args->await_barrier);
    pthread_mutex_lock(args->guard);
    if(*args->stop_flag) {
      pthread_mutex_unlock(args->guard);
      break;
    }
    pthread_mutex_unlock(args->guard);
    while(gen_buf_remove_elem(args->task_buffer, &current_task)) {
      task_block_run(&current_task);
    }
    pthread_barrier_wait(args->await_barrier);
  }
  worker_thread_exit(args);
  return NULL;
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

  new_tp->task_buffer = gen_buf_create(sizeof(TaskBlock), buffer_capacity);

  if (!new_tp->task_buffer) {
    pthread_barrier_destroy(&new_tp->await_barrier);
    free(new_tp);
    return NULL;
  }


  if (pthread_mutex_init(&new_tp->tp_mutex, NULL)) {
    free(new_tp->threads);
    gen_buf_delete(new_tp->task_buffer);
    pthread_barrier_destroy(&new_tp->await_barrier);
    free(new_tp);
    return NULL;
  }

  if (pthread_cond_init(&new_tp->exit_cond, NULL)) {
    pthread_mutex_destroy(&new_tp->tp_mutex);
    gen_buf_delete(new_tp->task_buffer);
    pthread_barrier_destroy(&new_tp->await_barrier);
    free(new_tp);
    return NULL;
  }

  new_tp->threads = (WorkerThreadArgs*) malloc(max_threads*sizeof(WorkerThreadArgs));

  if (!new_tp->threads) {
    pthread_cond_destroy(&new_tp->exit_cond);
    pthread_mutex_destroy(&new_tp->tp_mutex);
    gen_buf_delete(new_tp->task_buffer);
    pthread_barrier_destroy(&new_tp->await_barrier);
    free(new_tp);
    return NULL;
  }

  for (int i = 0; i<max_threads; i++) {
    (new_tp->threads+i)->task_buffer = new_tp->task_buffer;
    (new_tp->threads+i)->await_barrier = &new_tp->await_barrier;
    (new_tp->threads+i)->guard = &new_tp->tp_mutex;
    (new_tp->threads+i)->stop_flag = &new_tp->stop_flag;
    (new_tp->threads+i)->n_stopped_threads = &new_tp->n_stopped_threads;
    (new_tp->threads+i)->n_threads = &new_tp->n_threads;
    (new_tp->threads+i)->exit_cond = &new_tp->exit_cond;
  }

  for (int i = 0; i < max_threads; i++) {
    if(pthread_create(&((new_tp->threads+i)->tid), NULL, worker_thread_routine, (void*)(new_tp->threads+i))) {
      for(int j = 0; j < i; j++) {
        pthread_cancel((new_tp->threads+j)->tid);
      }
      pthread_cond_destroy(&new_tp->exit_cond);
      pthread_mutex_destroy(&new_tp->tp_mutex);
      gen_buf_delete(new_tp->task_buffer);
      pthread_barrier_destroy(&new_tp->await_barrier);
      free(new_tp->threads);
      free(new_tp);
      return NULL;
    }
  }

  for (int i = 0; i < max_threads; i++) {
    pthread_detach((new_tp->threads+i)->tid);
  }

  return new_tp;
}

void thread_pool_delete(ThreadPool *tp) {

  pthread_mutex_lock(&tp->tp_mutex);
  tp->stop_flag = 1;
  pthread_mutex_unlock(&tp->tp_mutex);
  pthread_barrier_wait(&tp->await_barrier);

  pthread_mutex_lock(&tp->tp_mutex);
  if(tp->n_stopped_threads == tp->n_threads) {
    pthread_mutex_unlock(&tp->tp_mutex);
  }
  else {
    pthread_cond_wait(&tp->exit_cond, &tp->tp_mutex);
    pthread_mutex_unlock(&tp->tp_mutex);
  }

  gen_buf_delete(tp->task_buffer);
  pthread_mutex_destroy(&tp->tp_mutex);
  pthread_cond_destroy(&tp->exit_cond);
  pthread_barrier_destroy(&tp->await_barrier);
  free(tp->threads);
  free(tp);
  
}

void thread_pool_run(ThreadPool *tp) {
  gen_buf_start(tp->task_buffer);
  pthread_barrier_wait(&tp->await_barrier);
}

void thread_pool_await(ThreadPool *tp) {
  gen_buf_stop(tp->task_buffer);
  pthread_barrier_wait(&tp->await_barrier);
}
