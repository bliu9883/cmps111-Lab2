#include <debug.h>
#include <stddef.h>
#include <random.h>
#include <stdio.h>
#include <string.h>

#include "threads/thread.h"
#include "threads/flags.h"
#include "threads/interrupt.h"
#include "threads/intr-stubs.h"
#include "threads/palloc.h"
#include "threads/switch.h"
#include "threads/semaphore.h"
#include "threads/lock.h"
#include "threads/condvar.h"
#include "threads/vaddr.h"
#include "threads/code.h"

bool priority_compare(const struct list_elem *x, 
                      const struct list_elem *y, 
                      void *aux UNUSED)
{
    struct thread *thread_x = list_entry(x, struct thread, elem);
    struct thread *thread_y = list_entry(y, struct thread, elem);
    if (thread_x->priority>thread_y->priority){
        return true;
    }
    else{
        return false;
    }
}

bool cmp_ticks_less(const struct list_elem *x,
               const struct list_elem *y,
               void *aux UNUSED)
{
  struct thread *thread_x = list_entry (x, struct thread, elem);
  struct thread *thread_y = list_entry (y, struct thread, elem);
  if (thread_x->sleeping_time < thread_y->sleeping_time){
      return true;
  }
  else{
      return false;
  }
}

bool sem_priority_compare(const struct list_elem *x, 
                          const struct list_elem *y,
                          void *aux UNUSED){
    struct semaphore *sx = list_entry(x, struct semaphore, elem);
    struct semaphore *sy = list_entry(y, struct semaphore, elem);
    
    list_sort(&sx->waiters, priority_compare, NULL);
    list_sort(&sy->waiters, priority_compare, NULL);
    
    if (list_empty(&sx->waiters)){
        return true;
    }
    if (list_empty(&sy->waiters)){
        return false;
    }
    struct thread *thread_x = list_entry(list_front(&sx->waiters), struct thread, elem);
    struct thread *thread_y = list_entry(list_front(&sy->waiters), struct thread, elem);
    
    if (thread_x->priority > thread_y->priority){
        return true;
    }
    else{
        return false;
    }
}

void remove_lock(struct lock *lock){
    struct list_elem *curr = list_begin(&thread_current()->needs_lock);
    struct list_elem *next;
    while (curr != list_end(&thread_current()->needs_lock)){
        struct thread *t = list_entry(curr, struct thread, needs_lock_elem);
        next = list_next(curr);
        if (t->wait_lock == lock){
            list_remove(curr);
        }
        curr = next;
    }
}

void reset_priority(void){
    struct thread *t = thread_current();
    t->priority = t->base_priority;
    if (list_empty(&t->needs_lock)){
        return;
    }
    struct thread *front = list_entry(list_front(&t->needs_lock), struct thread, 
                                      needs_lock_elem);
    if (front->priority > t->priority){
        t->priority = front->priority;
    }
}