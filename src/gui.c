#include <gtk/gtk.h>

#include "gui.h"
#include "visualizer.h"
#include "arrays.h"
#include "sorts.h"


/* Global variables */
globals_t globals;
guint timeout_id;



void
init_globals(GtkBuilder *builder)
{
    /* TODO: init from the actual value of the spinbutton */
    globals.number_of_items = 100;
    globals.delay_ms = 10;
    globals.delay_us = 1000 * globals.delay_ms;
    globals.algo_thread_id = 0;
    globals.is_algo_thread_running = 0;
    globals.is_algo_thread_paused = 0;

    globals.number_spinbutton = (GtkSpinButton *) gtk_builder_get_object(builder,"nb_spinbutton");
    globals.delay_spinbutton = (GtkSpinButton *) gtk_builder_get_object(builder,"delay_spinbutton");
    globals.comp_label = (GtkLabel *) gtk_builder_get_object(builder, "comp_label");
    globals.swap_label = (GtkLabel *) gtk_builder_get_object(builder, "swaps_label");
    globals.time_compl_label = (GtkLabel *) gtk_builder_get_object(builder, "complexity_label");
    globals.draw_area = (GtkDrawingArea *) gtk_builder_get_object(builder, "draw_area");
    globals.selector = (GtkComboBoxText *) gtk_builder_get_object(builder, "sort_selection");
    globals.rdm_button = (GtkButton *) gtk_builder_get_object(builder, "rdm_button");
    globals.play_button = (GtkButton *) gtk_builder_get_object(builder, "play_button");
}




/**
 * @brief Redraws the gui
 * 
 * @param data 
 * @return gboolean 
 */
gboolean 
refresh(gpointer data)
{
    visualizer_t *visualizer = (visualizer_t *) data;


    int tmp = gtk_spin_button_get_value_as_int(globals.number_spinbutton);
    if (!globals.is_algo_thread_running && tmp != globals.number_of_items)
    {
        globals.number_of_items = tmp;
        destroy_array(&(visualizer -> array));
        visualizer -> array = rdm_array(globals.number_of_items);
        visualizer -> need_redraw = 1;
    }

    if (!(visualizer -> need_redraw)) return TRUE;


    /*
     * We use static to avoid reallocating each time we need to redraw.
     *
     * comp_text and swap_text aren't freed since they'll be used until
     * the app is closed. (TODO: maybe free them at the end)
     */
    static gchar *comp_text = NULL;
    static gchar *swap_text = NULL;

    /* Only true once */
    if (comp_text == NULL || swap_text == NULL)
    {
        /*
         * 'Number of comparisons: ' -> 23 char. 
         * 'Number of swaps: ' -> 17 char.
         * There'll be at most log10(SIZE_MAX) + 1 digits for the number 
         */
        int max_digits = (int) log10((double) SIZE_MAX) + 1;
        comp_text = (gchar *) malloc(sizeof(gchar) * (23 + max_digits + 1));
        swap_text = (gchar *) malloc(sizeof(gchar) * (17 + max_digits + 1));
        if (comp_text == NULL || swap_text == NULL)
        {
            perror("Error while allocating memory");
            exit(ALLOC_FAILED);
        }
    }

    sprintf(comp_text, "Number of comparisons: %lu", visualizer -> nb_comparisons);
    sprintf(swap_text, "Number of swaps: %lu", visualizer -> nb_swaps);
    

    gtk_label_set_label(globals.comp_label, comp_text);
    gtk_label_set_label(globals.swap_label, swap_text);

    globals.delay_ms = gtk_spin_button_get_value(globals.delay_spinbutton);
    globals.delay_us = 1000 * globals.delay_ms;

    gtk_widget_queue_draw(GTK_WIDGET(globals.draw_area));

    visualizer -> need_redraw = 0;

    return TRUE;
}



void
thread_cleanup(visualizer_t *visualizer)
{
    reset_indexes(visualizer);
    globals.is_algo_thread_running = 0;
    globals.is_algo_thread_paused = 0;
    
    GtkWidget *play_img = gtk_image_new_from_icon_name("media-playback-start");
    GtkWidget *rdm_img = gtk_image_new_from_icon_name("media-playlist-shuffle");
    gtk_button_set_child(GTK_BUTTON(globals.play_button), GTK_WIDGET(play_img));
    gtk_button_set_child(GTK_BUTTON(globals.rdm_button), GTK_WIDGET(rdm_img));

    gtk_widget_set_sensitive(GTK_WIDGET(globals.selector), TRUE);
    gtk_widget_set_sensitive(GTK_WIDGET(globals.number_spinbutton), TRUE);
    visualizer -> need_redraw = 1;
}



