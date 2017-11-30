/**
 *  olback.net © 2017
 *  github.com/olback/rdg-linux
 */

#include "vars.h"
#include "functions.c"
#include "resources.c"

int main(int argc, char *argv[])
{  
    if(!DEV && argc == 1) {
        system("./rdg -start & disown");
        exit(1);
    }
    
    gtk_init(&argc, &argv);

    builder = gtk_builder_new();
    gtk_builder_add_from_resource(builder, "/net/olback/rdg/glade_file", NULL);

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

    if(DEV) {
        char devModeText[] = "DEV MODE";
        sprintf(title, "%s %s", title, devModeText);
        gtk_label_set_text(GTK_LABEL(devMode), devModeText);
        gtk_window_set_title(GTK_WINDOW(window), title);
        printf("%s\n", devModeText);
    } else {
        gtk_window_set_title(GTK_WINDOW(window), title);
    }

    gtk_widget_show(window);

    rdExists();

    gtk_main();

    return 0;
}