/*
 * 1 thread is writing in some shared memory
 * 5 threads are reading shared memory
 *
 * Create a semaphore for it.
 *
 * Help:
 * sem_wait - decrements the semaphore
 * sem_post - increment semapthore
 * sem_getvalue - get semaphore counter
 *
 * If you have  section of code in which you want up to 2 threads
 * to execute simultaneously while other wait, you init it to the
 * value 2, than put a sem_wait at the beginning of the code and a
 * sem_post at the end
 *
 * Problem:
 */

#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define ARRAY_SIZE 10
#define NR_OF_THREADS 10
#define NR_OF_ALLOWED_READERS 5
#define NR_OF_ALLOWED_WRITERS 1
#define SHARED_BETWEEN_THREADS 0
#define NR_LOOPS 100

static sem_t semaphore;
static int terminate_thread = 1;
static int shared_array[ARRAY_SIZE];
pthread_mutex_t mutex;

/*
 * Write value to the array by index
 *
 * @arg index index where value should be put
 * @arg value value to write
 *
 * note! Index no more than ARRAY_SIZE
 */
void write_to_array(int index, int value) {
    int sem_value, result;

    result = sem_getvalue(&semaphore, &sem_value);
    if(result != 0) {
        perror("Get semaphore value");
        exit(0);
    }

    if(sem_value < NR_OF_ALLOWED_WRITERS) {
        if(index > 0 && index < ARRAY_SIZE) {
            sem_post(&semaphore);
            shared_array[index] = value;
            sem_wait(&semaphore);
        }
    }
}

/*
 * Read value from the array by index
 *
 * @index required index for array sequence
 * @return value of the array
 */
int read_from_array(int index){
    int sem_value, result, value;

    result = sem_getvalue(&semaphore, &sem_value);
    if(result != 0) {
        perror("Get semaphore value");
        exit(0);
    }

    if(sem_value < NR_OF_ALLOWED_READERS) {
        sem_post(&semaphore);
        value = shared_array[index];
        sem_wait(&semaphore);
    }

    return value;

}

/*
 * Print full array values from 0 to ARRAY_SIZE
 */
void print_full_array() {
    for (int i = 0; i < ARRAY_SIZE; i++) {
        printf("%d ", read_from_array(i));
    }
    printf("\n");
}

/*
/* write random values to the shared array
 * print full array every ARRAY_SIZE times
 */
static void *thread_runner(void *arg) {
    int rand_i, rand_v, ctr = 0;
    srand(time(0));

    while(terminate_thread) {
        pthread_mutex_lock(&mutex);
        printf("Attempt %d: ", ctr);
        pthread_mutex_unlock(&mutex);

        for(int i = 0; i < ARRAY_SIZE; i++) {

            rand_i = rand() % ARRAY_SIZE;
            rand_v = rand() % 50;

            write_to_array(rand_i, rand_v);
        }

        print_full_array();
        pthread_mutex_lock(&mutex);
        printf("\n");
        ctr++;
        pthread_mutex_unlock(&mutex);
    }
}


int main() {

    pthread_t reader_threads[NR_OF_THREADS], writer_thread;
    int result;

    sem_init(&semaphore, 0, 0);
    pthread_mutex_init(&mutex, NULL);

    //fill array with zeroes
    memset(shared_array, 0, ARRAY_SIZE);

    //check that filling is valid
    print_full_array();

    //create 10 threads
    for(int i = 0; i < NR_OF_THREADS; i++) {
        result = pthread_create(&reader_threads[i], NULL, thread_runner, NULL);
        if(result != 0) {
            perror("Thread has been failed");
            return -1;
        }
    }

  //  sleep(10);
    terminate_thread = 0;

    //Wait for threads to terminate
    result = pthread_join(writer_thread, NULL);
    if(result != 0) {
        perror("writer join");
        return -1;
    }

    for(int i = 0; i < NR_OF_THREADS; i++) {
        result = pthread_join(reader_threads[i], NULL);
        if(result != 0){
            perror("reader join");
            return -1;
        }
    }

    pthread_mutex_destroy(&mutex);
    sem_destroy(&semaphore);

    return 0;
}