void *
test_thread(void *data)
{
    visualizer_t *visualizer = (visualizer_t *) data;

    GtkComboBoxText *selector = globals.selector;
    
    const gchar *tmp = gtk_combo_box_get_active_id((GtkComboBox *) selector);
    int selected = atoi(tmp);
    
    use_selected_sort(visualizer, selected);

    thread_cleanup(visualizer);
    return NULL;
}





void
play_button_callback(GtkButton *btn, visualizer_t *visualizer)
{
    GtkWidget *pause_img = gtk_image_new_from_icon_name("media-playback-pause");
    GtkWidget *play_img = gtk_image_new_from_icon_name("media-playback-start");
    GtkWidget *stop_img = gtk_image_new_from_icon_name("media-playback-stop");

    /* To avoid creating multiple threads */
    /* TODO: Pause/unpause when clicked again */
    if (globals.is_algo_thread_running)
    {
        if (globals.is_algo_thread_paused)
        {
            gtk_button_set_child(GTK_BUTTON(btn), GTK_WIDGET(pause_img));
        }
        else
        {
            /* Modifies the button to have a stop icon */
            gtk_button_set_child(GTK_BUTTON(btn), GTK_WIDGET(play_img));
        }
        globals.is_algo_thread_paused = !globals.is_algo_thread_paused;
        return;
    }
    
    gtk_widget_set_sensitive(GTK_WIDGET(globals.selector), FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(globals.number_spinbutton), FALSE);

    reset_visualizer(visualizer);
    gtk_button_set_child(GTK_BUTTON(btn), GTK_WIDGET(pause_img));
    gtk_button_set_child(GTK_BUTTON(globals.rdm_button), GTK_WIDGET(stop_img));
    pthread_create(&(globals.algo_thread_id), NULL, test_thread, visualizer);
    globals.is_algo_thread_running = 1;
}



void
rdm_button_callback(GtkButton *btn, visualizer_t *visualizer)
{
    visualizer -> need_redraw = 1;
    if (globals.is_algo_thread_running)
    {
        pthread_cancel(globals.algo_thread_id);
        thread_cleanup(visualizer);
        return;
    }

    
    reset_visualizer(visualizer);
    shuffle_array(visualizer -> array);
}



void at_gtk_exit()
{
    /* Stopping the recurrent call to `refresh` */
    g_source_remove(timeout_id);
}



void activate(GtkApplication *app, gpointer user_data)
{
    /* Construct a GtkBuilder instance and load our UI description */
    GtkBuilder *builder = gtk_builder_new ();
    gtk_builder_add_from_file (builder, "sort_visualizer.ui", NULL);

    init_globals(builder);

    // connect signal handlers to the constructed widgets
    GObject *window = gtk_builder_get_object(builder,"window");
    GObject *headerbar = gtk_builder_get_object(builder, "header_bar");
    GObject *play_button = gtk_builder_get_object(builder, "play_button");
    GObject *rdm_button = gtk_builder_get_object(builder, "rdm_button");
    GtkDrawingArea *draw_area = (GtkDrawingArea *) gtk_builder_get_object(builder, "draw_area");

    

    g_signal_connect(G_OBJECT(window),"destroy", at_gtk_exit, NULL);


    array_t *array = rdm_array(100);
    visualizer_t *visualizer = init_visualizer(array);


    gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(draw_area), draw, visualizer, NULL);
    
    g_signal_connect(play_button, "clicked", G_CALLBACK(play_button_callback), visualizer);
    g_signal_connect(rdm_button, "clicked", G_CALLBACK(rdm_button_callback), visualizer);
    /* g_signal_connect(combobox, "changed") -> quand le selectionné est changé */

    gtk_window_set_titlebar(GTK_WINDOW(window), GTK_WIDGET(headerbar));
    gtk_window_set_application(GTK_WINDOW(window), app);


    gtk_window_present(GTK_WINDOW(window));



    /* The `refresh` function will be called every millisecond */
    timeout_id = g_timeout_add(1, refresh, visualizer);

    g_object_unref(builder);
}