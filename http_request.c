//
// Created by ale96 on 04/04/18.
//

#include "basics.h"
#include "http_request.h"
#include "server_io.h"

char* s = "\n";

http_request *alloc_request()
{
    http_request *msg;
    msg = malloc(sizeof(http_request));
    if(msg == NULL)
    {
        fprintf(stderr, "Error in malloc: struct for http_msg not allocated\n");
        exit(EXIT_FAILURE);
    }
    return msg;
}

void free_request(http_request *msg)
{
    free(msg);
}

void parsing(int sockd, http_request *request)
{

    char line[MAXLINE];
    int n; //number of bytes read
    char *p;

    request->User_agent = NULL;
    request->Connection = NULL;
    request->Accept = NULL;
    request->Host = NULL;


    if ((n = (int) read(sockd, line, MAXLINE)) == 0){
        return; // il client ha chiuso la connessione e inviato EOF

    }
    p = strtok(line, s);
    request->GET = p;

    while(request->Host == NULL || request->Accept == NULL || request->Connection == NULL || request->User_agent == NULL)
    {
        p = (char*)strtok(NULL, s);
        if(strstr(p, "User-Agent:") != NULL)
        {
            request->User_agent = p;
        }
        if(strstr(p, "Connection:") != NULL)
        {
            request->Connection = p;
        }
        if(strstr(p, "Host:") != NULL)
        {
            request->Host = p;
        }
        if(strstr(p, "Accept:") != NULL)
        {
            request->Accept = p;
        }
    }
    return;
}
