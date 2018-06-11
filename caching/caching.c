//TODO: SELECT_PATH_FROM_ID ==> Togli la parte commentata, verificando anche il Q

#define _GNU_SOURCE
#include "caching.h"

/**
 * This function insert a new record in the DB with elements given in input
 * @Param: id, path, name, quality, last_modified date, user agent
 * @Return: void
 */
void insert(int id, char *path, char *name, char *q, long int last_modif, char *user_agent)
{
	sqlite3 *connection;
	sqlite3_stmt *stm;
	
	char *query=NULL;
	int result;
	connection = open_connection();
	asprintf(&query,"INSERT INTO cache VALUES('%d','%s','%s','%s','%ld','%s');",id,name,path,q,last_modif,user_agent);
	
	sqlite3_prepare_v2(connection,query,strlen(query),&stm,NULL);
	result=sqlite3_step(stm);
	
	if(result != SQLITE_DONE)
	{
		fprintf(stderr,"Error inserting data in DB %s\n",sqlite3_errmsg(connection));
		exit(EXIT_FAILURE);
	}

	sqlite3_finalize(stm);
	free(query);
	close_connection(connection);
}

/**
 * Delete an element from DB depending on its ID
 * @Param: id
 * @Return: void
 */
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
		fprintf(stderr,"Error deleting data from DB: %s\n", sqlite3_errmsg(connection));
		exit(EXIT_FAILURE);
	}

	sqlite3_finalize(stm);
	free(query);
	close_connection(connection);
}

/**
 *
 */
long int date_int(char *date)
{
	char *month=(char*)malloc(5);
	char *new_day=(char*)malloc(5);
	size_t len;
	char *fulldate=(char *)malloc(sizeof(char)*50);
    char restore[DIM_SHORT];
    strcpy(restore, date);

    strtok(restore," ");
	month=strtok(NULL," ");

	char *day=(char*)malloc(2);
	day=strtok(NULL," ");
	char *hour=(char*)malloc(2);
	hour=strtok(NULL,":");
	char *minuts=(char *)malloc(2);
	minuts=strtok(NULL,":");
	char *seconds=(char *)malloc(2);
	seconds=strtok(NULL," ");
	char *year=(char*)malloc(5);
	year=strtok(NULL,"\n");
	if(strcmp(month,"Jan")==0){
		month="01";
	}
	else if(strcmp(month,"Feb")==0){
		month="02";
	}
	else if(strcmp(month,"Mar")==0){
		month="03";
	}
	else if(strcmp(month,"Apr")==0){
		month="04";
	}
	else if(strcmp(month,"May")==0){
		month="05";
	}
	else if(strcmp(month,"Jun")==0){
		month="06";
	}
	else if(strcmp(month,"Jul")==0){
		month="07";
	}
	else if(strcmp(month,"Aug")==0){
		month="08";
	}
	else if(strcmp(month,"Sept")==0){
		month="09";
	}
	else if(strcmp(month,"Oct")==0){
		month="10";
	}
	else if(strcmp(month,"Nov")==0){
		month="11";
	}
	else if(strcmp(month,"Dec")==0){
		month="12";
	}
	len=strlen(day);
	if(len!=2){
		sprintf(new_day,"0%s",day);
	}
	else{
		new_day=day;
	}
	sprintf(fulldate,"%s%s%s%s%s%s",year,month,new_day,hour,minuts,seconds);
	long int int_date = atol(fulldate);
	free(fulldate);
	return int_date;
}

/**
 *
 * @param id
 */
int sel_max_id()
{
    printf("\n");
    sqlite3 *conn;
    sqlite3_stmt *stmt;
    char *sql;
    int result, id;
    conn = open_connection();

    sprintf(sql,"select MAX(ID) from cache");
    result = sqlite3_prepare_v2(conn, sql, strlen(sql), &stmt, NULL);
    if (result != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare database in sel_max_id()\n");
        close_connection(conn);
        exit(EXIT_FAILURE);
    }
    do {
        result = sqlite3_step(stmt) ;
        if (result == SQLITE_ROW) {
            id = sqlite3_column_int(stmt,0);
            id++;
        }
    } while (result == SQLITE_ROW);

    close_connection(conn);
    return id;
}

/**
 * This function return the number of elements in the DB
 * @Param: void
 * @Return: number of elements in the DB
 */
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

/**
 * This function calculate the older element with a particular ID
 * @Param: id
 * @Return: void
 */
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

/**
 * This function update last_modified field only if user-agent and quality are equals to elements already in DB
 * @Param:
 * @Return: void
 */
void update_lastModified(char *img, char *user_agent, char *quality)
{
	sqlite3 *connection;
	sqlite3_stmt *stmt;

	connection = open_connection();

    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    char s[64];
    strftime(s, sizeof(s), "%c", tm);
    long int date = date_int(s);

	int result, id;
	char query[DIM_SHORT], sql[DIM_SHORT], path[DIM_PATH], ua[DIM_SHORT], q[4];

    sprintf(sql, "SELECT User_Agent, Qualita, ID, Path from cache where Nome = '%s'", img);
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
			id = sqlite3_column_int(stmt,2);
            strcpy(path,(char *)sqlite3_column_text(stmt,3));
        }
	} while (result == SQLITE_ROW);

	int result_ua = strcmp(user_agent, ua);
	int result_q = strcmp(quality, q);
	if(result_ua == 0 && result_q == 0)
    {
        delete(id);
        insert(id, path,img, q, date,ua);
    } else{
	    int newId = sel_max_id();
        insert(newId, path, img, q, date, ua);
    }

    close_connection(connection);
}

/**
 * This function return the path of an image with a given user-agent and quality
 * @Param: img
 * @Return: void
 */
int select_id_from_img(char *img, char *user_agent, char *q)
{
    printf("\n");
    sqlite3 *connection;
	sqlite3_stmt *stmt;

	int id = 0;
	char *sql;
	int result;
	connection = open_connection();

    //sprintf(sql, "SELECT ID from cache where Nome = '%s' and User_Agent = '%s' and Qualita = '%s'", img, user_agent, q);
	sprintf(sql, "SELECT ID from cache where Nome = '%s' and User_Agent = '%s'", img, user_agent);
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
            id = sqlite3_column_int(stmt, 0);
		}
	} while (result == SQLITE_ROW);

	close_connection(connection);
    return id;
}