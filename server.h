#ifndef SERVER_CONTENTADAPTATION_SERVER_H
#define SERVER_CONTENTADAPTATION_SERVER_H

#include "services/adapt_image.h"
#include "services/basics.h"
#include "logging/logging.h"
#include "http/http_management.h"
#include <signal.h>
#include <sys/wait.h>
#include <fcntl.h>

#define PATH_MEMORY_CACHE "cache_memory/"
#define PATH_SOURCE_IMAGES "images/"
#define MAX_RECORD_IN_DB 15

// Definire qui le costanti

pid_t child_make(int i, int listenfd, int addrlen);
void my_lock_init(char *pathname);
void my_lock_wait();
void my_lock_release();

#endif //SERVER_CONTENTADAPTATION_SERVER_H
