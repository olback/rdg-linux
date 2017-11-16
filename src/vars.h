/**
 *  olback.net
 *  github.com/olback/rdg-linux
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>

// Global variables bla bla don't care.
char ip[16];
char port[6];
char keymap[16];
char width[5];
char height[5];
char username[255];
char domain[255];
char password[255];
char extraArgs[255];

int allowConnect = 0;
char connectString[1536];

GtkBuilder  *builder;

GtkWidget   *window;
GtkWidget   *dialog;

GtkEntry    *ipEntryA; // "ipEntry" is pre-defined on some systems. Thanks dad :)
GtkEntry    *portEntry;
GtkEntry    *widthEntry;
GtkEntry    *heightEntry;
GtkEntry    *usernameEntry;
GtkEntry    *domainEntry;
GtkEntry    *passwordEntry;
GtkEntry    *extraArgsEntry;

GtkComboBoxText *keymapInput;
