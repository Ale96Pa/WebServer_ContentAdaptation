//TODO: aggiornamento di last modified NON funziona
//TODO: aggiungere una restituzione dell'ID piu' grande possibile
//TODO: aggiungere commenti

#define _GNU_SOURCE
#include "caching.h"

void insert(int id, char *path, char *name, char *q, char *last_modif, char *user_agent)
{
	sqlite3 *connection;
	sqlite3_stmt *stm;
	
	char *query=NULL;
	int result;
	connection = open_connection();
	asprintf(&query,"INSERT INTO cache VALUES('%d','%s','%s','%s','%s','%s');",id,name,path,q,last_modif,user_agent);
	
	sqlite3_prepare_v2(connection,query,strlen(query),&stm,NULL);
	result=sqlite3_step(stm);
	
	if(result != SQLITE_DONE)
	{
		fprintf(stderr,"Error inserting data in DB %s\n",sqlite3_errmsg(connection));
		exit(EXIT_FAILURE);
	}

	/*free stm*/
	sqlite3_finalize(stm);
	free(query);
	close_connection(connection);
}

void delete(int id)
{
	sqlite3 *connection;
	sqlite3_stmt *stm;

	char *query=NULL;
	int result;
	connection = open_connection();
	asprintf(&query,"DELETE FROM cache WHERE ID='%d';", id);

	sqlite3_prepare_v2(connection,query,strlen(query),&stm,NULL);

	result = sqlite3_step(stm);
	if(result!=SQLITE_DONE)
	{
		fprintf(stderr,"Error deleting data from DB %s\n", sqlite3_errmsg(connection));
		exit(EXIT_FAILURE);
	}

	/*free stm*/
	sqlite3_finalize(stm);
	free(query);
	close_connection(connection);
}

int count(void)
{
	sqlite3_stmt *stmt;
	sqlite3 *connection;

    int counter, result;
	char *errmsg;
	char *sql = "select count(*) as tot from cache;";

	connection = open_connection();
	result = sqlite3_prepare_v2(connection, sql, strlen(sql), &stmt, NULL);
	if (result != SQLITE_OK)
	{
		fprintf(stderr, "Failed to prepare database in function 'count()'\n");
		close_connection(connection);
		exit(EXIT_FAILURE);
	}

	do {
		result = sqlite3_step(stmt) ;
		if (result == SQLITE_ROW)
		{
			counter = sqlite3_column_int(stmt,0);
		}
	} while (result == SQLITE_ROW);

	close_connection(connection);
    return counter;
}


void older(char *id)
{
	sqlite3_stmt *stmt;
	sqlite3 *connection;

	char *errmsg;
	char *query="SELECT ID, MIN(Last_Modified) FROM cache";
	int result;

	connection=open_connection();
	result = sqlite3_prepare_v2(connection, query, strlen(query), &stmt, NULL);

	if (result != SQLITE_OK)
	{
		fprintf(stderr, "Failed to prepare database in function 'older'\n");
		close_connection(connection);
		exit(EXIT_FAILURE);
	}

	do {
		result = sqlite3_step(stmt) ;
		if (result == SQLITE_ROW)
		{
			strcpy(id, (char *)sqlite3_column_text(stmt,0));
		}
	} while (result == SQLITE_ROW);

	close_connection(connection);
}

void older_id(int id,char *last_modif)
{
    sqlite3 *connection;
    sqlite3_stmt *stmt;

    char *errmsg;
    char *query=NULL;
    int result;

    connection = open_connection();
    asprintf(&query,"SELECT MIN(Last_Modified)  FROM cache WHERE ID='%d';",id);
	result = sqlite3_prepare_v2(connection, query, strlen(query), &stmt, NULL);

	if (result != SQLITE_OK)
	{
		fprintf(stderr, "Failed to prepare database in function 'older_id()'\n");
		close_connection(connection);
		exit(EXIT_FAILURE);
	}
	do {
		result = sqlite3_step(stmt) ;
		if (result == SQLITE_ROW)
		{
			strcpy(last_modif, (char *)sqlite3_column_text(stmt,0));
		}
	} while (result == SQLITE_ROW);

	close_connection(connection );
}

