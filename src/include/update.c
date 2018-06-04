/*
 *  © olback 2018
 */

#include "vars.h"
#include <gtk/gtk.h>
#include <curl/curl.h>

// int updatePressed = FALSE; // Prevent user from spamming the update button
// int updateAvailable = FALSE;
// void runUpdate()
// {
//     if(!updatePressed && updateAvailable) {
//         printf("Running update...\n");
//         system("xdg-open https://github.com/olback/git-gud/blob/master/UPDATE.md");
//         updatePressed = TRUE;
//     }

// }

// void notifyUpdate()
// {
//     if(!devMode) {
//         char notifyMsg[128];
//         sprintf(notifyMsg, "%s%s%s", "Update ", remoteVersion, " available!");

//         notify_init(windowTitle);
//         //NotifyNotification *n = notify_notification_new (windowTitle, notifyMsg, "git");
//         NotifyNotification *n = notify_notification_new (windowTitle, notifyMsg, "git");
//         notify_notification_add_action(n, windowTitle, "Update", NOTIFY_ACTION_CALLBACK(runUpdate), NULL, NULL);
//         notify_notification_set_hint_string(n, windowTitle, notifyMsg);
//         notify_notification_set_timeout(n, 5000); // 5 seconds

//         if (!notify_notification_show(n, NULL)) {
//             printf("Failed to show notification\n");
//         }
//     }
// }

/* function prototypes to define later */
char *do_web_request(char *url);
size_t static write_callback_func(void *buffer,
                        size_t size,
                        size_t nmemb,
                        void *userp);

void *checkUpdates()
{
    sleep(1);
    char *url = "https://raw.githubusercontent.com/olback/rdg-linux/release/project.json";
    char *content = NULL;

    content = do_web_request(url);
    if(content != NULL) {
        // content[strcspn(content, "\n")] = 0;
        // sprintf(remoteVersion, "%s", content);

        // struct Version v = projectJSON(NULL);
        // printf("%s\n", v.version);


        // if(strcmp(localVersion, remoteVersion) == 0) {
        //     printf("%s%sRunning latest version%s\n", KBLD, KGRN, KNRM);
        // } else {
        //     printf("%s%sUpdate %s available%s\n", KBLD, KYEL, remoteVersion, KNRM);
        //     updateAvailable = TRUE;
        //     notifyUpdate();
        // }
    } else {
        printf("%sFailed to check for updates. Check your connection.%s\n", KYEL, KNRM);
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

    char ua = "rdg-linux updater";

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
