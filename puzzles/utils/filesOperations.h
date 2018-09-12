/*
 * This header hontains the next operations for files
 *
 * - Get full path from the related one
 * - Check that file exists
 * - Check that file is not empty
 * - Create new file
 * - Read from file
 * - Write to file
 */

#ifndef FILES_OPERATIONS_H
#define FILES_OPERATIONS_H

#include <unistd.h>


/*
 * Get the full path of the file from the related one.
 * Suppose that current directory is the directory
 * where the program is executed
 *
 * @param relatedPath
 */
char* get_full_path_from_the_related(char* relatedPath);

/*
 * Try to open file in the read mode, return -1
 * if the open is faild or 0 on success
 *
 * @param filepath path to the file
 */
int is_file_exists(char *filepath);

/*
 * Check that size of the file is not 0
 * Return -1 if file is 0 size, otherwise - 1
 *
 * @param filepath path to the file
 */
int is_file_empty(char *filepath);

/*
 * Create and empty file with provided filepath. In case of the
 * related filepath - it'll be created in the executed
 * directory
 * Return 0 - in case of success and -1 otherwise
 *
 * @param filepath path for the future file
 */
int create_file(char *filepath);

/*
 * Read size bytes from the file into the buffer
 * return 0 - for success, -1 otherwise
 *
 * @param filepath - path to the file
 * @param buffer - buffer to read in
 * @param size - how much bytes would be read
 */
int read_file(char* filepath, void* buffer, size_t size);

/*
 * Write size butes from the buffer at the end of the file
 * Return 0 - for success, -1 otherwise
 *
 * @param filepath path for the file (throw error, if file not found)
 * @param buffer buffer with butes to be written
 * @param size - how much bytes would be written
 */
int append_file(char* filepath, void* buffer, size_t size);


/*
 * Write size butes from the buffer at the needed position of the file
 * Return 0 - for success, -1 otherwise
 *
 * @param filepath path for the file (throw error, if file not found)
 * @param buffer buffer with butes to be written
 * @param position where bytes should be append
 * @param size - how much bytes would be written
 */
int write_file(char* filepath, void* buffer, int position, size_t size);

#endif
