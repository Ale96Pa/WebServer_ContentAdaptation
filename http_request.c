//
// Created by ale96 on 04/04/18.
//

//TODO: *- Host salta lettere
//TODO: *- Controlla tutti errori
//TODO: *- Manca user-agent da parsare (serve?!?)
//TODO: *- Ripulisci codice, commenta, raggruppa funzioni, modularizza

#include "http_management.h"


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

char *parse_get(char *whole_msg)
{
    char *value;
    //TODO: CONTROLLA ERRORI
    strtok(whole_msg, "?");
    value = strtok(NULL, ".");
    return value;
}

char *parse_protocol(char *whole_msg)
{
    char *value;
    value = strstr(whole_msg, " HTTP");
    return value;
}

char *parse_host(char *whole_msg)
{
    char *value;
    //TODO: CONTROLLA ERRORI
    strtok(whole_msg, " ");
    value = strtok(NULL, "\n");
    return value;
}

char *parse_accept(char *whole_msg)
{
    char *value;
    strtok(whole_msg, "q=");
    strtok(NULL, "q=");
    value = strtok(NULL, "\n");
    return value;
}

void parsing(int sockd, http_request *request)
{

    char line[MAXLINE];
    int n; //number of bytes read
    char *p;
    int i=0;

    if((request->GET = malloc(sizeof(char)*256)) == NULL)
    {
        fprintf(stderr, "Error in malloc GET field\n");
        return;
    }
    if((request->Protocol = malloc(sizeof(char)*256)) == NULL)
    {
        fprintf(stderr, "Error in malloc Protocol field\n");
        return;
    }
    if((request->User_agent = malloc(sizeof(char)*256)) == NULL)
    {
        fprintf(stderr, "Error in malloc User-agent field\n");
        return;
    }
    if((request->Accept = malloc(sizeof(char)*256)) == NULL)
    {
        fprintf(stderr, "Error in malloc Accept field\n");
        return;
    }
    if((request->Host = malloc(sizeof(char)*256)) == NULL)
    {
        fprintf(stderr, "Error in malloc Host field\n");
        return;
    }
    if((request->Connection = malloc(sizeof(char)*256)) == NULL)
    {
        fprintf(stderr, "Error in malloc Connection field\n");
        return;
    }


    if ((int) read(sockd, line, MAXLINE) == 0){
        return; // il client ha chiuso la connessione e inviato EOF
    }

    p = (char *)strtok(line, s); // Prende la prima riga della richiesta (cioe' GET)
    request->GET = p;

    while(i != 4)
    {
        p = (char *)strtok(NULL, s);
        if(strstr(p, "User-Agent:") != NULL)
        {
            request->User_agent = p;
            i++;
        }
        if(strstr(p, "Connection:") != NULL)
        {
            request->Connection = p;
            i++;
        }
        if(strstr(p, "Host:") != NULL)
        {
            request->Host = p;
            i++;
        }
        if(strstr(p, "Accept:") != NULL)
        {
            request->Accept = p;
            i++;
        }
    }
    return;
}
