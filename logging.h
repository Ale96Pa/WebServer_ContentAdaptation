//
// Created by ale96 on 28/04/18.
//

#ifndef SERVER_CONTENTADAPTATION_LOGGING_H
#define SERVER_CONTENTADAPTATION_LOGGING_H

#include "basics.h"
#include "server_io.h"
#include "http_management.h"
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <syslog.h>

#define LOG_FILE  "/logServer.txt"

void logging(http_request *req, http_response *res);


#endif //SERVER_CONTENTADAPTATION_LOGGING_H
