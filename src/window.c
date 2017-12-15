/**
 *  olback.net © 2017
 *  github.com/olback/rdg-linux
 */

void openDialog() {
    gtk_widget_show(dialog);
}

void closeDialog() {
    gtk_widget_hide(dialog);
}

void rdExists() {
    if(access(rdPath, R_OK) == -1) {
        printf("Could not find rdesktop, make sure you have it installed.\n");
        gtk_widget_show(rdesktopError);
    }
}

void closeRdesktopError() {
    gtk_widget_hide(rdesktopError);
}

void startRDG() {

    char path[PATH_MAX];
    char dest[PATH_MAX];
    memset(dest, 0, sizeof(dest)); // readlink does not null terminate!
    struct stat;
    pid_t pid = getpid();
    sprintf(path, "/proc/%d/exe", pid);
    if(readlink(path, dest, PATH_MAX) == -1){
        perror("readlink");
    } else {
        //printf("%s\n", dest);
        sprintf(rdgPath, "%s", dest);
    }

}
