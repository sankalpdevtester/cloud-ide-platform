#include <gtk/gtk.h>
#include <sqlite3.h>
#include <curl/curl.h>
#include "src/database/models.c"
#include "src/config/config.c"

// Function to handle user registration
void register_user(GtkWidget *widget, gpointer data) {
    // Get user input from UI
    GtkWidget *username_entry = GTK_WIDGET(gtk_builder_get_object(GTK_BUILDER(data), "username_entry"));
    GtkWidget *password_entry = GTK_WIDGET(gtk_builder_get_object(GTK_BUILDER(data), "password_entry"));
    const gchar *username = gtk_entry_get_text(GTK_ENTRY(username_entry));
    const gchar *password = gtk_entry_get_text(GTK_ENTRY(password_entry));

    // Validate user input
    if (strlen(username) < 3 || strlen(password) < 8) {
        gtk_label_set_text(GTK_LABEL(gtk_builder_get_object(GTK_BUILDER(data), "error_label")), "Invalid username or password");
        return;
    }

    // Connect to database
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc = sqlite3_open("database.db", &db);
    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return;
    }

    // Insert user into database
    char *sql = "INSERT INTO users (username, password) VALUES (?, ?)";
    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, password, -1, SQLITE_STATIC);
        rc = sqlite3_step(stmt);
        if (rc != SQLITE_DONE) {
            fprintf(stderr, "SQL error: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);
        }
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    // Show success message
    gtk_label_set_text(GTK_LABEL(gtk_builder_get_object(GTK_BUILDER(data), "success_label")), "User registered successfully");
}

// Function to handle user login
void login_user(GtkWidget *widget, gpointer data) {
    // Get user input from UI
    GtkWidget *username_entry = GTK_WIDGET(gtk_builder_get_object(GTK_BUILDER(data), "username_entry"));
    GtkWidget *password_entry = GTK_WIDGET(gtk_builder_get_object(GTK_BUILDER(data), "password_entry"));
    const gchar *username = gtk_entry_get_text(GTK_ENTRY(username_entry));
    const gchar *password = gtk_entry_get_text(GTK_ENTRY(password_entry));

    // Validate user input
    if (strlen(username) < 3 || strlen(password) < 8) {
        gtk_label_set_text(GTK_LABEL(gtk_builder_get_object(GTK_BUILDER(data), "error_label")), "Invalid username or password");
        return;
    }

    // Connect to database
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc = sqlite3_open("database.db", &db);
    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return;
    }

    // Query user from database
    char *sql = "SELECT * FROM users WHERE username = ? AND password = ?";
    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, password, -1, SQLITE_STATIC);
        rc = sqlite3_step(stmt);
        if (rc == SQLITE_ROW) {
            // User found, show success message
            gtk_label_set_text(GTK_LABEL(gtk_builder_get_object(GTK_BUILDER(data), "success_label")), "User logged in successfully");
        } else {
            // User not found, show error message
            gtk_label_set_text(GTK_LABEL(gtk_builder_get_object(GTK_BUILDER(data), "error_label")), "Invalid username or password");
        }
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}