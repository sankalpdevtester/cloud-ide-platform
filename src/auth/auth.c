#include <gtk/gtk.h>
#include <sqlite3.h>
#include <curl/curl.h>
#include "src/database/models.c"
#include "src/config/config.c"

// Function to handle login button click
void on_login_button_clicked(GtkWidget *widget, gpointer data) {
    // Get username and password from entry fields
    GtkWidget *username_entry = GTK_WIDGET(data);
    GtkWidget *password_entry = GTK_WIDGET(gtk_widget_get_next_sibling(username_entry));
    const gchar *username = gtk_entry_get_text(GTK_ENTRY(username_entry));
    const gchar *password = gtk_entry_get_text(GTK_ENTRY(password_entry));

    // Validate username and password
    if (strlen(username) == 0 || strlen(password) == 0) {
        gtk_label_set_text(GTK_LABEL(gtk_widget_get_next_sibling(password_entry)), "Please fill in both fields");
        return;
    }

    // Connect to database
    sqlite3 *db;
    char *errMsg = NULL;
    int rc = sqlite3_open("database.db", &db);
    if (rc) {
        gtk_label_set_text(GTK_LABEL(gtk_widget_get_next_sibling(password_entry)), "Error connecting to database");
        return;
    }

    // Query database for user
    sqlite3_stmt *stmt;
    const char *query = "SELECT * FROM users WHERE username = ? AND password = ?";
    rc = sqlite3_prepare_v2(db, query, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        gtk_label_set_text(GTK_LABEL(gtk_widget_get_next_sibling(password_entry)), "Error preparing query");
        sqlite3_close(db);
        return;
    }

    // Bind parameters
    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password, -1, SQLITE_STATIC);

    // Execute query
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        // Login successful, redirect to dashboard
        gtk_label_set_text(GTK_LABEL(gtk_widget_get_next_sibling(password_entry)), "Login successful");
        // Redirect to dashboard
        GtkWidget *dashboard_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
        gtk_window_set_title(GTK_WINDOW(dashboard_window), "Dashboard");
        gtk_window_set_default_size(GTK_WINDOW(dashboard_window), 800, 600);
        gtk_widget_show_all(dashboard_window);
    } else {
        gtk_label_set_text(GTK_LABEL(gtk_widget_get_next_sibling(password_entry)), "Invalid username or password");
    }

    // Clean up
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

// Function to handle register button click
void on_register_button_clicked(GtkWidget *widget, gpointer data) {
    // Get username, password, and confirm password from entry fields
    GtkWidget *username_entry = GTK_WIDGET(data);
    GtkWidget *password_entry = GTK_WIDGET(gtk_widget_get_next_sibling(username_entry));
    GtkWidget *confirm_password_entry = GTK_WIDGET(gtk_widget_get_next_sibling(password_entry));
    const gchar *username = gtk_entry_get_text(GTK_ENTRY(username_entry));
    const gchar *password = gtk_entry_get_text(GTK_ENTRY(password_entry));
    const gchar *confirm_password = gtk_entry_get_text(GTK_ENTRY(confirm_password_entry));

    // Validate username, password, and confirm password
    if (strlen(username) == 0 || strlen(password) == 0 || strlen(confirm_password) == 0) {
        gtk_label_set_text(GTK_LABEL(gtk_widget_get_next_sibling(confirm_password_entry)), "Please fill in all fields");
        return;
    }

    if (strcmp(password, confirm_password) != 0) {
        gtk_label_set_text(GTK_LABEL(gtk_widget_get_next_sibling(confirm_password_entry)), "Passwords do not match");
        return;
    }

    // Connect to database
    sqlite3 *db;
    char *errMsg = NULL;
    int rc = sqlite3_open("database.db", &db);
    if (rc) {
        gtk_label_set_text(GTK_LABEL(gtk_widget_get_next_sibling(confirm_password_entry)), "Error connecting to database");
        return;
    }

    // Query database for user
    sqlite3_stmt *stmt;
    const char *query = "INSERT INTO users (username, password) VALUES (?, ?)";
    rc = sqlite3_prepare_v2(db, query, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        gtk_label_set_text(GTK_LABEL(gtk_widget_get_next_sibling(confirm_password_entry)), "Error preparing query");
        sqlite3_close(db);
        return;
    }

    // Bind parameters
    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password, -1, SQLITE_STATIC);

    // Execute query
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_DONE) {
        // Registration successful, redirect to login page
        gtk_label_set_text(GTK_LABEL(gtk_widget_get_next_sibling(confirm_password_entry)), "Registration successful");
        // Redirect to login page
        GtkWidget *login_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
        gtk_window_set_title(GTK_WINDOW(login_window), "Login");
        gtk_window_set_default_size(GTK_WINDOW(login_window), 400, 300);
        gtk_widget_show_all(login_window);
    } else {
        gtk_label_set_text(GTK_LABEL(gtk_widget_get_next_sibling(confirm_password_entry)), "Error registering user");
    }

    // Clean up
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}