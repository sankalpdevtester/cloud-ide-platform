#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <gtk/gtk.h>
#include <curl/curl.h>

// Define the cache structure
typedef struct CacheEntry {
    char* key;
    char* value;
    time_t ttl;
    struct CacheEntry* next;
} CacheEntry;

// Define the cache
typedef struct Cache {
    CacheEntry* head;
    CacheEntry* tail;
} Cache;

// Initialize the cache
Cache* cache_init() {
    Cache* cache = malloc(sizeof(Cache));
    cache->head = NULL;
    cache->tail = NULL;
    return cache;
}

// Add an entry to the cache
void cache_add(Cache* cache, const char* key, const char* value, int ttl) {
    CacheEntry* entry = malloc(sizeof(CacheEntry));
    entry->key = strdup(key);
    entry->value = strdup(value);
    entry->ttl = time(NULL) + ttl;
    entry->next = NULL;

    if (cache->head == NULL) {
        cache->head = entry;
        cache->tail = entry;
    } else {
        cache->tail->next = entry;
        cache->tail = entry;
    }
}

// Get a value from the cache
char* cache_get(Cache* cache, const char* key) {
    CacheEntry* current = cache->head;
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            if (current->ttl > time(NULL)) {
                return current->value;
            } else {
                // Remove expired entry
                if (current == cache->head) {
                    cache->head = current->next;
                } else {
                    CacheEntry* prev = cache->head;
                    while (prev->next != current) {
                        prev = prev->next;
                    }
                    prev->next = current->next;
                }
                if (current == cache->tail) {
                    cache->tail = prev;
                }
                free(current->key);
                free(current->value);
                free(current);
            }
        }
        current = current->next;
    }
    return NULL;
}

// Clean up the cache
void cache_cleanup(Cache* cache) {
    CacheEntry* current = cache->head;
    while (current != NULL) {
        CacheEntry* next = current->next;
        free(current->key);
        free(current->value);
        free(current);
        current = next;
    }
    free(cache);
}

// Example usage
void example_usage() {
    Cache* cache = cache_init();

    // Add some entries to the cache
    cache_add(cache, "api_response_1", "Response 1", 60); // 1 minute TTL
    cache_add(cache, "api_response_2", "Response 2", 300); // 5 minute TTL

    // Get a value from the cache
    char* value = cache_get(cache, "api_response_1");
    if (value != NULL) {
        printf("Cached value: %s\n", value);
    } else {
        printf("No cached value found\n");
    }

    // Clean up the cache
    cache_cleanup(cache);
}

// Integrate with existing API endpoint file
void api_endpoint_handler(Cache* cache, const char* key) {
    char* cached_value = cache_get(cache, key);
    if (cached_value != NULL) {
        // Return the cached value
        printf("Returning cached value: %s\n", cached_value);
    } else {
        // Make the API request and cache the response
        CURL* curl;
        CURLcode res;
        char* response;

        curl_global_init(CURL_GLOBAL_DEFAULT);
        curl = curl_easy_init();
        if(curl) {
            curl_easy_setopt(curl, CURLOPT_URL, "https://example.com/api/endpoint");
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
            res = curl_easy_perform(curl);
            if(res != CURLE_OK) {
                fprintf(stderr, "cURL error: %s\n", curl_easy_strerror(res));
            } else {
                cache_add(cache, key, response, 60); // Cache the response for 1 minute
            }
            curl_easy_cleanup(curl);
        }
        curl_global_cleanup();
    }
}

int main() {
    Cache* cache = cache_init();
    example_usage();
    api_endpoint_handler(cache, "api_response_1");
    cache_cleanup(cache);
    return 0;
}