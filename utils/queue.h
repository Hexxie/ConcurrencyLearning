#ifndef _QUEUE_H_
#define _QUEUE_H_

#include <pthread.h>

#define MAX_NR_OF_ITEMS (150)

struct {
    pthread_mutex_t lock;
    void* items[MAX_NR_OF_ITEMS];
    int length;
} typedef queue_t;


int queue_add(queue_t *q, void* item);
void* queue_get(queue_t q);
char queue_isEmpty(queue_t q);
int queue_remove(queue_t *q);



#endif
