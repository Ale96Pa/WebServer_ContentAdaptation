//TODO: aggiungi commenti

#define _GNU_SOURCE
#include "caching.h"

sqlite3 * open_connection(void)
{
	sqlite3 *connection;
	int error = sqlite3_open("caching/Cache.db",&connection);
	if(error != SQLITE_OK)
	{
        fprintf(stderr,"Error opening DB\n");
        exit(EXIT_FAILURE);
	}
	/*
	else{
        printf("Connessione stabilita\n");
	}*/
	return connection;
}

void close_connection(sqlite3 * conn)
{
	sqlite3_close(conn);
}

int callback(void *arg, int argc, char **argv, char **colName)
{
	int i;
	for(i=0; i<argc; i++){
		printf("%s = %s\t", colName[i], argv[i] ?  : "NULL");
	}
	printf("\n");
	return 0;
}

void create_table(sqlite3 *conn)
{
	char *zErrMsg = 0;
	int result;
	char *sql;

	sql = "CREATE TABLE cache("  \
         "ID INT PRIMARY KEY     NOT NULL," \
         "Nome          TEXT    NOT NULL," \
         "Path           CHAR(100)     NOT NULL," \
         "Qualità        TEXT," \
         "Last_Modified  CHAR(100)," \
		 "User_Agent     CHAR(100));";
	result = sqlite3_exec(conn, sql, callback, 0, &zErrMsg);

	if(result != SQLITE_OK)
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	} else {
		fprintf(stdout, "Table created successfully\n");
	}
	close_connection(conn);
}

