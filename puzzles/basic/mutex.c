/*
 * Add semaphores to the following example to enforce mutual exclusion
 * to shared variable count
 *
 * THREAD A                       THREAD B
 * count = count + 1              count = count + 1
 */

#include <pthread.h>
#include <semaphore.h>

sem_t semaphore_for_counter;
static int counter = 0;

static void* doA(void *arg) {
    printf("This is thread A\n");

    for(int i = 0; i < 10; i++) {
        sem_wait(&semaphore_for_counter);
        counter++;
        printf("A: +1 - counter %d\nPass B\n", counter);
        sem_post(&semaphore_for_counter);
    }
}

static void doB(void *arg) {
   printf("This is thread B\n");

   for(int i = 0; i < 10; i++) {
       sem_wait(&semaphore_for_counter);
       counter++;
       printf("B: +1 - counter %d\nPass A\n", counter);
       sem_post(&semaphore_for_counter);
   }

}


int main() {
    pthread_t threadA, threadB;
    sem_init(&semaphore_for_counter, 0, 1);
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
