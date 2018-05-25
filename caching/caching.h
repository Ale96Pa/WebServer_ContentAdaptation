#include <stdio.h>
#include <sqlite3.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

sqlite3 *open_connection(void);
void close_connection(sqlite3 * conn);
void create_table(sqlite3 *conn);

void older_id(int id,char *last); //TODO: non dovrebbe servire
void older(char *id);
void insert(int id,char *path,char *nome,char *q,char *mod,char *ua);
void delete(int id);
int count(void);
void select_path_from_img(char *img, char *user_agent, char *quality, char *path);
void update_lastModified(char *img,char *user_agent,char *quality);
void select_last_modified(char *img,char *last);