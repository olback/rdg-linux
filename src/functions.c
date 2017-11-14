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

void prepare() {

    sprintf(ip, "%s", gtk_entry_get_text(GTK_ENTRY(ipEntry)));
    trimSpaces(ip);
    if(strcmp(ip, "") != 0) {
        sprintf(ip, "%s", gtk_entry_get_text(GTK_ENTRY(ipEntry)));
        allowConnect = 1;
    }

    sprintf(port, "%s", gtk_entry_get_text(GTK_ENTRY(portEntry)));
    trimSpaces(port);
    if(strcmp(port, "") != 0) {
        sprintf(port, "%s", gtk_entry_get_text(GTK_ENTRY(portEntry)));
    } else {
        allowConnect = 0;
    }

    sprintf(width, "%s", gtk_entry_get_text(GTK_ENTRY(widthEntry)));
    sprintf(height, "%s", gtk_entry_get_text(GTK_ENTRY(heightEntry)));
    trimSpaces(width);
    trimSpaces(height);
    char windowGeo[32];
    if(strcmp(width, "") != 0 && strcmp(height, "") != 0) {
        sprintf(windowGeo, "-g %sx%s ", width, height);
    }

    sprintf(username, "%s", gtk_entry_get_text(GTK_ENTRY(usernameEntry)));
    trimSpaces(username);
    char user[255];
    if(strcmp(username, "") != 0) {
        sprintf(user, "-u %s ", username);
    } else {
        sprintf(user, "%s", "");
    }

    sprintf(domain, "%s", gtk_entry_get_text(GTK_ENTRY(domainEntry)));
    trimSpaces(domain);
    char dom[255];
    if(strcmp(domain, "") != 0) {
        sprintf(dom, "-d %s ", domain);
    } else {
        sprintf(dom, "%s", "");
    }

    sprintf(password, "%s", gtk_entry_get_text(GTK_ENTRY(passwordEntry)));
    trimSpaces(password);
    char pass[255];
    if(strcmp(password, "") != 0) {
        sprintf(pass, "-p %s ", password);
    } else {
        sprintf(pass, "%s", "");
    }

    sprintf(extraArgs, "%s", gtk_entry_get_text(GTK_ENTRY(extraArgsEntry)));

    sprintf(connectString, "rdesktop %s%s%s%s%s%s %s:%s", user, dom, pass, windowGeo, keymap, extraArgs, ip, port);

}

void setKeymap() {
    sprintf(keymap, "-k %s ", gtk_combo_box_text_get_active_text(keymapInput));
}

void connectRDP() {
    setKeymap();
    prepare();
    if(allowConnect) {
        printf("%s\n", connectString);
        system(connectString);
        allowConnect = 0;
    }
}

void openDialog() {
    gtk_widget_show(dialog);
}

void closeDialog() {
    gtk_widget_hide(dialog);
}
