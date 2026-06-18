#include <sqlite3.h>
#include "src/database/models.c"

// Function to create users table
void create_users_table(sqlite3 *db) {
    const char *query = "CREATE TABLE IF NOT EXISTS users (id INTEGER PRIMARY KEY, username TEXT, password TEXT)";
    char *errMsg = NULL;
    int rc = sqlite3_exec(db, query, NULL, NULL, &errMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error creating users table: %s\n", errMsg);
        sqlite3_free(errMsg);
    }
}

// Function to get user by username
void get_user_by_username(sqlite3 *db, const char *username, User **user) {
    sqlite3_stmt *stmt;
    const char *query = "SELECT * FROM users WHERE username = ?";
    int rc = sqlite3_prepare_v2(db, query, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error preparing query: %s\n", sqlite3_errmsg(db));
        return;
    }

    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        *user = malloc(sizeof(User));
        (*user)->id = sqlite3_column_int(stmt, 0);
        (*user)->username = strdup(sqlite3_column_text(stmt, 1));
        (*user)->password = strdup(sqlite3_column_text(stmt, 2));
    } else {
        *user = NULL;
    }

    sqlite3_finalize(stmt);
}

// Function to add user
void add_user(sqlite3 *db, const char *username, const char *password) {
    sqlite3_stmt *stmt;
    const char *query = "INSERT INTO users (username, password) VALUES (?, ?)";
    int rc = sqlite3_prepare_v2(db, query, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error preparing query: %s\n", sqlite3_errmsg(db));
        return;
    }

    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password, -1, SQLITE_STATIC);
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Error adding user: %s\n", sqlite3_errmsg(db));
    }

    sqlite3_finalize(stmt);
}