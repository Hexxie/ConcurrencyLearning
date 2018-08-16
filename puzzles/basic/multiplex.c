/*
 * Generalize the mutex solutin so that it allows multiple
 * threads to run in the critical section at the same time,
 * but it enforces an upper limit on the number of concurrent threads.
 * In other words, no more than n threads an run in the critical section
 * at the same time.
 */
