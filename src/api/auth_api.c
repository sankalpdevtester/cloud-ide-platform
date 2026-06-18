#include <curl/curl.h>
#include "src/config/config.c"

// Function to handle login API request
static size_t write_callback(void *ptr, size_t size, size_t nmemb, void *userdata) {
    return size * nmemb;
}

void login_api(const char *username, const char *password) {
    CURL *curl;
    CURLcode res;
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if(curl) {
        char *url = malloc(strlen("https://example.com/login") + strlen(username) + strlen(password) + 1);
        sprintf(url, "https://example.com/login?username=%s&password=%s", username, password);
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        res = curl_easy_perform(curl);
        if(res != CURLE_OK) {
            fprintf(stderr, "cURL error: %s\n", curl_easy_strerror(res));
        }
        curl_easy_cleanup(curl);
        free(url);
    }
    curl_global_cleanup();
}

// Function to handle register API request
void register_api(const char *username, const char *password) {
    CURL *curl;
    CURLcode res;
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if(curl) {
        char *url = malloc(strlen("https://example.com/register") + strlen(username) + strlen(password) + 1);
        sprintf(url, "https://example.com/register?username=%s&password=%s", username, password);
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        res = curl_easy_perform(curl);
        if(res != CURLE_OK) {
            fprintf(stderr, "cURL error: %s\n", curl_easy_strerror(res));
        }
        curl_easy_cleanup(curl);
        free(url);
    }
    curl_global_cleanup();
}