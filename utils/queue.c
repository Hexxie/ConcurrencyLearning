#include <stdio.h>

#include "queue.h"


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

static void print_queue_check(queue_t q, int nrItems) {
    for(int i = 0; i < nrItems; i++) {
        printf("item added: %d\n", (int)q.items[i]);
    }
    printf("\n");
}


static void main_queue_check() {
    //Check that queue works ok
    int result;
    queue_t queue;

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
}
