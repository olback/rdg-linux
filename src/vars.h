/**
 *  olback.net
 *  github.com/olback/rdg-linux
 */

#define DEV FALSE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <gtk/gtk.h>

char title[64] = "rDesktop GUI 1.1";
const char rdPath[] = "/usr/bin/rdesktop";

// Global variables bla bla don't care.
char ip[16];
char port[6];
char keymap[16];
char width[5];
char height[5];
char username[255];
char domain[255];
char experience[16];
char bpp[16];
char extraRDA[512];

int allowConnect = 0;
char connectString[2048];

GtkBuilder  *builder;

GtkWidget   *window;
GtkWidget   *dialog;
GtkWidget   *rdesktopError;

GtkEntry    *ipEntryA; // "ipEntry" is pre-defined on some systems. Thanks dad :)
GtkEntry    *portEntry;
GtkEntry    *widthEntry;
GtkEntry    *heightEntry;
GtkEntry    *usernameEntry;
GtkEntry    *domainEntry;
GtkEntry    *extraArgsEntry;

GtkSwitch   *fullscreenSwitch;
GtkSwitch   *encryptionSwitch;
GtkSwitch   *compressionSwitch;
GtkSwitch   *bitmapSwitch;
GtkSwitch   *numlockSync;

GtkComboBoxText *keymapInput;
GtkComboBoxText *experienceInput;
GtkComboBoxText *bppInput;

GtkLabel    *devMode;
