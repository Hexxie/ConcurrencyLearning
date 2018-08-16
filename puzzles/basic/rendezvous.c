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
