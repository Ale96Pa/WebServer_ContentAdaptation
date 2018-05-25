#define _GNU_SOURCE
#include <stdio.h>
#include <sqlite3.h>
#include <string.h>
#include <stdlib.h>
#include "caching.h"

sqlite3 * open_connection(void)
{
	sqlite3 *conn;
	int error;
	
	/*inserire il database da aprire nel formato nomedatabase.db*/
	error=sqlite3_open("Cache.db",&conn);
	if(error!=SQLITE_OK)
	{
			fprintf(stderr,"Errore open DB\n");
			exit(EXIT_FAILURE);
	}
	else{
			printf("Connessione stabilita\n");
	}
	return conn;
}


void close_connection(sqlite3 * conn){
	
	sqlite3_close(conn);
}
