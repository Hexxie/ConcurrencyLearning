/*
 * Add semaphores to the following example to enforce mutual exclusion
 * to shared variable count
 *
 * THREAD A                       THREAD B
 * count = count + 1              count = count + 1
 */

#include <pthread.h>
#include <semaphore.h>

pthread_mutex_t counter_mutex = PTHREAD_MUTEX_INITIALIZER;
static int counter = 0;

static void* doA(void *arg) {
    printf("This is thread A\n");

    for(int i = 0; i < 1000; i++) {
        pthread_mutex_lock(&counter_mutex);
        counter++;
        printf("A: +1 - counter %d\nPass B\n", counter);
        pthread_mutex_unlock(&counter_mutex);
    }
}

static void doB(void *arg) {
   printf("This is thread B\n");

   for(int i = 0; i < 1000; i++) {
       pthread_mutex_lock(&counter_mutex);
       counter++;
       printf("B: +1 - counter %d\nPass A\n", counter);
       pthread_mutex_unlock(&counter_mutex);
   }

}


int main() {
    pthread_t threadA, threadB;
    int result;

    printf("start to create threads\n\n");

    result = pthread_create(&threadB, NULL, doB, NULL);
    result = pthread_create(&threadA, NULL, doA, NULL);

    printf("\ndo main\n");

    result = pthread_join(threadB, NULL);
    result = pthread_join(threadA, NULL);

    printf("the app finished\n\n");

    return 0;
}
