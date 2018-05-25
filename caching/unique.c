//TODO: aggiornamento di last modified NON funziona
//TODO: aggiungere una restituzione dell'ID piu' grande possibile


#include "caching.h"


int callback(void *arg, int argc, char **argv, char **colName) {                            
    int i;
    for(i=0; i<argc; i++){
        printf("%s = %s\t", colName[i], argv[i] ?  : "NULL");
    }
    printf("\n");
    return 0;
}
/*
char *get_date()
{
    time_t current_time;
    char *c_time_string;
    int len;

    // Get the current time
    current_time = time(NULL);
    if (current_time == ((time_t) -1))
    {
        (void) fprintf(stderr, "Failure to obtain the current time.\n");
        exit(EXIT_FAILURE);
    }

    // Convert to local time format
    c_time_string = ctime(&current_time);
    if (c_time_string == NULL)
    {
        (void) fprintf(stderr, "Failure to convert the current time.\n");
        exit(EXIT_FAILURE);
    }

    len = (int) strlen(c_time_string);
    c_time_string[len - 1] = '\0';

    return c_time_string;
}
*/

int count(void){

    int counter;
	sqlite3_stmt *stmt;
	sqlite3 *c;
	char *errmsg;
	int result;
	char *sql = "select count(*) as tot from cache;";
	c=open_connection();
	result = sqlite3_prepare_v2(c, sql, strlen(sql), &stmt, NULL);
	if (result != SQLITE_OK) {

		printf("Failed to prepare database \n");
		close_connection(c);
		exit(EXIT_FAILURE);
	}
	printf("SQL prepared ok\n");
	do {
		result = sqlite3_step(stmt) ;
		if (result == SQLITE_ROW) { /* can read data */
			counter=sqlite3_column_int(stmt,0);
		}
	} while (result == SQLITE_ROW);

	/* finish off */
	close_connection(c);

    return counter;

}


/*il più antico di tutti*/
void older(char *id){

		sqlite3_stmt *stmt;
		sqlite3 *c;
		char *errmsg;
		char *query="SELECT ID, MIN(Last_Modified) FROM cache";
		int result;
		c=open_connection();
	result = sqlite3_prepare_v2(c, query, strlen(query), &stmt, NULL);
	if (result != SQLITE_OK) {

		printf("Failed to prepare database \n");
		close_connection(c);
		exit(EXIT_FAILURE);
	}
	printf("SQL prepared ok\n");
	do {
		result = sqlite3_step(stmt) ;
		if (result == SQLITE_ROW) { /* can read data */
			strcpy(id, (char *)sqlite3_column_text(stmt,0));
		}
	} while (result == SQLITE_ROW);
	/* finish off */
	close_connection(c);



		
}

void older_id(int id,char *last){

    sqlite3 *c;
    sqlite3_stmt *stmt;
    char *errmsg;
    char *query=NULL;
    int result;
    c=open_connection();
    asprintf(&query,"SELECT MIN(Last_Modified)  FROM cache WHERE ID='%d';",id);
	result = sqlite3_prepare_v2(c, query, strlen(query), &stmt, NULL);
	if (result != SQLITE_OK) {

		printf("Failed to prepare database \n");
		close_connection(c);
		exit(EXIT_FAILURE);
	}
	printf("SQL prepared ok\n");
	do {
		result = sqlite3_step(stmt) ;
		if (result == SQLITE_ROW) { /* can read data */
			strcpy(last, (char *)sqlite3_column_text(stmt,0));
		}
	} while (result == SQLITE_ROW);
	/* finish off */
	close_connection(c);


		
}


void create_table(sqlite3 *conn){
	
	 
   char *zErrMsg = 0;
   int rc;
   char *sql;

      /* Create SQL statement */
   sql = "CREATE TABLE cache("  \
         "ID INT PRIMARY KEY     NOT NULL," \
         "Nome          TEXT    NOT NULL," \
         "Path           CHAR(100)     NOT NULL," \
         "Qualità        TEXT," \
         "Last_Modified  CHAR(100)," \
		 "User_Agent     CHAR(100));";

   /* Execute SQL statement */
   rc = sqlite3_exec(conn, sql, callback, 0, &zErrMsg);
   
   if( rc != SQLITE_OK ){
   fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   } else {
      fprintf(stdout, "Table created successfully\n");
   }
   close_connection(conn);

}

void insert(int id,char *path,char *nome,char *q,char *mod,char *ua){

		sqlite3 *c;
		sqlite3_stmt *stm;
		char *query=NULL;
		int rc;
		c=open_connection();
		asprintf(&query,"INSERT INTO cache VALUES('%d','%s','%s','%s','%s','%s');",id,nome,path,q,mod,ua);

		printf("%s\n",query);

		sqlite3_prepare_v2(c,query,strlen(query),&stm,NULL);

		rc=sqlite3_step(stm);
		if(rc!=SQLITE_DONE){
				fprintf(stderr,"Errore inserimento dati %s\n",sqlite3_errmsg(c));
				exit(EXIT_FAILURE);}
		
		/*free stm*/
		sqlite3_finalize(stm);
		free(query);
		/*chiudi la connessione*/
		close_connection(c);




		
}

