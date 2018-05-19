#define _GNU_SOURCE
#include <stdio.h>
#include <sqlite3.h>
#include <string.h>
#include <stdlib.h>



sqlite3 * Connection(void);


void insert(int id,char *path,char *nome,char *q,char *mod,char *ua)
{
	sqlite3 *c;
	sqlite3_stmt *stm;
	char *query=NULL;
	int rc;
	c=Connection();
	asprintf(&query,"INSERT INTO cache VALUES('%d','%s','%s','%s','%s','%s');",id,nome,path,q,mod,ua);

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
	/*chiudi la connessione*/
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
	}
	else{
		printf("Connessione stabilita\n");
	}
	
	return conn;
}


int main(void){
	insert(1,"home/alessio/","img1","0.8","12-02-2002 13:57","Mozilla");
	insert(2,"/home/","img2","0.7","12-02-2002 12:00","chrome");
	insert(3,"/home/alessio/Scarcati","img3","0.8","13-12-2002 12:01","Mozilla");
	insert(1,"home/alessio/","img1","0.5","12-02-2002 11:00","chrome");
}
