//TODO: *- Host salta lettere -- CONTROLLA DA' ERRORI !!!
//TODO: *- Manca user-agent da parsare (serve?!?)
//TODO: *- Ripulisci codice, commenta, raggruppa funzioni, modularizza
//TODO: *- La funzione con la free e' necessaria??

#include "http_management.h"

//This char is used as SPLITTER for the whole http message
char* s = "\n";

/*
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

/*
 * This function de-allocate memory for the whole request
 * @Param: pointer to struct to de-allocate
 * @Return: void
 */
void free_request(http_request *msg)
{
    free(msg);
}

/*
 * This functions are used to extract information by relative fields
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
    if((protocol = strstr(msg_get, " HTTP")) == NULL)
    {
        perror("Error in strstr while parsing GET for protocol\n");
        exit(EXIT_FAILURE);
    }
    return protocol;
}
char *parse_host(char *msg_host)
{
    char *host;
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
    return host;
}
char *parse_accept(char *msg_accept)
{
    char *q;
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
    return q;
}

/*
 * This function is used to parse the whole http_request: it is organized
 * in a struct with the http fields
 * @Param: socket with message, pointer to struct containing request
 * @Return: void
 */
void parsing(int sockd, http_request *request)
{

    char line[MAXLINE];
    int n; //number of bytes read
    char *p;
    int i=0;

    // Allocation of every field of the struct
    if((request->GET = malloc(sizeof(char)*256)) == NULL)
    {
        perror("Error in malloc GET field\n");
        return;
    }
    if((request->Protocol = malloc(sizeof(char)*256)) == NULL)
    {
        perror("Error in malloc Protocol field\n");
        return;
    }
    if((request->User_agent = malloc(sizeof(char)*256)) == NULL)
    {
        perror("Error in malloc User-agent field\n");
        return;
    }
    if((request->Accept = malloc(sizeof(char)*256)) == NULL)
    {
        perror("Error in malloc Accept field\n");
        return;
    }
    if((request->Host = malloc(sizeof(char)*256)) == NULL)
    {
        perror("Error in malloc Host field\n");
        return;
    }
    if((request->Connection = malloc(sizeof(char)*256)) == NULL)
    {
        perror("Error in malloc Connection field\n");
        return;
    }


    if ((n = read(sockd, line, MAXLINE) == 0)){
        return; // Client closes the connection and sends EOF
    }

    if((p = strtok(line, s)) == NULL) // Takes the first row (so GET field)
    {
        perror("Error in parsing: bad request\n");
        exit(EXIT_FAILURE);
    }
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
