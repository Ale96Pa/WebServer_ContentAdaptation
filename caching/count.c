#define _GNU_SOURCE
#include <stdio.h>
#include <sqlite3.h>
#include <string.h>
#include <stdlib.h>

//TODO: Da decidere in che modo e in quale struttura dati salvare i dati che ottengo da una select

sqlite3 * Connection(void);


int callback(void *arg, int argc, char **argv, char **colName)
{
    int i;
    for(i=0; i<argc; i++)
    {
        printf("%s = %s\t", colName[i], argv[i] ?  : "NULL");
    }
    printf("\n");
    return 0;
}


void count()
{
	sqlite3 *c;
	char *errmsg;
	int rc;
	c=Connection();
	rc = sqlite3_exec(c,"select count(*) as tot from cache;",callback, NULL, &errmsg);

	if (errmsg != NULL)
	{
		printf("Error in sqlite3_exec: %s\n", errmsg);
		sqlite3_free(errmsg);
	}

	/*chiudo la connesione*/
	sqlite3_close(c);
}		

sqlite3 * Connection(void)
{
	sqlite3 *conn;
	int error;

	error=sqlite3_open("Cache.db",&conn);
	if(error!=SQLITE_OK)
	{
		fprintf(stderr,"Errore open DB\n");
		exit(EXIT_FAILURE);
	}else{
		printf("Connessione stabilita\n");
	}
	
	return conn;
}


int main(void)
{
	count();
}
