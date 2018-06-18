#ifndef SERVER_CONTENTADAPTATION_SERVER_H
#define SERVER_CONTENTADAPTATION_SERVER_H

#include "services/basics.h"
#include "http/http_management.h"
#include "services/adapt_image.h"
#include "logging/logging.h"
#include "storage/client/client.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

#define HTTP0 "HTTP/1.0"
#define HTTP1 "HTTP/1.1"
#define PATH_MEMORY_CACHE "storage/cache_memory/"
#define MAX_RECORD_IN_DB 7
#define FORMAT_IMG "jpeg"

#define NUMBER_CHILD 7
#define SERV_PORT	5193
#define BACKLOG		10

pid_t child_make(int i, int listenfd, int addrlen);
void my_lock_init(char *pathname);
void my_lock_wait();
void my_lock_release();

#endif //SERVER_CONTENTADAPTATION_SERVER_H
