#include <pthread.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "error.h"

//#define DEBUG

typedef struct alarm_tag {
    time_t              time;
    struct alarm_tag    *link;
    int                 seconds;
    char                message[64];
} alarm_t;

/* Statically initialized mutex
 * 
 * Static mutex should be static or extern
 * Static within file scope and xtern if it used by the other files
 * If you use malloc to create a structure which contains mutex,
 * you have to initialize id dynamically:
 * pthread_mutex_init(mutex, NULL)
*/
static pthread_mutex_t alarm_mutex = PTHREAD_MUTEX_INITIALIZER;
static alarm_t *alarm_list = NULL;

void *alarm_thread(void *args) {
    alarm_t *alarm;
    time_t now;
    int status;
    time_t sleep_time;
    
    /*
     * Loop forever to process the commands. The alarm thread will 
     * be disintegrated when process exits
     */ 
    while (1) {
        status = pthread_mutex_lock(&alarm_mutex);
        if(status != 0){
            perror("Lock mutex");
        }
        alarm = alarm_list;
        
        /*
         * If the alarm list is empty, wait for one second.
         * This allows the main thread to run, and read another command.
         * If the list is not empty, remove the first item. Compute the
         * number of secinds to wait -- if the result is less than 0,
         * than set sleep_time to 0.
         */ 
        if(alarm == NULL) {
            sleep_time = 1;
        } else {
            alarm_list = alarm->link;
            now = time(NULL);
            if(alarm->time <= now) {
                sleep_time = 0;
            } else {
                sleep_time = alarm->time - now;
            }
#ifdef DEBUG
            printf("[waiting: %d(%d)\"%s\"\n", alarm->time, sleep_time, alarm->message);
#endif
        }
        
        /*
         * Unlock the mutex before waiting, so that the main thread can
         * lock it to insert a new alarm request. If the sleep_time is 0,
         * then call sched_yoild, giving the main thread a
         * chance to run if it has been readied by used iput without delaying
         * the message if there's no input.
         */ 
        status = pthread_mutex_unlock(&alarm_mutex);

        if(status != 0) {
            sleep(sleep_time);
        } else {
            sched_yield();
        }
        
        /*
         * If time expired, print the message and free the structure
         */
        if(alarm != NULL) {
            printf("{%d} %s\n", alarm->seconds, alarm->message);
            free(alarm);
        }
    }
}

int main12() {
    int status;
    char line[128];
    alarm_t *alarm, **last, *next;
    pthread_t thread;
    
    status = pthread_create(&thread, NULL, alarm_thread, NULL);
    if(status != 0) {
        perror("Create alarm thread");
        return -1;
    }
    while(1) {
        printf("alarm>");
        if(NULL == fgets(line, sizeof(line), stdin)) return 0;
        if(strlen(line) <= 1) continue;
        alarm = (alarm_t*)malloc(sizeof (alarm_t));
        if(alarm == NULL) {
            perror("Allocate alarm");
        }
        
        /*
         * Parse input line into seconds and a message 
         * consisting of up to 64 characters 
         * separated from the seconds by whitespace
         */
        
        if(sscanf(line, "%d %64[^\n]",
                  &alarm->seconds, alarm->message) < 2) {
            fprintf(stderr, "Bad command");
            free (alarm);
        } else {
            status = pthread_mutex_lock(&alarm_mutex);
            if (status != 0) {
                perror("Lock mutex");
            }

            alarm->time = time(NULL) + alarm->seconds;
                
            /*
             * Insert hte new alarm into the list of the alarms
             * sorted by expiration time.
             */

            last = &alarm_list;
            next = *last;
            while (next != NULL) {
                if(next->time >= alarm->time) {
                    alarm->link = next;
                    *last = alarm;
                    break;
                }
                last = &next->link;
                next = next->link;
            }

            /*
             * If we reached the end of the list, insert the new
             * alarm there. ("next" is NULL and "last" point to the link
             * field of the last item or to the list header
             */

            if(next == NULL) {
                *last = alarm;
                alarm->link = NULL;
            }
#ifdef DEBUG
            printf("list: ");
            for(next = alarm_list; next != NULL; next = next->link)
                printf("%d(%d)[\"%s\"] ", next->time, next->time - time(NULL), next->message);
            printf("]\n");
#endif
            status = pthread_mutex_unlock(&alarm_mutex);
            if(status != 0){
                perror("Unlock mutex");
                return -1;
            }
        }
    }
}

