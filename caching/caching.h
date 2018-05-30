#include <sqlite3.h>
#include "../services/basics.h"
#include "../http/http_management.h"

sqlite3 *open_connection(void);
void close_connection(sqlite3 * connection);
void create_table(sqlite3 *connection);
int callback(void *arg, int argc, char **argv, char **colName);
void create_table(sqlite3 *connection);

void older_id(int id,char *last_modified); //TODO: non dovrebbe servire
void older(char *id);
void insert(int id, char *path, char *name, char *quality, char *last_modified, char *user_agent);
void delete(int id);
int count(void);
void select_path_from_img(char *img, char *user_agent, char *quality, char *path);
void update_lastModified(char *img, char *user_agent, char *quality);
void select_last_modified(char *img, char *last_modified);