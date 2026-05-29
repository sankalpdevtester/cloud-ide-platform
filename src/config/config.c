#include <gtk/gtk.h>
#include <sqlite3.h>
#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define configuration structure
typedef struct {
    char *db_path;
    char *api_url;
    char *username;
    char *password;
} Config;

// Function to initialize configuration
Config *config_init() {
    Config *config = malloc(sizeof(Config));
    config->db_path = NULL;
    config->api_url = NULL;
    config->username = NULL;
    config->password = NULL;
    return config;
}

// Function to load configuration from file
void config_load(Config *config, const char *file_path) {
    // Load configuration from file
    FILE *file = fopen(file_path, "r");
    if (file == NULL) {
        printf("Error loading configuration file\n");
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        // Remove newline character
        line[strcspn(line, "\n")] = 0;

        // Parse configuration options
        if (strncmp(line, "db_path=", 8) == 0) {
            config->db_path = strdup(line + 8);
        } else if (strncmp(line, "api_url=", 8) == 0) {
            config->api_url = strdup(line + 8);
        } else if (strncmp(line, "username=", 9) == 0) {
            config->username = strdup(line + 9);
        } else if (strncmp(line, "password=", 9) == 0) {
            config->password = strdup(line + 9);
        }
    }

    fclose(file);
}

// Function to save configuration to file
void config_save(Config *config, const char *file_path) {
    // Save configuration to file
    FILE *file = fopen(file_path, "w");
    if (file == NULL) {
        printf("Error saving configuration file\n");
        return;
    }

    fprintf(file, "db_path=%s\n", config->db_path);
    fprintf(file, "api_url=%s\n", config->api_url);
    fprintf(file, "username=%s\n", config->username);
    fprintf(file, "password=%s\n", config->password);

    fclose(file);
}

// Function to initialize GTK+ UI
void config_init_ui(Config *config) {
    // Initialize GTK+ UI
    gtk_init(NULL, NULL);

    // Create main window
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Cloud Integrated Development Environment");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);

    // Create configuration dialog
    GtkWidget *dialog = gtk_dialog_new_with_buttons("Configuration", GTK_WINDOW(window), GTK_DIALOG_MODAL, "_OK", GTK_RESPONSE_OK, "_Cancel", GTK_RESPONSE_CANCEL, NULL);

    // Create configuration entries
    GtkWidget *db_path_entry = gtk_entry_new();
    GtkWidget *api_url_entry = gtk_entry_new();
    GtkWidget *username_entry = gtk_entry_new();
    GtkWidget *password_entry = gtk_entry_new();

    // Set configuration entries
    gtk_entry_set_text(GTK_ENTRY(db_path_entry), config->db_path);
    gtk_entry_set_text(GTK_ENTRY(api_url_entry), config->api_url);
    gtk_entry_set_text(GTK_ENTRY(username_entry), config->username);
    gtk_entry_set_text(GTK_ENTRY(password_entry), config->password);

    // Add configuration entries to dialog
    gtk_box_pack_start(GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(dialog))), db_path_entry, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(dialog))), api_url_entry, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(dialog))), username_entry, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(dialog))), password_entry, TRUE, TRUE, 0);

    // Show configuration dialog
    gtk_widget_show_all(dialog);

    // Run GTK+ main loop
    gtk_main();
}

// Function to initialize SQLite database
void config_init_db(Config *config) {
    // Initialize SQLite database
    sqlite3 *db;
    int rc = sqlite3_open(config->db_path, &db);
    if (rc) {
        printf("Error opening database: %s\n", sqlite3_errmsg(db));
        return;
    }

    // Create tables
    const char *sql = "CREATE TABLE IF NOT EXISTS projects (id INTEGER PRIMARY KEY, name TEXT NOT NULL)";
    rc = sqlite3_exec(db, sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        printf("Error creating table: %s\n", sqlite3_errmsg(db));
        return;
    }

    // Close database
    sqlite3_close(db);
}

// Function to initialize libcurl API
void config_init_api(Config *config) {
    // Initialize libcurl API
    CURL *curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, config->api_url);
        res = curl_easy_perform(curl);
        if(res != CURLE_OK) {
            printf("Error performing API request: %s\n", curl_easy_strerror(res));
        }
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
}

int main() {
    // Initialize configuration
    Config *config = config_init();

    // Load configuration from file
    config_load(config, "config.txt");

    // Initialize GTK+ UI
    config_init_ui(config);

    // Initialize SQLite database
    config_init_db(config);

    // Initialize libcurl API
    config_init_api(config);

    // Save configuration to file
    config_save(config, "config.txt");

    return 0;
}