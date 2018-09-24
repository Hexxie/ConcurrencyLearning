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

pthread_cond_t threadA_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t threadB_cond = PTHREAD_COND_INITIALIZER;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

static void* doA(void *arg) {
    printf("This is a thread A\n");
    printf("We're going to signal to thread B\n");

    sleep(10);
    pthread_cond_signal(&threadA_cond);

    printf("wait for threadB again\n");
    pthread_mutex_lock(&mutex);
    pthread_cond_wait(&threadB_cond, &mutex);
    pthread_mutex_unlock(&mutex);
    printf("Got signal from B1\n");
}

static void doB(void *arg) {
    int sem_value;

    printf("This is threadB!\n");
    pthread_mutex_lock(&mutex);
    pthread_cond_wait(&threadA_cond, &mutex);
    printf("ThreadB got it!\n");
    pthread_mutex_unlock(&mutex);

    printf("Lets signal to threadA again!\n");

    sleep(5);
    pthread_cond_signal(&threadB_cond);

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
