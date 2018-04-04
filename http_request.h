//
// Created by ale96 on 04/04/18.
//

#ifndef SERVER_CONTENTADAPTATION_HTTP_REQUEST_H
#define SERVER_CONTENTADAPTATION_HTTP_REQUEST_H


typedef struct http_request{
    char *GET;
    char *Host;
    char *Connection;
    char *Accept;
    char *User_agent;
} http_request;

http_request parsing(int sockd);

#endif //SERVER_CONTENTADAPTATION_HTTP_REQUEST_H