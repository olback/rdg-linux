/**
 *  olback.net
 *  github.com/olback/rdg-linux
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>

// Global variables bla bla don't care.
char ip[15];
char port[5];
char keymap[5];
char width[4];
char height[4];
char username[255];
char domain[255];
char password[255];
char extraArgs[255];

int allowConnect = 0;
char connectString[1024];

GtkBuilder  *builder;

GtkWidget   *window;
GtkWidget   *dialog;

GtkEntry    *ipEntry;
GtkEntry    *portEntry;
GtkEntry    *widthEntry;
GtkEntry    *heightEntry;
GtkEntry    *usernameEntry;
GtkEntry    *domainEntry;
GtkEntry    *passwordEntry;
GtkEntry    *extraArgsEntry;

GtkComboBoxText *keymapInput;