void update_lastModified(char *img,char *user_agent,char *quality)
{
	sqlite3 *c;
	sqlite3_stmt *stmt;

    //char *date = get_date();
    char *date = "2018-8-8";

	char *User_a;
	int result;
	char *Qual;
	char *query=(char*)malloc(100);
	char *sql=(char*)malloc(100);

	c=open_connection();


    sprintf(sql,"SELECT User_Agent,Qualità from cache where Nome = '%s'",img);
							        
		result = sqlite3_prepare_v2(c, sql, strlen(sql), &stmt, NULL);
								        
		if (result != SQLITE_OK) {
			printf("Failed to prepare database \n");
			close_connection(c);
			exit(EXIT_FAILURE);
																        
		}
									        
		printf("SQL prepared ok\n");
										        
		User_a = (char *)malloc(100);
		Qual = (char *)malloc(100);		
		do {
													                
			result = sqlite3_step(stmt) ;
															                
			if (result == SQLITE_ROW) { /* can read data */
																		                        
				strcpy(User_a, (char *)sqlite3_column_text(stmt,0));
				strcpy(Qual,(char *)sqlite3_column_text(stmt,1));						
																								                
			}
																	        
		} while (result == SQLITE_ROW);
		if(User_a ==user_agent && Qual ==quality){
			sprintf(query,"UPDATE cache SET Last_modified = '%s', WHERE Nome = '%s' ",date,img);
			result = sqlite3_prepare_v2(c, query, strlen(query), &stmt, NULL);
			if (result != SQLITE_OK){
				printf("Failed to prepare database \n");
				close_connection(c);
				exit(EXIT_FAILURE);
			}


												        
		/* finish off */
												        
		close_connection(c);
												        
		free(Qual);
		free(User_a);
	
	

		}	

}
void delete(int id){

		sqlite3 *c;
		sqlite3_stmt *stm;
		char *query=NULL;
		int rc;
		c=open_connection();
		asprintf(&query,"DELETE FROM cache WHERE ID='%d';",id);

		printf("%s\n",query);

		sqlite3_prepare_v2(c,query,strlen(query),&stm,NULL);

		rc=sqlite3_step(stm);
		if(rc!=SQLITE_DONE){
				fprintf(stderr,"Errore inserimento dati %s\n",sqlite3_errmsg(c));
				exit(EXIT_FAILURE);}
		
		/*free stm*/
		sqlite3_finalize(stm);
		free(query);
		close_connection(c);




		
}

void select_last_modified(char *img, char *last){

	sqlite3 *c;
	sqlite3_stmt *stmt;
	char *PATH;
	char *sql;
	int result;
	c=open_connection();
	

	sprintf(sql,"SELECT Last_Modified from cache where Nome = '%s'",img);
	result = sqlite3_prepare_v2(c, sql, strlen(sql), &stmt, NULL);	
	if (result != SQLITE_OK) {
	
		printf("Failed to prepare database \n");
		close_connection(c);
		exit(EXIT_FAILURE);
	}
	printf("SQL prepared ok\n");	
	do {
		result = sqlite3_step(stmt) ;
		if (result == SQLITE_ROW) { /* can read data */									                          
			strcpy(last, (char *)sqlite3_column_text(stmt,0));
		}
	} while (result == SQLITE_ROW);
	/* finish off */
	close_connection(c);	


}

void select_path_from_img(char *img, char *user_agent, char *q, char *path){

	sqlite3 *c;
	sqlite3_stmt *stmt;
	char *PATH;
	char *sql;
	int result;
	c = open_connection();
	

	sprintf(sql,"SELECT Path from cache where Nome = '%s' and User_Agent = '%s' and Qualità = '%s'",img, user_agent, q);
	result = sqlite3_prepare_v2(c, sql, strlen(sql), &stmt, NULL);	
	if (result != SQLITE_OK) {
	
		printf("Failed to prepare database \n");
		close_connection(c);
		exit(EXIT_FAILURE);
	}
	printf("SQL prepared ok\n");	
	do {
		result = sqlite3_step(stmt) ;
		if (result == SQLITE_ROW) { /* can read data */									                          
			strcpy(path, (char *)sqlite3_column_text(stmt,0));
		}
	} while (result == SQLITE_ROW);
	/* finish off */
	close_connection(c);	


}
/*
int main(void){
	//sqlite3 * c=Connection();
	//Create_table(c);
    char *id = (char *) malloc(sizeof(char)*10);
    char *lm = malloc(sizeof(char)*25);
    char *path = malloc(sizeof(char)*25);
    int con = count();
	//insert(1,"home/alessio/","img1","0.8","12-02-2002 13:57","Mozilla");
	//insert(2,"/home/","img2","0.7","12-02-2002 12:00","chrome");
	//insert(3,"/home/alessio/Scarcati","img3","0.8","13-12-2002 12:01","Mozilla");
	//insert(1,"home/alessio/","img1","0.5","12-02-2002 11:00","chrome");
	older(id);
	select_last_modified("img3", lm);
	select_path_from_img("img1", "chrome", "0.5", path);
	//update_lastModified("img1", "Mozilla", "0.8");
	//older_id(1);
	//delete(1);
	//delete(2);
	//delete(3);
    printf("%s\n", path);
    //printf("%s\n%s\n%s\n%d\n", id, lm, path, con);

}
 */