#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <gtk/gtk.h>
#include <curl/curl.h>
#include <sqlite3.h>

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

// Get an entry from the cache
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
int main() {
    Cache* cache = cache_init();

    // Add some entries to the cache
    cache_add(cache, "api/response1", "Response 1", 60); // 1 minute TTL
    cache_add(cache, "api/response2", "Response 2", 300); // 5 minutes TTL

    // Get an entry from the cache
    char* response = cache_get(cache, "api/response1");
    if (response != NULL) {
        printf("Cached response: %s\n", response);
    } else {
        printf("No cached response found\n");
    }

    // Clean up the cache
    cache_cleanup(cache);

    return 0;
}