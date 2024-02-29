#ifndef SORTS_HEADER
#define SORTS_HEADER

#include "arrays.h"
#include "visualizer.h"


/* IDs used in the GtkComboBox */
typedef enum {
    BUBBLE_SORT_ID = 1,
    SELECTION_SORT_ID = 2,
    INSERTION_SORT_ID = 3,
    MERGE_SORT_ID = 4,
    QUICK_SORT_ID = 5,
    HEAP_SORT_ID = 6,
    COCKTAIL_SORT_ID = 7
} sorts_ids;


index_t selection(visualizer_t *visualizer, index_t start);
void selection_sort(visualizer_t *visualizer);

char propagate(visualizer_t *visualizer, index_t start, index_t end);
void bubble_sort(visualizer_t *visualizer);
void cocktail_sort(visualizer_t *visualizer);


void insert(visualizer_t *visualizer, index_t start);
void insertion_sort(visualizer_t *visualizer);

index_t partition(visualizer_t *visualizer, index_t p, index_t r);
void forward(visualizer_t *visualizer, int pivot, index_t *i);
void backward(visualizer_t *visualizer, int pivot, index_t *j);
void quick_sort(visualizer_t *visualizer, index_t p, index_t r);

void sift(visualizer_t *visualizer, index_t father_idx, index_t end);
void heapify(visualizer_t *visualizer);
void heap_sort(visualizer_t *visualizer);

void merge(visualizer_t *visualizer, index_t p, index_t q, index_t r);
void merge_sort(visualizer_t *visualizer);

void counting_sort(visualizer_t *visualizer);
void repartition_sort(visualizer_t *visualizer);


/* Utils functions */
void use_selected_sort(visualizer_t *visualizer, int sort_id);

#endif