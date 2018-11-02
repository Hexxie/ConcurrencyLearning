/*
 * Producers create items of some kind and add the to a data structure
 * Consumers remove the items and process them
 *
 * Event-driven programs: event occurs - produces thread create an event
 * object and adds in to the buffer. Consumer take events out of the buffer
 * and process then. Consummers are called "event handlers"
 *
 * - While an item is being added to or removed from the buffer, the buffer
 * is in inconsistent state. Therefore threads must have exclusive access
 * to the buffer.
 *
 * - If a consumer thread arrives wile the buffer is empty it blocks until a
 * prduces adds a new item
 *
 * Let producers generate some log messages and add it to the queue
 * Let consumers get this messages and print it to the terminal
 *
 * One more example. For example forecasting. You can get the
 * weather from the different sources, store it to the queue and display
 * it for the different clients.
 *
 * 1-st implementation
 *  - 1 producer. 1 consumer. Producer generates masseges, consumer writes
 *    it to the terminal
 *  - n producers generates logs, consumer writes it to the terminal
 *  - n producers generates logs, n consumer writes it to the different
 *   terminals (1 terminal - 1 consumer). Q: How to share it between different
 *   copies of the program?
 *
 * //TBD: Check that queue is correct (extract it to the separate file)
 * //     Create consumer and producer threads
 *
 */

#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include <stdio.h>

#include "utils/queue.h"

#define MAX_NR_OF_ITEMS (150)

//---------------- queue ---------------------------


//------------------thread exec functions-------------
void* produceEvent(void *arg) {



    return NULL;
}

void* consumeEvent(void* arg) {
    return NULL;
}

//------------------- main --------------------------

void print_queue_check(queue_t q, int nrItems) {
    for(int i = 0; i < nrItems; i++) {
        printf("item added: %d\n", (int)q.items[i]);
    }
    printf("\n");
}

int main(void) {

    queue_t queue;
    pthread_t consumer, producer;

    //init block
    pthread_mutex_init(&queue.lock, NULL);
    memset(queue.items, (int)NULL, MAX_NR_OF_ITEMS);
    queue.length = 0;

    pthread_create(&producer, NULL, produceEvent, NULL);
    pthread_create(&consumer, NULL, consumeEvent, NULL);


    //destroy block
    pthread_join(consumer, NULL);
    pthread_join(producer, NULL);
    pthread_mutex_destroy(&queue.lock);


    return 0;
}
