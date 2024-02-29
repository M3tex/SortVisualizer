#ifndef ARRAYS_HEADER
#define ARRAYS_HEADER

#include <stdlib.h>

/* Error codes */
#define ALLOC_FAILED 1


/**
 * @brief Structure to represent array of integers
 * 
 * data: the actual array of integers.
 * size: its size (number of elements in the array).
 * 
 */
typedef struct {
    int *data;
    size_t size;
} array_t;


/* Probably useless as we may never use arrays of more than MAX_INT elements */
typedef size_t index_t;


array_t *new_array(size_t size);
array_t *rdm_array(size_t size);

void destroy_array(array_t **to_destroy);
void print_array(array_t *to_print);
void shuffle_array(array_t *to_shuffle);
void swap(array_t *arr, index_t i, index_t j);



#endif