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
    gtk_builder_add_from_resource(builder, "/net/olback/rdg/src/rdg.glade", NULL);

    window = GTK_WIDGET(gtk_builder_get_object(builder, "main"));
    dialog = GTK_WIDGET(gtk_builder_get_object(builder, "dialog"));

    ipEntryA = GTK_ENTRY(gtk_builder_get_object(builder, "ip"));
    portEntry = GTK_ENTRY(gtk_builder_get_object(builder, "port"));
    widthEntry = GTK_ENTRY(gtk_builder_get_object(builder, "width"));
    heightEntry = GTK_ENTRY(gtk_builder_get_object(builder, "height"));
    usernameEntry = GTK_ENTRY(gtk_builder_get_object(builder, "username"));
    domainEntry = GTK_ENTRY(gtk_builder_get_object(builder, "domain"));
    passwordEntry = GTK_ENTRY(gtk_builder_get_object(builder, "password"));
    extraArgsEntry = GTK_ENTRY(gtk_builder_get_object(builder, "extraArgs"));

    keymapInput = GTK_COMBO_BOX_TEXT(gtk_builder_get_object(builder, "keymap"));

    gtk_window_set_title(GTK_WINDOW(window), "rdg");

    gtk_builder_connect_signals(builder, NULL);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    g_object_unref(builder);

    gtk_widget_show(window);

    gtk_main();

    return 0;
}