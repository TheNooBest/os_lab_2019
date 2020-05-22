/********************************************************
 * An example source module to accompany...
 *
 * "Using POSIX Threads: Programming with Pthreads"
 *     by Brad nichols, Dick Buttlar, Jackie Farrell
 *     O'Reilly & Associates, Inc.
 *  Modified by A.Kostin
 ********************************************************
 * mutex.c
 *
 * Simple multi-threaded example with a mutex lock.
 */
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void* do_one_thing(void *);
void* do_another_thing(void *);
void do_wrap_up(int);
int common = 0; /* A shared variable for two threads */
int r1 = 0, r2 = 0, r3 = 0;
pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;

int main() {
  pthread_t thread1, thread2;

  if (pthread_create(&thread1, NULL, do_one_thing,
			  (void *)&common) != 0) {
    perror("pthread_create");
    exit(1);
  }

  if (pthread_create(&thread2, NULL, do_another_thing,
                     (void *)&common) != 0) {
    perror("pthread_create");
    exit(1);
  }

  if (pthread_join(thread1, NULL) != 0) {
    perror("pthread_join");
    exit(1);
  }

  if (pthread_join(thread2, NULL) != 0) {
    perror("pthread_join");
    exit(1);
  }

  do_wrap_up(common);

  return 0;
}

void* do_one_thing(void *pnum_times) {
  int i, j, x;
  unsigned long k;
  int work;
  for (i = 0; i < 50; i++) {
    #ifdef MAKE_WITH_MUTEX
    pthread_mutex_lock(&mut);
    #endif

    printf("doing one thing\n");
    work = *(int*)pnum_times;
    printf("counter = %d\n", work);
    work++; /* increment, but not write */
    for (k = 0; k < 500000; k++)
      ;                 /* long cycle */
    *(int*)pnum_times = work; /* write back */
    
    #ifdef MAKE_WITH_MUTEX
	  pthread_mutex_unlock(&mut);
    #endif
  }
  return NULL;
}

void* do_another_thing(void *pnum_times) {
  int i, j, x;
  unsigned long k;
  int work;
  for (i = 0; i < 50; i++) {
    #ifdef MAKE_WITH_MUTEX
    pthread_mutex_lock(&mut);
    #endif

    printf("doing another thing\n");
    work = *(int*)pnum_times;
    printf("counter = %d\n", work);
    work++; /* increment, but not write */
    for (k = 0; k < 500000; k++)
      ;                 /* long cycle */
    *(int*)pnum_times = work; /* write back */
    
    #ifdef MAKE_WITH_MUTEX
	  pthread_mutex_unlock(&mut);
    #endif
  }
  return NULL;
}

void do_wrap_up(int counter) {
  int total;
  printf("All done, counter = %d\n", counter);
}
