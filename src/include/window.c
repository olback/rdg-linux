/**
 *  olback.net © 2018
 *  github.com/olback/rdg-linux
 */

#include "vars.h"
#include <gtk/gtk.h>
// #include <cjson/cJSON.h>
#include "../lib/cJSON.c"

void openDialog() {
    gtk_widget_show(dialog);
}

void closeDialog() {
    gtk_widget_hide(dialog);
}

struct Version projectJSON(char jsonIn[]);

void menu_about() {
    char vString[5];
    struct Version v = projectJSON(NULL);
    sprintf(vString, "%1.1f", v.version);

    gtk_about_dialog_set_program_name(about, v.name);
    gtk_about_dialog_set_version(about, vString);
    gtk_about_dialog_set_website (about, v.project_url);
    gtk_widget_show(GTK_WIDGET(about));
}

void about_close() {
    gtk_widget_hide(GTK_WIDGET(about));
}

void menu_help() {
    struct Version v = projectJSON(NULL);
    char updateCommand[354];
    sprintf(updateCommand, "xdg-open %s", v.issue_url);
    system(updateCommand);
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

struct Version projectJSON(char jsonIn[]) {

    char json[4096];

    if (jsonIn != NULL) {
        sprintf(json, "%s", jsonIn);
    } else {
        GInputStream *json_stream = g_resources_open_stream("/net/olback/rdg/project.json", G_RESOURCE_LOOKUP_FLAGS_NONE, NULL);
        g_input_stream_read(json_stream, json, 1024, NULL, NULL);
    }

    cJSON *name = NULL;
    cJSON *version = NULL;
    cJSON *type = NULL;
    cJSON *project_url = NULL;
    cJSON *update_url = NULL;
    cJSON *issue_url = NULL;

    cJSON *data = cJSON_Parse(json);

    if(data == NULL) {
        printf("%s%sError parsing json%s\n", KRED, KBLD, KNRM);
        exit(-1);
    }

    name = cJSON_GetObjectItemCaseSensitive(data, "name");
    version = cJSON_GetObjectItemCaseSensitive(data, "version");
    type = cJSON_GetObjectItemCaseSensitive(data, "type");
    project_url = cJSON_GetObjectItemCaseSensitive(data, "project_url");
    update_url = cJSON_GetObjectItemCaseSensitive(data, "update_url");
    issue_url = cJSON_GetObjectItemCaseSensitive(data, "issue_url");

    struct Version v;

    sprintf(v.name, "%s", name->valuestring);
    v.version = version->valuedouble;
    sprintf(v.type, "%s", type->valuestring);
    sprintf(v.project_url, "%s", project_url->valuestring);
    sprintf(v.update_url, "%s", update_url->valuestring);
    sprintf(v.issue_url, "%s", issue_url->valuestring);

    cJSON_Delete(data);

    return v;
}
