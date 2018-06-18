#include <sqlite3.h>
#include "../services/basics.h"
#include "../http/http_management.h"

sqlite3 *open_connection(void);
void close_connection(sqlite3 * connection);
void create_table(sqlite3 *connection);
void create_table(sqlite3 *connection);

void older(char *id);
void insert(int id, char *path, char *name, char *quality, long int last_modified, char *user_agent);
void delete(int id);
void delete_remove(int id);

int count(void);
int sel_max_id(void);
void sel_name(int id, char *name);
int select_id_from_img(char *img, char *user_agent, char *quality);
void update_lastModified(char *img, char *user_agent, char *quality);
long int date_int(char *current_date);