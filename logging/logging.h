#ifndef SERVER_CONTENTADAPTATION_LOGGING_H
#define SERVER_CONTENTADAPTATION_LOGGING_H

#include "../services/basics.h"
#include "../http/http_management.h"
#include <pthread.h>
#include <syslog.h>

#define LOG_FILE  "/WebServerLOG.txt"

void logging(http_request *req, http_response *res);

#endif //SERVER_CONTENTADAPTATION_LOGGING_H
