#ifndef GUI_HEADER
#define GUI_HEADER

#include <pthread.h>
#include <gtk/gtk.h>
#include "arrays.h"
#include "visualizer.h"



typedef struct {
    gulong delay_us;
    double delay_ms;

    index_t number_of_items;
    pthread_t algo_thread_id;
    char is_algo_thread_running;

    char is_algo_thread_paused;

    GtkLabel *comp_label;
    GtkLabel *swap_label;
    GtkLabel *time_compl_label;
    GtkComboBoxText *selector;
    GtkDrawingArea *draw_area;
    GtkSpinButton *delay_spinbutton;
    GtkSpinButton *number_spinbutton;
    GtkButton *rdm_button;
    GtkButton *play_button;
} globals_t;



extern globals_t globals;



void init_globals(GtkBuilder *builder);
gboolean refresh(gpointer data);
void activate(GtkApplication *app, gpointer user_data);
void at_gtk_exit();
void rdm_button_callback(GtkButton *btn, visualizer_t *visualizer);
void play_button_callback(GtkButton *btn, visualizer_t *visualizer);
void *test_thread(void *data);
void thread_cleanup(visualizer_t *visualizer);

#endif
