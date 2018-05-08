

//TODO: *- Verifica se parsing di user agent va bene (prende solo il nome del browser)
//TODO: *- parse_get e parse_userAgent NON funziona il restore del parametro (quindi li ho tolti)


#include "http_management.h"

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
char *parse_get(char *msg_get)
{
    char *resource;

    if((strtok(msg_get, "?")) == NULL)
    {
        perror("Error in strotk while parsing GET field: BAD REQUEST\n");
        // rimanda a pagina BAD REQUEST
        exit(EXIT_FAILURE);
    }
    if((resource = strtok(NULL, ".")) == NULL)
    {
        perror( "Error in strtok while parsing GET field\n");
        exit(EXIT_FAILURE);
    }

    return resource;
}

char *parse_protocol(char *msg_get)
{
    char *protocol;
    char restore[DIM_LONG];
    strcpy(restore, msg_get);

    if((protocol = strstr(msg_get,"HTTP")) == NULL)
    {
        perror("Error in strstr while parsing GET for protocol\n");
        exit(EXIT_FAILURE);
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
        perror("Error in strtok while parsing Host\n");
        // rimanda a bad request
        exit(EXIT_FAILURE);
    }
    if((host = strtok(NULL, "\n")) == NULL)
    {
        perror("Error in strtok while parsing Host field\n");
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
        perror("Error in strtok while parsing Accept field\n");
        // rimanda bad request
        exit(EXIT_FAILURE);
    }
    if((strtok(NULL, "q=")) == NULL)
    {
        perror("Error in strtok while searching factor q\n");
        // rimanda bad request
        exit(EXIT_FAILURE);
    }
    if((q = strtok(NULL, "\n")) == NULL)
    {
        perror("Error in strtok while searching factor q\n");
        // rimanda bad request
        exit(EXIT_FAILURE);
    }
    if((q = strtok(q, "=")) == NULL)
    {
        perror("Error in strtok while searching factor q\n");
        // rimanda bad request
        exit(EXIT_FAILURE);
    }

    strcpy(msg_accept, restore);
    return q;
}

char *parse_userAgent(char *msg_userAgent)
{
    char *user_agent;

    if((strtok(msg_userAgent, " ")) == NULL)
    {
        perror("Error in strtok while parsing User-Agent\n");
        exit(EXIT_FAILURE);
    }
    if((user_agent = strtok(NULL, " ")) == NULL)
    {
        perror("Error in strtok while parsing User-Agent\n");
        exit(EXIT_FAILURE);
    }

    return user_agent;
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
        return; // Client closes the connection and sends EOF
    }

    if((p = strtok(line, s)) == NULL) // Takes the first row (so GET field)
    {
        perror("Error in parsing: bad request\n");
        exit(EXIT_FAILURE);
    }
    strcpy(request->GET, p);

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
