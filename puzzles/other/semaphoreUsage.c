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
 * Writer reads the file and write it fragment by fragment into shared memory
 * Readers read the shared memory and writes to the new files
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
/* Readers read shared memory and writes to each
 * own file
 *
 * arg - the name of the file to write
 */
static void *thread_reader(void *arg) {

}

/*
 * Read the input file and fill shared memory
 */
static void *thread_writer(void *arg) {

}


int main_usage() {

    pthread_t reader_threads[NR_OF_THREADS], writer_threads[NR_OF_THREADS];
    int result;

    sem_init(&semaphore, 0, 0);
    pthread_mutex_init(&mutex, NULL);

    //fill array with zeroes
    memset(shared_array, 0, ARRAY_SIZE);

    //check that filling is valid
    //print_full_array();

    //create readers
    for(int i = 0; i < NR_OF_THREADS; i++) {
        result = pthread_create(&reader_threads[i], NULL, thread_reader, "");
        if(result != 0) {
            perror("Thread has been failed");
            return -1;
        }
    }

    //create writers
    for(int i = 0; i < NR_OF_THREADS; i++) {
        result = pthread_create(&writer_threads[i], NULL, thread_writer, NULL);
        if(result != 0) {
            perror("Thread has been failed");
            return -1;
        }
    }

  //  sleep(10);
    terminate_thread = 0;

    //Wait for threads to terminate    
    for(int i = 0; i < NR_OF_THREADS; i++) {
        result = pthread_join(writer_threads[i], NULL);
        if(result != 0){
            perror("reader join");
            return -1;
        }
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
