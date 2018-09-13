#include <sqlite3.h>

int sqlite3_open(
  const char *filename,   /* Database filename (UTF-8) */
  sqlite3 **ppDb          /* OUT: SQLite db handle */
) {
    return 0;
}

int sqlite3_open_v2(
  const char *filename,   /* Database filename (UTF-8) */
  sqlite3 **ppDb,         /* OUT: SQLite db handle */
  int flags,              /* Flags */
  const char *zVfs        /* Name of VFS module to use */
) {
    return 0;
}

int sqlite3_prepare_v2(
  sqlite3 *db,            /* Database handle */
  const char *zSql,       /* SQL statement, UTF-8 encoded */
  int nByte,              /* Maximum length of zSql in bytes. */
  sqlite3_stmt **ppStmt,  /* OUT: Statement handle */
  const char **pzTail     /* OUT: Pointer to unused portion of zSql */
) {
    return 0;
}

int sqlite3_finalize(sqlite3_stmt *pStmt) {
    return 0;
}

int sqlite3_close(sqlite3*) {
    return 0;
}

int sqlite3_close_v2(sqlite3*) {
    return 0;
}

int sqlite3_exec(
  sqlite3*,                                  /* An open database */
  const char *sql,                           /* SQL to be evaluated */
  int (*callback)(void*,int,char**,char**),  /* Callback function */
  void *,                                    /* 1st argument to callback */
  char **errmsg                              /* Error msg written here */
) {
    return 0;
}

int sqlite3_bind_int(sqlite3_stmt*, int, int) {
    return 0;
}

int sqlite3_bind_int64(sqlite3_stmt*, int, sqlite3_int64) {
    return 0;
}

int sqlite3_bind_text(sqlite3_stmt*,int,const char*,int,void(*)(void*)) {
    return 0;
}

int sqlite3_reset(sqlite3_stmt *pStmt) {
    return 0;
}

char *sqlite3_mprintf(const char*,...) {
    return 0;
}

void sqlite3_free(void*) {

}

int sqlite3_step(sqlite3_stmt*) {
    return 0;
}

int sqlite3_column_count(sqlite3_stmt *pStmt) {
    return 0;
}

int sqlite3_column_int(sqlite3_stmt*, int iCol) {
    return 0;
}

sqlite3_int64 sqlite3_column_int64(sqlite3_stmt*, int iCol) {
    return 0;
}

const unsigned char *sqlite3_column_text(sqlite3_stmt*, int iCol) {
    return 0;
}

const char *sqlite3_column_name(sqlite3_stmt*, int N) {
    static char *name = "Name";
    return name;
}

const char *sqlite3_errmsg(sqlite3*) {
    static char *errmsg = "Hello";
    return errmsg;
}