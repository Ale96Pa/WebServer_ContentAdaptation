//
// Created by ale96 on 28/04/18.
//

#ifndef SERVER_CONTENTADAPTATION_LOGGING_H
#define SERVER_CONTENTADAPTATION_LOGGING_H

#include "../services/basics.h"
#include "../services/server_io.h"
#include "../http/http_management.h"
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <syslog.h>

#define LOG_FILE  "/logServer.txt"

void logging(http_request *req, http_response *res);


#endif //SERVER_CONTENTADAPTATION_LOGGING_H
