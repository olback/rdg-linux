/**
 *  olback.net © 2018
 *  github.com/olback/rdg-linux
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <gtk/gtk.h>
#include "lib/cJSON.c"
#include "include/vars.h"
#include "include/connect.c"
#include "include/window.c"
#include "include/resources.c"

int main(int argc, char *argv[])
{
    struct Version v = projectJSON();
    char title[64];

    if(strcmp(v.type, "dev") == 0) {
        sprintf(title, "%s %1.1f DEV", v.name, v.version);
        DEV = TRUE;
    } else {
        sprintf(title, "%s %1.1f", v.name, v.version);
        if(argc == 1) {
            char startString[PATH_MAX];
            sprintf(startString, "%s -start & disown", argv[0]);
            system(startString);
            exit(0);
        }
    }
    
    gtk_init(&argc, &argv);

    builder = gtk_builder_new();
    gtk_builder_add_from_resource(builder, "/net/olback/rdg/src/layout/rdg.glade", NULL);

    window = GTK_WIDGET(gtk_builder_get_object(builder, "main"));
    ipEntryA = GTK_ENTRY(gtk_builder_get_object(builder, "ip"));
    portEntry = GTK_ENTRY(gtk_builder_get_object(builder, "port"));
    widthEntry = GTK_ENTRY(gtk_builder_get_object(builder, "width"));
    heightEntry = GTK_ENTRY(gtk_builder_get_object(builder, "height"));
    usernameEntry = GTK_ENTRY(gtk_builder_get_object(builder, "username"));
    domainEntry = GTK_ENTRY(gtk_builder_get_object(builder, "domain"));
    keymapInput = GTK_COMBO_BOX_TEXT(gtk_builder_get_object(builder, "keymap"));
    devMode = GTK_LABEL(gtk_builder_get_object(builder, "dev"));

    dialog = GTK_WIDGET(gtk_builder_get_object(builder, "dialog"));
    experienceInput = GTK_COMBO_BOX_TEXT(gtk_builder_get_object(builder, "experience"));
    bppInput = GTK_COMBO_BOX_TEXT(gtk_builder_get_object(builder, "bpp"));
    fullscreenSwitch = GTK_SWITCH(gtk_builder_get_object(builder, "fullscreen"));
    encryptionSwitch = GTK_SWITCH(gtk_builder_get_object(builder, "encryption"));
    compressionSwitch = GTK_SWITCH(gtk_builder_get_object(builder, "compression"));
    bitmapSwitch = GTK_SWITCH(gtk_builder_get_object(builder, "cacheBitmaps"));
    numlockSync = GTK_SWITCH(gtk_builder_get_object(builder, "numlockSync"));
    extraArgsEntry = GTK_ENTRY(gtk_builder_get_object(builder, "extraArgs"));

    rdesktopError = GTK_WIDGET(gtk_builder_get_object(builder, "rdesktopError"));

    gtk_builder_connect_signals(builder, NULL);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    g_object_unref(builder);

    gtk_window_set_title(GTK_WINDOW(window), title);

    if(DEV) {
        gtk_label_set_text(GTK_LABEL(devMode), "DEV MODE");
    }

    gtk_widget_show(window);

    rdExists();

    gtk_main();

    return 0;
}