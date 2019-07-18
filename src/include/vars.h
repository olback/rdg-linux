/**
 *  olback.net © 2018
 *  github.com/olback/rdg-linux
 */

#include <gtk/gtk.h>
#include <pthread.h>

#ifndef __RDG_VARS

#define __RDG_VARS

#define false 0;
#define true 1;

// Dev mode?
int dev = true;

// Path to rDesktop binary
const char rdPath[] = "/usr/bin/rdesktop";

struct Version
{
    char name[16];
    double version;
    char type[8];
    char project_url[255];
    char update_url[255];
    char issue_url[255];
};

// Define terminal colors
#define KNRM "\x1B[0m"
#define KRED "\x1B[31m"
#define KGRN "\x1B[32m"
#define KYEL "\x1B[33m"
#define KBLU "\x1B[34m"
#define KMAG "\x1B[35m"
#define KCYN "\x1B[36m"
#define KWHT "\x1B[37m"
#define KBLD "\x1B[1m"

// Global variables bla bla don't care.
char ip[255];
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
char rdgPath[PATH_MAX];

GtkBuilder *builder;

GtkWidget *window;
GtkWidget *dialog;
GtkWidget *rdesktopError;
GtkAboutDialog *about;

GtkEntry *ipEntryA; // "ipEntry" is pre-defined on some systems.
GtkEntry *portEntry;
GtkEntry *widthEntry;
GtkEntry *heightEntry;
GtkEntry *usernameEntry;
GtkEntry *domainEntry;
GtkEntry *extraArgsEntry;

GtkSwitch *fullscreenSwitch;
GtkSwitch *encryptionSwitch;
GtkSwitch *compressionSwitch;
GtkSwitch *bitmapSwitch;
GtkSwitch *numlockSync;

GtkComboBoxText *keymapInput;
GtkComboBoxText *experienceInput;
GtkComboBoxText *bppInput;

GtkLabel *devMode;

pthread_t t_update;

#endif
