#include <gtk/gtk.h>
#include "src/auth/auth.c"

// Function to create authentication UI
void create_auth_ui(GtkBuilder *builder) {
    // Create registration form
    GtkWidget *registration_form = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(builder), registration_form);

    GtkWidget *username_label = gtk_label_new("Username:");
    gtk_box_pack_start(GTK_BOX(registration_form), username_label, FALSE, FALSE, 0);

    GtkWidget *username_entry = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(registration_form), username_entry, FALSE, FALSE, 0);

    GtkWidget *password_label = gtk_label_new("Password:");
    gtk_box_pack_start(GTK_BOX(registration_form), password_label, FALSE, FALSE, 0);

    GtkWidget *password_entry = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(registration_form), password_entry, FALSE, FALSE, 0);

    GtkWidget *register_button = gtk_button_new_with_label("Register");
    gtk_box_pack_start(GTK_BOX(registration_form), register_button, FALSE, FALSE, 0);

    // Create login form
    GtkWidget *login_form = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(builder), login_form);

    GtkWidget *login_username_label = gtk_label_new("Username:");
    gtk_box_pack_start(GTK_BOX(login_form), login_username_label, FALSE, FALSE, 0);

    GtkWidget *login_username_entry = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(login_form), login_username_entry, FALSE, FALSE, 0);

    GtkWidget *login_password_label = gtk_label_new("Password:");
    gtk_box_pack_start(GTK_BOX(login_form), login_password_label, FALSE, FALSE, 0);

    GtkWidget *login_password_entry = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(login_form), login_password_entry, FALSE, FALSE, 0);

    GtkWidget *login_button = gtk_button_new_with_label("Login");
    gtk_box_pack_start(GTK_BOX(login_form), login_button, FALSE, FALSE, 0);

    // Connect signals
    g_signal_connect(register_button, "clicked", G_CALLBACK(register_user), builder);
    g_signal_connect(login_button, "clicked", G_CALLBACK(login_user), builder);
}