#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>


int main (void){
	sqlite3 *conn;
	int error=0;
	char *err_msg;
	int err=0;
	error=sqlite3_open("C.sl3",&conn);
	if(error!=SQLITE_OK){
			fprintf(stderr,"Non riesco ad aprire il DB\n");
			exit(EXIT_FAILURE);}
	
	char *sql="CREATE TABLE PROVA(ID INT)";
	//error=sqlite3_exec(conn,"INSERT INTO cache VALUES(1,'Mozilla');",0,0,&err_msg);
	err=sqlite3_exec(conn,sql,0,0,&err_msg);

	printf("%d\n",*err_msg);
	if(err!=SQLITE_OK){
			fprintf(stderr,"Errore nell'inserimento\n");
			exit(EXIT_FAILURE);}
	return 0;
}		
