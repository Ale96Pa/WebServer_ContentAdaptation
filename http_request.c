//
// Created by ale96 on 04/04/18.
//

#include "basics.h"
#include "http_request.h"
#include "server_io.h"

http_request parsing(int sockd)
{
    http_request *request;
    char line[MAXLINE];
    int n; //number of bytes read
    char *v;

    for(;;)
    {
        if ((n = readline(sockd, line, MAXLINE)) == 0){
            return *request; /* il client ha chiuso la connessione e inviato EOF */
        }
    }
}
