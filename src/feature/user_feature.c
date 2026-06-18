#include <gtk/gtk.h>
#include "src/database/users.c"
#include "src/api/auth_api.c"

// Function to create user feature
void create_user_feature(GtkWidget *window) {
    // Create user feature widgets
    GtkWidget *username_label = gtk_label_new("Username:");
    GtkWidget *username_entry = gtk_entry_new();
    GtkWidget *password_label = gtk_label_new("Password:");
    GtkWidget *password_entry = gtk_entry_new();
    GtkWidget *login_button = gtk_button_new_with_label("Login");
    GtkWidget *register_button = gtk_button_new_with_label("Register");

    // Add user feature widgets to window
    gtk_container_add(GTK_CONTAINER(window), username_label);
    gtk_container_add(GTK_CONTAINER(window), username_entry);
    gtk_container_add(GTK_CONTAINER(window), password_label);
    gtk_container_add(GTK_CONTAINER(window), password_entry);
    gtk_container_add(GTK_CONTAINER(window), login_button);
    gtk_container_add(GTK_CONTAINER(window), register_button);

    // Connect signals to user feature widgets
    g_signal_connect(login_button, "clicked", G_CALLBACK(on_login_button_clicked), username_entry);
    g_signal_connect(register_button, "clicked", G_CALLBACK(on_register_button_clicked), username_entry);
}

// Function to show user feature
void show_user_feature(GtkWidget *window) {
    create_user_feature(window);
    gtk_widget_show_all(window);
}