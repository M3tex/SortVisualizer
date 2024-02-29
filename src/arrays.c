#include "arrays.h"
#include <stdio.h>
#include <stdlib.h>


/**
 * @brief Allocate memory for a new array.
 * All values are initialized to 0.
 * 
 * @param size The size of the new array
 * @return array_t* 
 */
array_t *
new_array(size_t size)
{
    int *new_data = (int *) calloc(size, sizeof(int));
    array_t *result = (array_t *) malloc(sizeof(array_t));
    if (new_data == NULL || result == NULL)
    {
        perror("Error while allocating memory for the new array");
        exit(ALLOC_FAILED);
    }

    result -> data = new_data;
    result -> size = size;

    return result;
}



/**
 * @brief Free all the memory allocated to the given array.
 * 
 * @param to_destroy The array to free
 */
void 
destroy_array(array_t **to_destroy)
{
    free((*to_destroy) -> data);
    (*to_destroy) -> data = NULL;
    (*to_destroy) -> size = 0;
    free(*to_destroy);
    *to_destroy = NULL;
}



/**
 * @brief Swaps 2 elements of the array
 * 
 * @param arr 
 * @param i 
 * @param j 
 */
void 
swap(array_t *arr, index_t i, index_t j)
{
    int tmp = (arr -> data)[i];
    (arr -> data)[i] = (arr -> data)[j];
    (arr -> data)[j] = tmp;
}



/**
 * @brief Prints the array to the standard output.
 * Mostly for debug purposes.
 * 
 * Python-like printing, for example:
 * [1, 2, 3, 4]
 * 
 * @param to_print 
 */
void 
print_array(array_t *to_print)
{
    printf("[");
    for (index_t i = 0; i < (to_print -> size); i++)
    {
        printf("%d", (to_print -> data)[i]);
        if (i != (to_print -> size) - 1)
            printf(", ");
    }
    printf("]\n");
}



/**
 * @brief Shuffle an array using the Fisher-Yates algorithm.
 * 
 * @param to_shuffle 
 */
void 
shuffle_array(array_t *to_shuffle)
{
    for (index_t i = (to_shuffle -> size) - 1; i > 0; i--)
    {
        index_t j = rand() % (i + 1);
        swap(to_shuffle, i, j);
    }
}



/**
 * @brief Generates a random permutation of the `size` first integers.
 * 
 * @param size The size of the array to generate
 * @return array_t* 
 */
array_t *
rdm_array(size_t size)
{
    array_t *result = new_array(size);

    for (index_t i = 0; i < size; i++)
        (result -> data)[i] = i + 1;

    shuffle_array(result);

    return result;
}