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

#define MAX_NR_OF_ITEMS (150)

//---------------- queue ---------------------------
struct {
    pthread_mutex_t lock;
    void* items[MAX_NR_OF_ITEMS];
    int length;
} typedef queue_t;

/*
 * Check that there is no elements in queue
 *
 * @param q - queue instance
 * @return 1 if queue is empty, othrwise 0
 */
char queue_isEmpty(queue_t q) {
    return q.length == 0 ? 1 : 0;
}

/*
 * Get the first element
 *
 * @param q queue instance
 * @return element or -1 if queue is empty
 */
void* queue_get(queue_t q) {

    if(!queue_isEmpty(q)){
      return q.items[0];
    } else {
      return (void*)-1;
    }
}

/*
 * Remove the frst element and shift it
 *
 * @param q - queue instance
 * @return 0 - success, -1 - queue is empty
 */
int queue_remove(queue_t *q) {
    if(!queue_isEmpty(*q)) {
        q->items[0] = NULL;

        for(int i = 0; i < q->length - 1; i++) {
            q->items[i] = q->items[i+1];
        }
        q->items[q->length - 1] = NULL;
        q->length--;

    } else {
        return -1;
    }

    return 0;
}

/*
 * Add an element to the queueETIMEDOUT
 *
 * @param q queue instance
 * @param item item to add
 *
 * @return -1 if queue is full, 0 for success
 */
int queue_add(queue_t *q, void* item) {

    if(q->length == MAX_NR_OF_ITEMS) {
        return -1;
    }

    q->items[q->length] = item;
    q->length++;

    return 0;
}

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

    //Check that queue works ok
    int result;

    result = queue_add(&queue, (void*)1);
    printf("result of the op: %d\n", result);
    result = queue_add(&queue, (void*)2);
    printf("result of the op: %d\n", result);
    result = queue_add(&queue, (void*)3);
    printf("result of the op: %d\n", result);
    result = queue_add(&queue, (void*)4);
    printf("result of the op: %d\n", result);
    printf("length %d\n", queue.length);

    print_queue_check(queue, 4);


    result = queue_remove(&queue);
    printf("result of the op: %d\n", result);
    print_queue_check(queue, 4);

    result = queue_remove(&queue);
    printf("result of the op: %d\n", result);
    print_queue_check(queue, 4);

    result = queue_remove(&queue);
    printf("result of the op: %d\n", result);
    print_queue_check(queue, 4);

    //destroy block
    pthread_join(consumer, NULL);
    pthread_join(producer, NULL);
    pthread_mutex_destroy(&queue.lock);


    return 0;
}
