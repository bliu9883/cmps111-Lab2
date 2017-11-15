#ifndef CODE 
#define CODE
 
#include <debug.h>
#include <list.h>
#include <stdint.h>


bool priority_compare(const struct list_elem *x, 
                      const struct list_elem *y, 
                      void *aux);
bool cmp_ticks_less(const struct list_elem *x,
               const struct list_elem *y,
               void *aux UNUSED);

bool sem_priority_compare(const struct list_elem *x, 
                          const struct list_elem *y,
                          void *aux UNUSED);

//bool lock_priority_compare(const struct list_elem *x,
//                           const struct list_elem *y, 
//                           void *aux UNUSED);
void thread_preemption(void);
void thread_priority_donate(struct thread *t, int priority);
void update_priority(struct thread *t);
void donate_priority(struct thread *t);
void get_lock(struct lock *lock);
void remove_lock(struct lock *lock);
void reset_priority(void);

#endif  /* my helper functions */