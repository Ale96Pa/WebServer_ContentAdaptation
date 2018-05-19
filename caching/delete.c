#define _GNU_SOURCE
#include <stdio.h>
#include <sqlite3.h>
#include <string.h>
#include <stdlib.h>



sqlite3 * Connection(void);


void delete(int id)
{
	sqlite3 *c;
	sqlite3_stmt *stm;
	char *query=NULL;
	int rc;
	c=Connection();
	asprintf(&query,"DELETE FROM cache WHERE ID='%d';",id);

	printf("%s\n",query);

	sqlite3_prepare_v2(c,query,strlen(query),&stm,NULL);

	rc=sqlite3_step(stm);
	if(rc!=SQLITE_DONE)
	{
		fprintf(stderr,"Errore inserimento dati %s\n",sqlite3_errmsg(c));
		exit(EXIT_FAILURE);
	}

	/*free stm*/
	sqlite3_finalize(stm);
	free(query);
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
	delete(1);
}
