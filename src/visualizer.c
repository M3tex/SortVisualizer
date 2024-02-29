#include "visualizer.h"
#include "arrays.h"
#include "gui.h"

#include <gtk/gtk.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>

#include <unistd.h>



visualizer_t *
init_visualizer(array_t *array)
{
    visualizer_t *result = (visualizer_t *) malloc(sizeof(visualizer_t));
    if (result == NULL)
    {
        perror("Error while allocating memory");
        exit(ALLOC_FAILED);
    }


    result -> array = array;
    result -> current_idx = ULONG_MAX;
    result -> interest_idx1 = ULONG_MAX;
    result -> interest_idx2 = ULONG_MAX;
    result -> nb_swaps = 0;
    result -> nb_comparisons = 0;
    result -> need_redraw = 0;
    

    return result;
}



void 
reset_visualizer(visualizer_t *visualizer)
{
    reset_indexes(visualizer);
    visualizer -> nb_swaps = 0;
    visualizer -> nb_comparisons = 0;
}


void
reset_indexes(visualizer_t *visualizer)
{
    visualizer -> current_idx = ULONG_MAX;
    visualizer -> interest_idx1 = ULONG_MAX;
    visualizer -> interest_idx2 = ULONG_MAX;
}



/**
 * @brief Updates the GtkDrawArea (where our array's displayed).
 * 
 * Called automatically by GTK everytime necessary, and called via
 * gtk_widget_queue_draw when we need to update the drawing.
 * 
 * @param area 
 * @param cr 
 * @param width 
 * @param height 
 * @param user_data Will point to a visualizer_t.
 */
void 
draw(GtkDrawingArea *area, cairo_t *cr, int width, int height, gpointer user_data)
{
    visualizer_t *visualizer = (visualizer_t *) user_data;
    int *values = visualizer -> array -> data;
    size_t arr_size = visualizer -> array -> size;

    cairo_set_line_width(cr, 1.0);

    /* So the highest rectangle doesn't touch the header bar */
    int top_padding = 10;

    /* So rectangles don't touch each others */
    int spacing = (arr_size * 2 < width) ? 1 : 0;
    int total_spacing = spacing * (arr_size - 1);

    double x, y, bar_width, h;
    bar_width = ((double) width - total_spacing) / arr_size;
    y = height;
    
    for (index_t i = 0; i < arr_size; i++)
    {
        /*
         * The array will contain all values from 1 to arr_size.
         * 
         * Therefore, the maximum will always be arr_size.
         */
        h = ((double) (height - top_padding) / arr_size) * values[i];
        x = (double) i * (bar_width + spacing);

        /*
         * Blue for the current index, green for the 1st interesting index and
         * orange for the 2nd interesting index (if used).
         */
        if (i == visualizer -> current_idx) cairo_set_source_rgb (cr, 0, 0, 1.0);
        else if (i == visualizer -> interest_idx1) cairo_set_source_rgb (cr, 0, 1.0, 0);
        else if (i == visualizer -> interest_idx2) cairo_set_source_rgb (cr, 1.0, 0.6, 0);
        else cairo_set_source_rgb (cr, 0.5, 0.5, 0.5);

        cairo_rectangle(cr, x, y, bar_width, -h);
        cairo_fill(cr);
    }
}


void update(visualizer_t *visualizer)
{
    /* No need to use any kind of mutex as is_algo_thread_paused's value
    will never be updates in this thread */
    while (globals.is_algo_thread_paused);

    visualizer -> need_redraw = 1;
    g_usleep(globals.delay_us);
}