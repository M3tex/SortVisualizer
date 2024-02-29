#include <gtk/gtk.h>

#include "sorts.h"
#include "arrays.h"
#include "visualizer.h"
#include "gui.h"



/**
 * @brief Find the index of the smallest element in the sub
 * array starting at index start
 * 
 * @param visualizer 
 * @param start From where the search starts
 * @return index_t 
 */
index_t
selection(visualizer_t *visualizer, index_t start)
{
    array_t *array = visualizer -> array;

    index_t min_idx = start;
    visualizer -> interest_idx1 = min_idx;
    visualizer -> interest_idx2 = start;
    for (index_t i = start + 1; i < array -> size; i++)
    {
        if ((array -> data)[i] < (array -> data)[min_idx])
            min_idx = i;
        
        (visualizer -> nb_comparisons)++;

        visualizer -> interest_idx1 = min_idx;
        visualizer -> current_idx = i;

        update(visualizer);
    }

    return min_idx;
}



/**
 * @brief Sorts the array using the selection sort algorithm.
 * 
 * Time complexity is in O(n²)
 * 
 * @param visualizer 
 */
void 
selection_sort(visualizer_t *visualizer)
{
    array_t *array = visualizer -> array;

    for (size_t i = 0; i < array -> size; i++)
    {
        size_t min_idx = selection(visualizer, i);
        swap(array, i, min_idx);
        (visualizer -> nb_swaps)++;
    }
}



/**
 * @brief Propagates values of the array, from left to right
 * 
 * @param visualizer 
 * @param start Where to start the propagation
 * @param end Where to stop the propagation
 * @return 0 if swaps have been done, else 1 (in this case the array is
 * sorted)
 */
char
propagate(visualizer_t *visualizer, index_t start, index_t end)
{
    array_t *array = visualizer -> array;

    char is_sorted = 1;
    visualizer -> interest_idx1 = end;
    for (index_t i = start; i < end - 1; i++)
    {
        visualizer -> current_idx = i;
        update(visualizer);
        if ((array -> data)[i] > (array -> data)[i + 1])
        {
            swap(array, i, i + 1);
            is_sorted = 0;
            (visualizer -> nb_swaps)++;
        }
        (visualizer -> nb_comparisons)++;
        
        
    }

    return is_sorted;
}



/**
 * @brief Propagates values of the array, from right to left
 * 
 * @param visualizer 
 * @param start Where to start the propagation
 * @param end Where to stop the propagation
 * @return 0 if swaps have been done, else 1 (in this case the array is
 * sorted)
 */
char
r_propagate(visualizer_t *visualizer, index_t start, index_t end)
{
    array_t *array = visualizer -> array;

    char is_sorted = 1;
    visualizer -> interest_idx2 = end;
    for (index_t i = start; i > end + 1; i--)
    {
        visualizer -> current_idx = i;
        update(visualizer);
        if ((array -> data)[i] < (array -> data)[i - 1])
        {
            swap(array, i, i - 1);
            is_sorted = 0;
            (visualizer -> nb_swaps)++;
        }
        (visualizer -> nb_comparisons)++;
        
        
    }

    return is_sorted;
}




/**
 * @brief Sorts the array using the (optimized) propagation/bubble sort.
 * 
 * Time complexity is in O(n²)
 * 
 * @param visualizer 
 */
void 
bubble_sort(visualizer_t *visualizer)
{
    array_t *array = visualizer -> array;

    index_t i = (array -> size);
    while (i > 0 && !propagate(visualizer, 0, i)) i--;
}



/**
 * @brief Sorts the array using the cocktail sort (wich is a variant of
 * the bubble sort).
 * 
 * Time complexity is still in O(n²) but with a better hidden constant.
 * 
 * @param visualizer 
 */
void 
cocktail_sort(visualizer_t *visualizer)
{
    array_t *array = visualizer -> array;

    index_t i = array -> size;
    char flag = 1;

    while (flag && i > 0)
    {
        flag = !(propagate(visualizer, array -> size - i, i) || r_propagate(visualizer, i, array -> size - i - 1));
        i--;
    }
}




void
insert(visualizer_t *visualizer, index_t start)
{
    array_t *array = visualizer -> array;
    visualizer -> interest_idx1 = start;

    while (start > 0 && (array -> data)[start] < (array -> data)[start - 1])
    {
        visualizer -> current_idx = start;
        (visualizer -> nb_comparisons)++;
        (visualizer -> nb_swaps)++;

        update(visualizer);
        swap(array, start, start - 1);
        start--;
    }
}