//TODO: malloc necessarie?? ==> AGGIUSTA MISURA MALLOC
void update_lastModified(char *img, char *user_agent, char *quality)
{
	sqlite3 *connection;
	sqlite3_stmt *stmt;

    //char *date = get_date();
    char *date = "2018-8-8";

	int result;
	char *query = (char *)malloc(100);
	char *sql = (char *)malloc(100);
	char *ua = (char *)malloc(100);
	char *q = (char *)malloc(100);

	connection=open_connection();
    sprintf(sql,"SELECT User_Agent,Qualità from cache where Nome = '%s'", img);
							        
	result = sqlite3_prepare_v2(connection, sql, strlen(sql), &stmt, NULL);

	if (result != SQLITE_OK)
	{
		fprintf(stderr, "Failed to prepare database in function 'update_lastModified()'\n");
		close_connection(connection);
		exit(EXIT_FAILURE);
	}

	do {
		result = sqlite3_step(stmt) ;
		if (result == SQLITE_ROW)
		{
			strcpy(ua, (char *)sqlite3_column_text(stmt,0));
			strcpy(q,(char *)sqlite3_column_text(stmt,1));
		}

	} while (result == SQLITE_ROW);

	if(ua ==user_agent && q ==quality)
	{
		sprintf(query,"UPDATE cache SET Last_modified = '%s', WHERE Nome = '%s' ",date,img);
		result = sqlite3_prepare_v2(connection, query, strlen(query), &stmt, NULL);
		if (result != SQLITE_OK)
		{
			fprintf(stderr, "Failed to prepare database in function 'update_lastModifed()'\n");
			close_connection(connection);
			exit(EXIT_FAILURE);
		}

	close_connection(connection);
	}

	free(q);
	free(ua);
	free(query);
	free(sql);
}

void select_last_modified(char *img, char *last_modif)
{
	sqlite3 *connection;
	sqlite3_stmt *stmt;

	char *sql;
	int result;
	connection = open_connection();

	sprintf(sql,"SELECT Last_Modified from cache where Nome = '%s'",img);
	result = sqlite3_prepare_v2(connection, sql, strlen(sql), &stmt, NULL);

	if (result != SQLITE_OK)
	{
		fprintf(stderr, "Failed to prepare database in function 'select_last_modifed()'\n");
		close_connection(connection);
		exit(EXIT_FAILURE);
	}

	do {
		result = sqlite3_step(stmt) ;
		if (result == SQLITE_ROW)
		{
			strcpy(last_modif, (char *)sqlite3_column_text(stmt,0));
		}
	} while (result == SQLITE_ROW);

	close_connection(connection);
}

void select_path_from_img(char *img, char *user_agent, char *q, char *path)
{
	sqlite3 *connection;
	sqlite3_stmt *stmt;

	char *sql;
	int result;
	connection = open_connection();
	

	sprintf(sql,"SELECT Path from cache where Nome = '%s' and User_Agent = '%s' and Qualità = '%s'", img, user_agent, q);
	result = sqlite3_prepare_v2(connection, sql, strlen(sql), &stmt, NULL);

	if (result != SQLITE_OK)
	{
		fprintf(stderr, "Failed to prepare database in function 'select_path_from_img()'\n");
		close_connection(connection);
		exit(EXIT_FAILURE);
	}

	do {
		result = sqlite3_step(stmt) ;
		if (result == SQLITE_ROW)
		{
			strcpy(path, (char *)sqlite3_column_text(stmt,0));
		}
	} while (result == SQLITE_ROW);

	close_connection(connection);
}

/*
int main(void)
{
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
    //printf("%s\n", path);
    printf("%s\n%s\n%s\n%d\n", id, lm, path, con);
    free(id);
    free(lm);
    free(path);
}
*/