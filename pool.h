#ifndef __POOL__
#define __POOL__

#include <stdbool.h>
#include <stddef.h>

typedef void (*thread_func_t)(void *arg);

struct tpool_work
{
  thread_func_t func;
  void *arg;
  struct tpool_work *next;
} typedef tpool_work_t;

struct tpool
{
  tpool_work_t *work_first;
  tpool_work_t *work_last;
  pthread_mutex_t work_mutex;
  pthread_cond_t work_cond;
  pthread_cond_t working_cond;
  size_t working_cnt;
  size_t thread_cnt;
  bool stop;
} typedef tpool_t;

tpool_t *tpool_create(size_t num);
void tpool_destroy(tpool_t *tm);

bool tpool_add_work(tpool_t *tm, thread_func_t func, void *arg);
void tpool_wait(tpool_t *tm);

#endif // __POOL__
