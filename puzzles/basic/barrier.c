/*
 * Generalize rendezvous solution. Every thread should run the
 * following code:
 *
 * rendezvous
 * critical point
 *
 * The synchronixation requirement is that no thread executes critical point
 * until after all threads have executes rendezvous.
 * You can assume that there are n threads and that this value is stored in a
 * variable, n, that is accessible from all threads.
 * When the first n-1 threads arrive they should block until the n-th thread
 * arrives, at which point all the threads may proceed.
 */
