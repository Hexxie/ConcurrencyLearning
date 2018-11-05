/**
 * Readers and writers executes different code. Constraints:
 * 1. Any number of readers can be in the CS simultaneously
 * 2. Writers must have exclusive access to the CS
 *
 * There is a file with some poem.
 * Writer is get the file byte by byte (Allow user to define NR
 * of bytes) and put it in some buffer.
 * Reader is creates a separate file, and write there a poem.
 *
 * 1. 1 reader vs 1 writer
 * 2. n readers vs 1 writer
 * 3. Is it possible to have n writers and n readers?
 *
 *
 */
