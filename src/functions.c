/**
 *  olback.net
 *  github.com/olback/rdg-linux
 */

void trimSpaces(char* source) {
    char* i = source;
    char* j = source;
    while(*j != 0) {
        *i = *j++;
        if(*i != ' ')
            i++;
    }
    *i = 0;
}

void prepareIP() {
    sprintf(ip, "%s", gtk_entry_get_text(GTK_ENTRY(ipEntryA)));
    trimSpaces(ip);
    if(strcmp(ip, "") != 0) {
        allowConnect = 1;
    }
}

void preparePort() {
    sprintf(port, "%s", gtk_entry_get_text(GTK_ENTRY(portEntry)));
    trimSpaces(port);
    if(strcmp(port, "") == 0) {
        allowConnect = 0;
    }
}

char user[255];
void prepareUsername() {
    sprintf(username, "%s", gtk_entry_get_text(GTK_ENTRY(usernameEntry)));
    trimSpaces(username);
    if(strcmp(username, "") != 0) {
        sprintf(user, "-u %s ", username);
    } else {
        sprintf(user, "%s", "");
    }
}

char dom[255];
void prepareDomain() {
    sprintf(domain, "%s", gtk_entry_get_text(GTK_ENTRY(domainEntry)));
    trimSpaces(domain);
    if(strcmp(domain, "") != 0) {
        sprintf(dom, "-d %s ", domain);
    } else {
        sprintf(dom, "%s", "");
    }
}

char windowGeo[32];
void prepareWindowGeo() {
    sprintf(width, "%s", gtk_entry_get_text(GTK_ENTRY(widthEntry)));
    sprintf(height, "%s", gtk_entry_get_text(GTK_ENTRY(heightEntry)));
    trimSpaces(width);
    trimSpaces(height);
    if(strcmp(width, "") != 0 && strcmp(height, "") != 0) {
        sprintf(windowGeo, "-g %sx%s ", width, height);
    }
}

void getExtraRDA() {

    sprintf(extraRDA, "%s", ""); // Reset extraRDA.

    sprintf(extraRDA, "%s-k %s ", extraRDA, gtk_combo_box_text_get_active_text(keymapInput));

    sprintf(extraRDA, "%s-x %s ", extraRDA, gtk_combo_box_text_get_active_text(experienceInput));

    sprintf(extraRDA, "%s-a %s ", extraRDA, gtk_combo_box_text_get_active_text(bppInput));

    if(gtk_switch_get_state(GTK_SWITCH(fullscreenSwitch))) {
        sprintf(extraRDA, "%s%s", extraRDA, "-f ");
    }

    if(gtk_switch_get_state(GTK_SWITCH(encryptionSwitch))) {
        sprintf(extraRDA, "%s%s", extraRDA, "-E ");
    }

    if(gtk_switch_get_state(GTK_SWITCH(compressionSwitch))) {
        sprintf(extraRDA, "%s%s", extraRDA, "-z ");
    }

    if(gtk_switch_get_state(GTK_SWITCH(bitmapSwitch))) {
        sprintf(extraRDA, "%s%s", extraRDA, "-P ");
    }

    if(gtk_switch_get_state(GTK_SWITCH(numlockSync))) {
        sprintf(extraRDA, "%s%s", extraRDA, "-N ");
    }

    sprintf(extraRDA, "%s%s", extraRDA, gtk_entry_get_text(GTK_ENTRY(extraArgsEntry)));

}

void prepareConnectString() {
    sprintf(connectString, "rdesktop %s%s%s%s %s:%s &", user, dom, windowGeo, extraRDA, ip, port);
}

void connectRDP() {
    prepareIP();
    preparePort();
    if(allowConnect) {
        prepareUsername();
        prepareDomain();
        prepareWindowGeo();
        getExtraRDA();
        prepareConnectString();
        printf("%s\n", connectString);
        if(!DEV) {
            system(connectString);
        }
        allowConnect = 0;
    }
}

void openDialog() {
    gtk_widget_show(dialog);
}

void closeDialog() {
    gtk_widget_hide(dialog);
}
