//
// Created by ale96 on 10/04/18.
//

#ifndef SERVER_CONTENTADAPTATION_HTTP_MANAGEMENT_H
#define SERVER_CONTENTADAPTATION_HTTP_MANAGEMENT_H

#include "basics.h"
#include "server_io.h"
#include <fcntl.h>

char* path_pages = "/home/ale96/Documents/internetWeb/project/Server_ContentAdaptation/pages";

typedef struct http_request{
    char *GET;
    char *Protocol;
    char *Host;
    char *Connection;
    char *Accept;
    char *User_agent;
} http_request;

typedef struct http_response{
    char *Header;
    char *Date;
    char *Server;
    char *Last_Modified;
    char *Content_Length;
    char *Content_Type;
    char *Connection;
    char *Body_Response;
} http_response;

void parsing(int sockd, http_request *request);
http_request *alloc_request();
void free_request(http_request *msg);


#endif //SERVER_CONTENTADAPTATION_HTTP_MANAGEMENT_H