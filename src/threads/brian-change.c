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

bool priority_compare(const struct list_elem *x, 
                      const struct list_elem *y, void *aux){
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

//void thread_preemption(void){
//    enum intr_level old_level = intr_disable();
//    
//    if (!list_empty(&ready_list) && thread_current()->priority < list_entry(list_front(&ready_list), struct thread, elem)->priority){
//        thread_yield();
//    }
//    intr_set_level(old_level);
//}

//bool lock_priority_compare(const struct list_elem *x,
//                           const struct list_elem *y, 
//                           void *aux UNUSED){
//    struct lock *lock_x = list_entry(x, struct lock, elem);
//    struct lock *lock_y = list_entry(y, struct lock, elem);
//    return lock_x->priority > lock_y->priority;
//}
//
//void update_priority(struct thread *t){
//    enum intr_level old_level = intr_disable();
//    struct thread *curr = thread_current();
//    int max_priority = t->priority;
//    if (t->priority < curr->priority){
//        max_priority = curr->priority;
//    }
//    if (!list_empty(&t->lock_list)){
//        list_sort(&t->lock_list, lock_priority_compare, NULL);
//        int front_priority =  list_entry(list_front(&t->lock_list), struct lock, elem)->priority;
//        if (front_priority > max_priority){
//            max_priority = front_priority;
//        }
//    }
//    t->start_priority = t->priority;
//    t->priority = max_priority;
//    intr_set_level(old_level);
//}
//
//void donate_priority(struct thread *t){
//    enum intr_level old_level = intr_disable();
//    update_priority(t);
//    if (t->status==THREAD_READY){
//        list_sort(&ready_list, priority_compare, NULL);
//    }
//    intr_set_level(old_level);
//}
//
//void get_lock(struct lock *lock){
//    enum intr_level old_level = intr_disable();
//    struct thread *curr = thread_current();
//    list_insert_ordered(&curr->lock_list, &lock->elem, lock_priority_compare, NULL);
////    if (curr->priority > lock->priority){
////        lock->priority = curr->priority;
////    }
//    intr_set_level(old_level);
//}
//
//void remove_lock(struct lock *lock){
//    enum intr_level old_level = intr_disable();
//    list_remove(&lock->elem);
//    
//    struct thread *curr = thread_current();
//    if (curr->start_priority != curr->priority){
//        curr->priority = curr->start_priority;
//    }
//    intr_set_level(old_level);
//}

//void thread_priority_donate(struct thread *t, int priority){
//    t->priority = priority;
//    if (t==thread_current() && !list_empty(&ready_list)){
//        struct thread *next = list_entry(list_begin(&ready_list), struct thread, elem);
//        if (next!=NULL && next->priority>priority){
//            thread_yield();
//        }
//    }
//}