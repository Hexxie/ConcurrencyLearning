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
#include <stdlib.h>

#include "utils/queue.h"

#define MAX_NR_MESSAGES 10

struct {
    queue_t queue;
    pthread_mutex_t lock;
    pthread_cond_t event;
} typedef shared_t;

static shared_t buffer;

//------------------thread exec functions-------------
void* produceEvent(void *arg) {
    time_t t;
    int cycles = 5;
    int value = 0;
    int counter = 0;
    char* message;

    message = (char*)malloc(sizeof(char) * 100);

    while(counter < MAX_NR_MESSAGES) {
        if(message) {
            srand((unsigned) time(&t));

            for(int i = 0; i < cycles; i++) {
                value = rand() % 50;
            }

            sprintf(message, "generated nr %d\n", value);

            pthread_mutex_lock(&buffer.lock);
            queue_add(&buffer.queue, (void*)message);
            pthread_mutex_unlock(&buffer.lock);

            pthread_cond_signal(&buffer.event);
        }

    counter++;
    }

    free(message);

    return NULL;
}

void* consumeEvent(void* arg) {
    void *item;

    //Q: can I use the same mutex for different (totally) threads?
    pthread_mutex_lock(&buffer.lock);
    pthread_cond_wait(&buffer.event, &buffer.lock);
    pthread_mutex_unlock(&buffer.lock);

    pthread_mutex_lock(&buffer.lock);
    item = queue_get(buffer.queue);
    queue_remove(&buffer.queue);
    pthread_mutex_unlock(&buffer.lock);

    printf("extracted item: %s\n", (char*)item);

    return NULL;
}

//------------------- main --------------------------

int main(void) {

    pthread_t consumer, producer;

    //init block
    pthread_mutex_init(&buffer.lock, NULL);
    pthread_cond_init(&buffer.event, NULL);
    memset(&buffer.queue.items, (int)NULL, MAX_NR_OF_ITEMS);
    buffer.queue.length = 0;

    pthread_create(&producer, NULL, produceEvent, NULL);
    pthread_create(&consumer, NULL, consumeEvent, NULL);



    //destroy block
    pthread_join(consumer, NULL);
    pthread_join(producer, NULL);
    pthread_cond_destroy(&buffer.event);
    pthread_mutex_destroy(&buffer.lock);
    return 0;
}
