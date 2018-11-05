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
 *    it to the terminal -----------> done
 *  - n producers generates logs, consumer writes it to the terminal ------> done
 *  - n producers generates logs, n consumers --------------> done
 *
 *
 */

#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "utils/queue.h"

#define MAX_NR_MESSAGES 10
#define NR_PRODUCERS 3
#define NR_CONSUMERS 2

struct {
    queue_t queue;
    pthread_mutex_t lock;
    pthread_cond_t event;
} typedef shared_t;

static shared_t buffer;

static int doConcumerCycle = 1;

static pthread_mutex_t finishConsumer = PTHREAD_MUTEX_INITIALIZER;

//------------------thread exec functions-------------
void* produceEvent(void *arg) {
    time_t t;
    int cycles = 5;
    int value = 0;
    int counter = 0;
    char* message;

    srand((unsigned) time(&t));

    while(counter < MAX_NR_OF_ITEMS + 10) {



        //Q: When should I remove the message? -->> memory leak (and how)
        message = (char*)malloc(sizeof(char) * 100);

        if(message) {

            for(int i = 0; i < cycles; i++) {
                value = rand() % 50;
            }

            sprintf(message, "Producer %ul generated nr %d\n", pthread_self(), value);

            pthread_mutex_lock(&buffer.lock);

            //if queue is full - wait till consumer remove some
            if(buffer.queue.length >= MAX_NR_OF_ITEMS) {
                pthread_cond_wait(&buffer.event, &buffer.lock);
            }

            queue_add(&buffer.queue, (void*)message);

            //signal that consumer can start to consume
            pthread_cond_signal(&buffer.event);
            pthread_mutex_unlock(&buffer.lock);            
        }

    counter++;
    }//while

    //signal consumer to finish
    pthread_mutex_lock(&finishConsumer);
    doConcumerCycle = 0;
    pthread_mutex_unlock(&finishConsumer);

    return NULL;
}

void* consumeEvent(void* arg) {
    void *item;

   while(doConcumerCycle == 1 || buffer.queue.length != 0) {

    pthread_mutex_lock(&buffer.lock);

    //if queue is empty wait till producer produce something
    if(buffer.queue.length == 0) {
        pthread_cond_wait(&buffer.event, &buffer.lock);
    }

    item = queue_get(buffer.queue);
    queue_remove(&buffer.queue);

    //signal that producer can produce
    pthread_cond_signal(&buffer.event);
    pthread_mutex_unlock(&buffer.lock);

    printf("Thread %ul: extracted item: %s\n", pthread_self(), (char*)item);
    free(item);
   }//while

    return NULL;
}

//------------------- main --------------------------

int main(void) {

    pthread_t consumer[NR_CONSUMERS], producer[NR_PRODUCERS];

    //init block
    pthread_mutex_init(&buffer.lock, NULL);
    pthread_cond_init(&buffer.event, NULL);
    memset(&buffer.queue.items, (int)NULL, MAX_NR_OF_ITEMS);
    buffer.queue.length = 0;

    for(int i = 0; i < NR_PRODUCERS; i++) {
        pthread_create(&producer[i], NULL, produceEvent, NULL);
    }
    for(int i = 0 ; i < NR_CONSUMERS; i++) {
       pthread_create(&consumer[i], NULL, consumeEvent, NULL);
    }



    //destroy block
    for(int i = 0; i < NR_CONSUMERS; i++) {
        pthread_join(consumer[i], NULL);
    }
    for(int i = 0; i < NR_PRODUCERS; i++) {
        pthread_join(producer[i], NULL);
    }
    pthread_cond_destroy(&buffer.event);
    pthread_mutex_destroy(&buffer.lock);
    return 0;
}
