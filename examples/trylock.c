#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define SPIN 10000000

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static long counter;
static time_t end_time;

/*
 * Thread start routine that repeately locks a mutex and increments a counter
 *
 * Writer thread
 */
void *counter_thread(void *args) {
    int status;
    int spin;

    /*
     * Until end time, inclrement the counter each second. Instead of
     * just incrementing the counter, it sleeps for aother second with
     * the mutex locked? to give onitor_thread a reasonable chance of running
     */

    while(time (NULL) < end_time) {
        status = pthread_mutex_lock(&mutex);
        if(status != 0) {
            perror("Lock mutex");
            exit(0);
        }
        //write to the global variable
        for(spin = 0; spin < SPIN; spin++) {
            counter++;
        }
        status = pthread_mutex_unlock(&mutex);
        if(status != 0) {
            perror("Unock mutex");
            exit(0);
        }
        sleep(1);
    }
    printf("Counter is %#lx\n", counter);
    return NULL;
}

/*
 * Thread starts routine to "monitor" the counter.
 * Every 3 seconds, try to lock the mutex and read the counter. If the
 * trylock fails, skip the cycle.
 *
 * Reader thread. Id should not block writer thread
 */

void *monitor_thread(void *args) {
    int status;
    int misses = 0;

    /*
     * Loop until end_time, checking the counter every 3 seconds.
     */

    while(time (NULL) < end_time) {
        sleep(3);
        status = pthread_mutex_trylock(&mutex);
        if(status != EBUSY) {
            if(status != 0) {
                perror("Trylock mutex");
                exit(0);
            }
            //read the global variable
            printf("Counter is %ld\n", counter/SPIN);
            status = pthread_mutex_unlock(&mutex);
            if(status != 0) {
                perror("Unlock mutex");
                exit(0);
            }
        } else {
            //Count misses on the lock
            misses++;
        }
        printf("Monitor thread missed update %d times\n", misses);
    }
}

int main() {
    int status;
    pthread_t counter_thread_id;
    pthread_t monitor_thread_id;

    end_time = time(NULL) + 60; //Run for 1 min
    status = pthread_create(&counter_thread_id, NULL, counter_thread, NULL);
    if(status != 0) {
        perror("Create counter thread");
        exit(0);
    }
    status = pthread_create(&monitor_thread_id, NULL, monitor_thread, NULL);
    if(status != 0) {
        perror("Create monitor thread");
        exit(0);
    }
    status = pthread_join(counter_thread_id, NULL);
    if(status != 0) {
        perror("Join counter thread");
        exit(0);
    }
    status = pthread_join(monitor_thread_id, NULL);
    if(status != 0) {
        perror("Join monitor thread");
        exit(0);
    }
    return 0;
}
