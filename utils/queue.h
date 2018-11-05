#ifndef _Q_
#define _Q_

#include <pthread.h>

#define MAX_NR_OF_ITEMS (150)

struct {
    void* items[MAX_NR_OF_ITEMS];
    int length;
} typedef queue_t;


int queue_add(queue_t *q, void* item);
void* queue_get(queue_t q);
char queue_isEmpty(queue_t q);
int queue_remove(queue_t *q);
void print_queue_check(queue_t q, int nrItems);



#endif
