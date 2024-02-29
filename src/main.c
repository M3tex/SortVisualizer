#include <gtk/gtk.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/sem.h>

#include "visualizer.h"
#include "arrays.h"
#include "sorts.h"
#include "gui.h"





int main(int argc, char ** argv)
{
    srand(time(NULL));

    GtkApplication * app = gtk_application_new("fr.m3tex.sort_visualizer", G_APPLICATION_FLAGS_NONE);

    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    
    g_object_unref(app);


    return status;
}