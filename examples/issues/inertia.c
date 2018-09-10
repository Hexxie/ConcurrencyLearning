#include <pthread.h>

/*
 * RULE #1: Always design your code so that threads aren;t started until after all
 * the resources they need have been created and initialized exactly the way you
 * want the thread to see them.
 *
 * Creating a thread that rilies on themporary storage in the creator thread
 * is almost always a bad idea.
 *
 * Test your code thoroughly on a multiprocessor, if at all possible
 */

void *printer_thread(void *arg) {
    char *string = *(char**)arg;

    printf("%s\n", string);
    return NULL;
}

int main41() {
    pthread_t printer_id;
    char *string_ptr;
    int i, status;

    string_ptr = "Before value";
    status = pthread_create(&printer_id, NULL, printer_thread, (void*)&string_ptr);
    if(status != 0) {
        perror("Create thread");
        exit(0);
    }

    /*
     * Give the thread a chance to get started if it's going to run in parallel,
     * but not enough that the current thread is likely to be timesliced.
     * (This is a tricky balance, and the loop may need to be adjusted on
     * your system before you can see the bug.
     */
   // for(i = 0; i < 1000000000000000; i++) {};

    string_ptr = "After value";

    status = pthread_join(printer_id, NULL);
    if(status != 0) {
        perror("join thread");
        exit(0);
    }
    return 0;
}
