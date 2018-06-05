/*
 *  © olback 2018
 */

#include "vars.h"
#include <pthread.h>
#include <gtk/gtk.h>
#include <curl/curl.h>
#include <libnotify/notify.h>

void runUpdate() {
    struct Version v = projectJSON(NULL);
    char updateCommand[354];
    sprintf(updateCommand, "xdg-open %s", v.update_url);
    system(updateCommand);
}

void notifyUpdate(struct Version v)
{
    char notifyMsg[128];
    sprintf(notifyMsg, "%s %1.1f %s", "Update", v.version, "available!");

    notify_init(v.name);
    NotifyNotification *n = notify_notification_new (v.name, notifyMsg, "package-x-generic");
    notify_notification_add_action(n, v.name, "Update", NOTIFY_ACTION_CALLBACK(runUpdate), NULL, NULL);
    notify_notification_set_hint_string(n, v.name, notifyMsg);
    notify_notification_set_timeout(n, 5000); // 5 seconds

    if (!notify_notification_show(n, NULL)) {
        printf("Failed to show notification\n");
    }
}

/* function prototypes to define later */
char *do_web_request(char *url);
size_t static write_callback_func(void *buffer,
                        size_t size,
                        size_t nmemb,
                        void *userp);

void *checkUpdates()
{
    if(dev) {
        printf("%sNot cheking for updates in dev mode!%s\n", KYEL, KNRM);
        pthread_join(t_update, NULL);
        return NULL;
    }

    sleep(1);
    char *url = "https://raw.githubusercontent.com/olback/rdg-linux/master/project.json";
    char *content = NULL;

    content = do_web_request(url);

    if(content != NULL) {

        struct Version vl = projectJSON(NULL);
        printf("\nLocal version: %1.1f\n", vl.version);

        struct Version vr = projectJSON(content);
        printf("Remote version: %1.1f\n", vr.version);

        if(vl.version >= vr.version) {
            printf("%sRunning latest version%s\n", KGRN, KNRM);
        } else {
            printf("%sUpdate available%s\n", KYEL, KNRM);
            notifyUpdate(vr);
        }

    } else {

        printf("%sFailed to check for updates. Check your connection.%s\n", KRED, KNRM);

    }

    pthread_join(t_update, NULL);
    return NULL;
}

/* the function to return the content for a url */
char *do_web_request(char *url)
{
    /* keeps the handle to the curl object */
    CURL *curl_handle = NULL;

    /* to keep the response */
    char *response = NULL;

    char ua[] = "rdg-linux updater";

    /* initializing curl and setting the url */
    curl_handle = curl_easy_init();
    curl_easy_setopt(curl_handle, CURLOPT_URL, url);
    curl_easy_setopt(curl_handle, CURLOPT_HTTPGET, 1);
    curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, ua);

    /* follow locations specified by the response header */
    curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, 1);

    /* setting a callback function to return the data */
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_callback_func);

    /* passing the pointer to the response as the callback parameter */
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &response);

    long int response_code;

    /* perform the request */
    curl_easy_perform(curl_handle);

    /* cleaning all curl stuff */
    curl_easy_cleanup(curl_handle);

    return response;
}

/* the function to invoke as the data recieved */
size_t static write_callback_func(void *buffer,
                        size_t size,
                        size_t nmemb,
                        void *userp)
{
    char **response_ptr =  (char**)userp;

    /* assuming the response is a string */
    *response_ptr = strndup(buffer, (size_t)(size *nmemb));

    return 0;
}
