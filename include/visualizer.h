#ifndef VISUALIZER_HEADER
#define VISUALIZER_HEADER

#include "arrays.h"
#include <gtk/gtk.h>




typedef struct {
    array_t *array;

    index_t current_idx;
    index_t interest_idx1;
    index_t interest_idx2;

    size_t nb_swaps;
    size_t nb_comparisons;

    char need_redraw;
} visualizer_t;



visualizer_t *init_visualizer(array_t *array);


void draw(GtkDrawingArea *area, cairo_t *cr, int width, int height, gpointer user_data);

void reset_visualizer(visualizer_t *visualizer);
void reset_indexes(visualizer_t *visualizer);

void update(visualizer_t *visualizer);

#endif
