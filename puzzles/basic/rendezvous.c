/*
 * Generalize the signal pattern so that it works both ways
 * Thread A has to wait for Thread B and vice versa.
 * In other words given the code:
 *
 * THREAD A                    THREAD B
 * 1. statement a1             1. statement b1
 * 2. statement b2             2. statement b2
 *
 * we want to guarantee that a1 happens before b2 and b1 happens before a2
 */

#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>

static sem_t threadA1_finishes;
static sem_t threadB1_finishes;

static void* doA(void *arg) {
    printf("This is a thread A\n");
    printf("We're going to signal to thread B\n");
    sleep(10);
    sem_post(&threadA1_finishes);
    printf("wait for threadB again\n");
    sem_wait(&threadB1_finishes);
    printf("Got signal from B1\n");
}

static void* doB(void *arg) {
    int sem_value;

    printf("This is threadB!\n");

    sem_wait(&threadA1_finishes);
    printf("ThreadB got it!\n");

    sem_getvalue(&threadA1_finishes, &sem_value);
    printf("Lets signal to threadA again!\n");
    sleep(5);
    sem_post(&threadB1_finishes);

}


int main() {

    pthread_t threadA, threadB;

    int result;

    printf("Initialize the semaphore\n\n");
    sem_init(&threadA1_finishes, 0, 0);
    sem_init(&threadB1_finishes, 0, 0);

    printf("start to create threads\n\n");

    result = pthread_create(&threadB, NULL, doB, NULL);
    result = pthread_create(&threadA, NULL, doA, NULL);

    printf("\ndo main\n");

    result = pthread_join(threadB, NULL);
    result = pthread_join(threadA, NULL);

    printf("destroy the semaphore\n\n");
    sem_destroy(&threadB1_finishes);
    sem_destroy(&threadA1_finishes);
    printf("the app finished\n\n");

    return 0;
}
