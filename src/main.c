/**
 *  olback.net
 *  github.com/olback/rdg-linux
 */

#include "vars.h"
#include "functions.c"
#include "resources.c"

int main(int argc, char *argv[])
{  

    gtk_init(&argc, &argv);

    builder = gtk_builder_new();
    gtk_builder_add_from_resource(builder, "/org/gnome/rdg/src/rdg.glade", NULL);

    window = GTK_WIDGET(gtk_builder_get_object(builder, "main"));
    keymapInput = GTK_COMBO_BOX_TEXT(gtk_builder_get_object(builder, "keymap"));
    printf("Keymap set to %s\n", gtk_combo_box_text_get_active_text (keymapInput));

    gtk_window_set_title(GTK_WINDOW(window), "rdg");

    gtk_builder_connect_signals(builder, NULL);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    g_object_unref(builder);

    gtk_widget_show(window);

    gtk_main();

    return 0;
}