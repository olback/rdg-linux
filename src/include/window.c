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

// char *selfPath() {

//     char path[PATH_MAX];
//     char *dest = (char *)malloc(sizeof(char) * PATH_MAX);
//     memset(dest, 0, sizeof(dest)); // readlink does not null terminate!
//     pid_t pid = getpid();
//     sprintf(path, "/proc/%d/exe", pid);
//     if(readlink(path, dest, PATH_MAX) == -1){
//         perror("readlink");
//     }

//     return dest;
// }

struct Version projectJSON() {
    char json[1024];
    GInputStream *json_stream = g_resources_open_stream("/net/olback/rdg/project.json", G_RESOURCE_LOOKUP_FLAGS_NONE, NULL);
    g_input_stream_read(json_stream, json, 1024, NULL, NULL);

    cJSON *name = NULL;
    cJSON *version = NULL;
    cJSON *type = NULL;

    cJSON *data = cJSON_Parse(json);

    if(data == NULL) {
        printf("%s%sError parsing json%s\n", KRED, KBLD, KNRM);
        exit(-1);
    }

    name = cJSON_GetObjectItemCaseSensitive(data, "name");
    version = cJSON_GetObjectItemCaseSensitive(data, "version");
    type = cJSON_GetObjectItemCaseSensitive(data, "type");

    struct Version v;

    sprintf(v.name, "%s", name->valuestring);
    v.version = version->valuedouble;
    sprintf(v.type, "%s", type->valuestring);

    cJSON_Delete(data);

    return v;
}