void
insertion_sort(visualizer_t *visualizer)
{
    array_t *array = visualizer -> array;

    /* Starting to 1 because there's nothing to do for the 1st element */
    for (index_t i = 1; i < array -> size; i++)
    {
        insert(visualizer, i);
    }
}



void
sift(visualizer_t *visualizer, index_t father_idx, index_t end)
{
    int *arr = visualizer -> array -> data;

    char loop = 1;

    while (loop)
    {
        loop = 0;
        index_t son_idx = 2 * father_idx + 1;
        visualizer -> current_idx = father_idx;
        if ((son_idx < end - 1) && (arr[son_idx + 1] > arr[son_idx]))
        {
            son_idx++;
            visualizer -> interest_idx1 = son_idx;
        }
        // visualizer -> interest_idx2 = father_idx;
        
        if ((son_idx < end) && (arr[father_idx] < arr[son_idx]))
        {
            swap(visualizer -> array, father_idx, son_idx);
            (visualizer -> nb_swaps)++;
            loop = 1;
            father_idx = son_idx;
        }

        visualizer -> nb_comparisons += 2;

        update(visualizer);
    }
}


void
heapify(visualizer_t *visualizer)
{
    array_t *array = visualizer -> array;

    index_t start = (array -> size) / 2;
    // start = (array -> size % 2) ? start : start - 1;

    for (index_t i = start; i > 0; i--)
    {
        sift(visualizer, i, array -> size);
    }
    sift(visualizer, 0, array -> size);
}


void heap_sort(visualizer_t *visualizer)
{
    array_t *array = visualizer -> array;

    heapify(visualizer);
    index_t i = array -> size - 1;
    while (i > 0)
    {
        swap(array, 0, i);
        (visualizer -> nb_swaps)++;
        sift(visualizer, 0, i);
        i--;
    }

}




void forward(visualizer_t *visualizer, int pivot, index_t *i)
{
    int *arr = visualizer -> array -> data;
    index_t tmp = *i + 1;
    visualizer -> interest_idx1 = tmp;

    while (arr[tmp] < pivot)
    {
        tmp++;
        (visualizer -> nb_comparisons)++;
        visualizer -> interest_idx1 = tmp;
        update(visualizer);
    }

    *i = tmp;
}



void backward(visualizer_t *visualizer, int pivot, index_t *j)
{
    int *arr = visualizer -> array -> data;
    index_t tmp = *j - 1;
    visualizer -> interest_idx2 = tmp;

    while (arr[tmp] > pivot)
    {
        tmp--;
        (visualizer -> nb_comparisons)++;
        visualizer -> interest_idx2 = tmp;
        update(visualizer);
    }

    *j = tmp;
}



index_t partition(visualizer_t *visualizer, index_t p, index_t r)
{
    int *arr = visualizer -> array -> data;
    int pivot = arr[p];

    while (arr[r] > arr[p])
    {
        (visualizer -> nb_comparisons)++;
        r--;
    }

    while (p < r)
    {
        swap(visualizer -> array, p, r);
        (visualizer -> nb_swaps)++;
        backward(visualizer, pivot, &r);
        forward(visualizer, pivot, &p);
    }

    return r;
}



void quick_sort(visualizer_t *visualizer, index_t p, index_t r)
{
    if (p < r)
    {
        index_t q = partition(visualizer, p, r);
        visualizer -> current_idx = q;
        quick_sort(visualizer, p, q);
        quick_sort(visualizer, q + 1, r);
    }
}

void use_selected_sort(visualizer_t *visualizer, int sort_id)
{
    switch (sort_id)
    {
    case BUBBLE_SORT_ID:
        bubble_sort(visualizer);
        break;
    
    case SELECTION_SORT_ID:
        selection_sort(visualizer);
        break;

    case INSERTION_SORT_ID:
        insertion_sort(visualizer);
        break;
    
    case QUICK_SORT_ID:
        quick_sort(visualizer, 0, visualizer -> array -> size - 1);
        break;
    case HEAP_SORT_ID:
        heap_sort(visualizer);
        break;
    case COCKTAIL_SORT_ID:
        cocktail_sort(visualizer);
        break;
    default:
        break;
    }
}



