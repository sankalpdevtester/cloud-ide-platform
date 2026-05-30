#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "config/config.h"

// Define database schema
#define PROJECT_TABLE "CREATE TABLE IF NOT EXISTS projects (id INTEGER PRIMARY KEY, name TEXT NOT NULL, description TEXT)"
#define USER_TABLE "CREATE TABLE IF NOT EXISTS users (id INTEGER PRIMARY KEY, username TEXT NOT NULL, email TEXT NOT NULL)"
#define COLLABORATOR_TABLE "CREATE TABLE IF NOT EXISTS collaborators (project_id INTEGER, user_id INTEGER, role TEXT NOT NULL, FOREIGN KEY (project_id) REFERENCES projects (id), FOREIGN KEY (user_id) REFERENCES users (id))"

// Define database connection structure
typedef struct {
    sqlite3* db;
} Database;

// Function to create a new database connection
Database* create_database(const char* db_path) {
    Database* db = malloc(sizeof(Database));
    if (sqlite3_open(db_path, &db->db) != SQLITE_OK) {
        fprintf(stderr, "Error opening database: %s\n", sqlite3_errmsg(db->db));
        exit(1);
    }
    return db;
}

// Function to close a database connection
void close_database(Database* db) {
    sqlite3_close(db->db);
    free(db);
}

// Function to create tables in the database
void create_tables(Database* db) {
    char* err_msg = NULL;
    if (sqlite3_exec(db->db, PROJECT_TABLE, NULL, NULL, &err_msg) != SQLITE_OK) {
        fprintf(stderr, "Error creating projects table: %s\n", err_msg);
        sqlite3_free(err_msg);
        exit(1);
    }
    if (sqlite3_exec(db->db, USER_TABLE, NULL, NULL, &err_msg) != SQLITE_OK) {
        fprintf(stderr, "Error creating users table: %s\n", err_msg);
        sqlite3_free(err_msg);
        exit(1);
    }
    if (sqlite3_exec(db->db, COLLABORATOR_TABLE, NULL, NULL, &err_msg) != SQLITE_OK) {
        fprintf(stderr, "Error creating collaborators table: %s\n", err_msg);
        sqlite3_free(err_msg);
        exit(1);
    }
}

// Function to insert a new project into the database
void insert_project(Database* db, const char* name, const char* description) {
    sqlite3_stmt* stmt;
    const char* query = "INSERT INTO projects (name, description) VALUES (?, ?)";
    if (sqlite3_prepare_v2(db->db, query, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Error preparing query: %s\n", sqlite3_errmsg(db->db));
        exit(1);
    }
    sqlite3_bind_text(stmt, 1, name, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, description, -1, SQLITE_STATIC);
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        fprintf(stderr, "Error executing query: %s\n", sqlite3_errmsg(db->db));
        exit(1);
    }
    sqlite3_finalize(stmt);
}

// Function to insert a new user into the database
void insert_user(Database* db, const char* username, const char* email) {
    sqlite3_stmt* stmt;
    const char* query = "INSERT INTO users (username, email) VALUES (?, ?)";
    if (sqlite3_prepare_v2(db->db, query, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Error preparing query: %s\n", sqlite3_errmsg(db->db));
        exit(1);
    }
    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, email, -1, SQLITE_STATIC);
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        fprintf(stderr, "Error executing query: %s\n", sqlite3_errmsg(db->db));
        exit(1);
    }
    sqlite3_finalize(stmt);
}

// Function to insert a new collaborator into the database
void insert_collaborator(Database* db, int project_id, int user_id, const char* role) {
    sqlite3_stmt* stmt;
    const char* query = "INSERT INTO collaborators (project_id, user_id, role) VALUES (?, ?, ?)";
    if (sqlite3_prepare_v2(db->db, query, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Error preparing query: %s\n", sqlite3_errmsg(db->db));
        exit(1);
    }
    sqlite3_bind_int(stmt, 1, project_id);
    sqlite3_bind_int(stmt, 2, user_id);
    sqlite3_bind_text(stmt, 3, role, -1, SQLITE_STATIC);
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        fprintf(stderr, "Error executing query: %s\n", sqlite3_errmsg(db->db));
        exit(1);
    }
    sqlite3_finalize(stmt);
}

int main() {
    // Create a new database connection
    Database* db = create_database("cloud_ide.db");

    // Create tables in the database
    create_tables(db);

    // Insert a new project into the database
    insert_project(db, "My Project", "This is my project");

    // Insert a new user into the database
    insert_user(db, "john_doe", "john@example.com");

    // Insert a new collaborator into the database
    insert_collaborator(db, 1, 1, "admin");

    // Close the database connection
    close_database(db);

    return 0;
}