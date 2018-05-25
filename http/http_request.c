#include "http_management.h"

//TODO: controllo su ciclo while finale (se non setta tutti i parametri ==> BAD REQUEST)
//todo: aggiustare read dalla socket (usare read_line del prof?? )

// This char is used as SPLITTER for the whole http message
char* s = "\n";

/**
  * This function allocate memory for the whole request
  * @Param: none
  * @Return: pointer to allocated struct for the request
  */
http_request *alloc_request()
{
    http_request *msg;

    if((msg = malloc(sizeof(http_request))) == NULL)
    {
        perror("Error in malloc: struct for http_msg not allocated\n");
        exit(EXIT_FAILURE);
    }
    return msg;
}

/**
  * These functions are used to extract information by relative fields
  * In these functions the buffer "restore" is used to restor the effective value of parameter
  * (that should be a field of the management struct).
  * @Param: single message
  * @Return: string including information
  */
char *parse_method(char *msg_method)
{
    char *method;
    char *method_restore = malloc(sizeof(char)*DIM_SHORT);
    char *restore = malloc(sizeof(char)*DIM_LONG);
    strcpy(restore, msg_method);

    if((method = strtok(msg_method," ")) == NULL)
    {
        fprintf(stderr, "Error in strtok while parsing method\n");
        return NULL;
    }

    strcpy(method_restore, method);
    strcpy(msg_method, restore);
    free(restore);
    return method_restore;
}
char *parse_get(char *msg_get)
{
    char *resource;
    char *resource_restore = malloc(sizeof(char)*DIM_SHORT);
    char *restore = malloc(sizeof(char)*DIM_LONG);
    strcpy(restore, msg_get);

    if((strtok(msg_get, "?")) == NULL)
    {
        fprintf(stderr, "Error in strotk while parsing GET field: BAD REQUEST\n");
        return NULL;
    }
    if((resource = strtok(NULL, ".")) == NULL)
    {
        fprintf(stderr, "Error in strtok while parsing GET field\n");
        exit(EXIT_FAILURE);
    }

    strcpy(resource_restore, resource);
    strcpy(msg_get, restore);
    free(restore);
    return resource_restore;
}
char *parse_protocol(char *msg_get)
{
    char *protocol;
    char restore[DIM_LONG];
    strcpy(restore, msg_get);

    if((protocol = strstr(msg_get,"HTTP")) == NULL)
    {
        fprintf(stderr, "Error in strstr while parsing GET for protocol\n");
        return NULL;
    }

    strcpy(msg_get, restore);
    return protocol;
}
char *parse_host(char *msg_host)
{
    char *host;
    char restore[DIM_SHORT];
    strcpy(restore, msg_host);

    if((strtok(msg_host, " ")) == NULL)
    {
        fprintf(stderr, "Error in strtok while parsing Host\n");
        return NULL;
    }
    if((host = strtok(NULL, "\n")) == NULL)
    {
        fprintf(stderr, "Error in strtok while parsing Host field\n");
        exit(EXIT_FAILURE);
    }

    strcpy(msg_host, restore);
    return host;
}
char *parse_accept(char *msg_accept)
{
    char *q;
    char restore[DIM_LONG];
    strcpy(restore, msg_accept);

    if((strtok(msg_accept, "q=")) == NULL)
    {
        fprintf(stderr, "Error in strtok while parsing Accept field\n");
        return NULL;
    }
    if((strtok(NULL, "q=")) == NULL)
    {
        fprintf(stderr, "Error in strtok while searching factor q\n");
        return NULL;
    }
    if((q = strtok(NULL, "\n")) == NULL)
    {
        fprintf(stderr, "Error in strtok while searching factor q\n");
        return NULL;
    }
    if((q = strtok(q, "=")) == NULL)
    {
        fprintf(stderr, "Error in strtok while searching factor q\n");
        return NULL;
    }

    strcpy(msg_accept, restore);
    return q;
}
char *parse_userAgent(char *msg_userAgent)
{
    char *user_agent;
    char *ua_restore = malloc(sizeof(char)*DIM_SHORT);
    char *restore = malloc(sizeof(char)*DIM_LONG);
    strcpy(restore, msg_userAgent);

    if((strtok(msg_userAgent, " ")) == NULL)
    {
        fprintf(stderr, "Error in strtok while parsing User-Agent\n");
        return NULL;
    }
    if((user_agent = strtok(NULL, " ")) == NULL)
    {
        fprintf(stderr, "Error in strtok while parsing User-Agent\n");
        return NULL;
    }

    strcpy(ua_restore, user_agent);
    strcpy(msg_userAgent, restore);
    free(restore);
    return ua_restore;
}

/**
  * This function is used to parse the whole http_request: it is organized
  * in a struct with the http fields
  * @Param: socket with message, pointer to struct containing request
  * @Return: void
  */
void parsing_request(int sockd, http_request *request)
{
    char line[MAXLINE];
    int n; //number of bytes read
    char *p;
    int i=0;

    if ((n = read(sockd, line, MAXLINE) == 0)){
        // Client closes the open_connection and sends EOF
        return;
    }

    if((p = strtok(line, s)) == NULL) // Takes the first row (so GET field)
    {
        fprintf(stderr, "Error in parsing: bad request\n");
        exit(EXIT_FAILURE);
    }
    strcpy(request->Request, p);

    while(i != 4)
    {
        p = strtok(NULL, s);
        if(strstr(p, "User-Agent:") != NULL)
        {
            strcpy(request->User_agent, p);
            i++;
        }
        if(strstr(p, "Connection:") != NULL)
        {
            strcpy(request->Connection, p);
            i++;
        }
        if(strstr(p, "Host:") != NULL)
        {
            strcpy(request->Host, p);
            i++;
        }
        if(strstr(p, "Accept:") != NULL)
        {
            strcpy(request->Accept, p);
            i++;
        }
    }
}