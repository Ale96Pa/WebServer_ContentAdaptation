//
// Created by ale96 on 10/04/18.
//

#ifndef SERVER_CONTENTADAPTATION_HTTP_MANAGEMENT_H
#define SERVER_CONTENTADAPTATION_HTTP_MANAGEMENT_H

#include "basics.h"
#include "server_io.h"
#include <fcntl.h>
#include <time.h>

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
    //char *Last_Modified;
    char *Content_Length;
    char *Content_Type;
    char *Connection;
    char *Body_Response;
} http_response;

void parsing(int sockd, http_request *request);
http_request *alloc_request();
void free_request(http_request *msg);
char *parse_get(char *whole_msg);
char *parse_protocol(char *whole_msg);
char *parse_host(char *whole_msg);
char *parse_accept(char *whole_msg);


http_response *alloc_response();
void page_not_found(char *protocol, http_response *response);
void page_bad_request(char *protocol, http_response *response);
void page_default(char *protocol, http_response *response, char *path);
void parsing_response(int sockd, http_response *response);




#endif //SERVER_CONTENTADAPTATION_HTTP_MANAGEMENT_H
