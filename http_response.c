//
// Created by ale96 on 10/04/18.
//

#include "http_management.h"

//TODO: RIVEREDE MISURE DELLE MALLOC
//TODO: CONTROLLA ERRORI
//TODO: non si vede immagine nella pagina di default (html?)

char *header_html = "<!DOCTYPE html>\n"
                    "<html>\n"
                    "\n"
                    "<head>\n"
                    "\t<title> Adapt Image </title>\n"
                    "</head>\n"
                    "\n"
                    "<body bgcolor=\"#f0f8ff\">\n"
                    "\n"
                    "\t<h2> Dynamic image adaptation </h2>\n"
                    "\t<br><br>";

/*
 * This function allocate memory for the whole response
 * @Param: none
 * @Return: pointer to allocated struct for the request
 */
http_response *alloc_response()
{
    http_response *msg;
    if((msg = malloc(sizeof(http_response))) == NULL)
    {
        perror("Error in malloc: struct for http_msg not allocated\n");
        exit(EXIT_FAILURE);
    }
    return msg;
}


void page_not_found(char *protocol, http_response *response)
{
    // Header field
    response->Header = malloc(sizeof(char)*100);
    char *protocol_allocate=malloc(sizeof(char)*100);
    strcpy(protocol_allocate, protocol);
    char *header = strcat(protocol_allocate, " 404 Not Found");
    free(protocol_allocate);
    response->Header = header;

    // Server field
    response->Server = malloc(sizeof(char)*100);
    char server[] = "Server: nomeServer test";
    response->Server = server;

    // Content-Type field
    response->Content_Type = malloc(sizeof(char)*100);
    char type[] = "Content-Type: text/html";
    response->Content_Type = type;

    // Connection field
    response->Connection = malloc(sizeof(char)*100);
    char conn[] = "Connection: close";
    response->Connection = conn;

    // Body message
    char *body = malloc(sizeof(char)*1024);
    response->Body_Response = malloc(sizeof(char)*1024);
    strcpy(body, header_html);
    strcat(body, "404 PAGE NOT FOUND </body> </html>");
    response->Body_Response = body;

    // Content-Length field/
    size_t len = strlen(body);
    response->Content_Length = malloc(sizeof(char)*100);
    char *lenTxt = malloc(sizeof(char)*100);
    char lenHeader[] = "Content-Length: ";
    char lenStr[8];
    snprintf(lenStr, sizeof lenStr, "%zu", len);
    strcat(lenTxt, lenHeader);
    strcat(lenTxt, lenStr);
    response->Content_Length = lenTxt;

    // Data field
    char buf[1000];
    time_t now = time(0);
    struct tm tm = *gmtime(&now);
    strftime(buf, sizeof buf, "%a, %d %b %Y %H:%M:%S %Z", &tm);
    response->Date = malloc(sizeof(char)*100);
    char dataHeader[] = "Data: ";
    char *date = strcat(dataHeader, buf);
    response->Date = date;

    return;
}

void page_bad_request(char *protocol, http_response *response)
{
    // Header field
    response->Header = malloc(sizeof(char)*100);
    char *protocol_allocate=malloc(sizeof(char)*100);
    strcpy(protocol_allocate, protocol);
    char *header = strcat(protocol_allocate, " 300 Bad Request");
    response->Header = header;

    // Server field
    response->Server = malloc(sizeof(char)*100);
    char server[] = "Server: nomeServer test";
    response->Server = server;

    // Content-Type field
    response->Content_Type = malloc(sizeof(char)*100);
    char type[] = "Content-Type: text/html";
    response->Content_Type = type;

    // Connection field
    response->Connection = malloc(sizeof(char)*100);
    char conn[] = "Connection: close";
    response->Connection = conn;

    // Body message
    char *body = malloc(sizeof(char)*1024);
    response->Body_Response = malloc(sizeof(char)*1024);
    strcpy(body, header_html);
    strcat(body, "BAD REQUEST </body> </html>");
    response->Body_Response = body;

    // Content-Length field/
    size_t len = strlen(body);
    response->Content_Length = malloc(sizeof(char)*100);
    char *lenTxt = malloc(sizeof(char)*100);
    char lenHeader[] = "Content-Length: ";
    char lenStr[8];
    snprintf(lenStr, sizeof lenStr, "%zu", len);
    strcat(lenTxt, lenHeader);
    strcat(lenTxt, lenStr);
    response->Content_Length = lenTxt;

    // Data field
    char buf[1000];
    time_t now = time(0);
    struct tm tm = *gmtime(&now);
    strftime(buf, sizeof buf, "%a, %d %b %Y %H:%M:%S %Z", &tm);
    response->Date = malloc(sizeof(char)*100);
    char dataHeader[] = "Data: ";
    char *date = strcat(dataHeader, buf);
    response->Date = date;

    return;
}

void page_default(char *protocol, http_response *response, char *path)
{
    // Header field
    response->Header = malloc(sizeof(char)*100);
    char *protocol_allocate=malloc(sizeof(char)*100);
    strcpy(protocol_allocate, protocol);
    char *header = strcat(protocol_allocate, " 200 OK");
    response->Header = header;

    // Server field
    response->Server = malloc(sizeof(char)*100);
    char server[] = "Server: nomeServer test";
    response->Server = server;

    // Content-Type field
    response->Content_Type = malloc(sizeof(char)*100);
    char type[] = "Content-Type: text/html";
    response->Content_Type = type;

    // Connection field
    response->Connection = malloc(sizeof(char)*100);
    char conn[] = "Connection: close";
    response->Connection = conn;

    // Body message
    char *body = malloc(sizeof(char)*1024);
    response->Body_Response = malloc(sizeof(char)*1024);
    strcpy(body, header_html);
    strcat(body, "<img src='");
    strcat(body, path);
    strcat(body, "'> </body> </html>");
    response->Body_Response = body;

    // Content-Length field/
    size_t len = strlen(body);
    response->Content_Length = malloc(sizeof(char)*100);
    char *lenTxt = malloc(sizeof(char)*100);
    char lenHeader[] = "Content-Length: ";
    char lenStr[8];
    snprintf(lenStr, sizeof lenStr, "%zu", len);
    strcat(lenTxt, lenHeader);
    strcat(lenTxt, lenStr);
    response->Content_Length = lenTxt;

    // Data field
    char buf[1000];
    time_t now = time(0);
    struct tm tm = *gmtime(&now);
    strftime(buf, sizeof buf, "%a, %d %b %Y %H:%M:%S %Z", &tm);
    response->Date = malloc(sizeof(char)*100);
    char dataHeader[] = "Data: ";
    char *date = strcat(dataHeader, buf);
    response->Date = date;

    return;

}

void parsing_response(int sockd, http_response *response) {

    char *effective_response = malloc(sizeof(char)*1024);
    memset(effective_response, 0, sizeof(char)*1024);

    strcat(effective_response, response->Header);
    strcat(effective_response ,"\n");
    strcat(effective_response, response->Date);
    strcat(effective_response ,"\n");
    strcat(effective_response, response->Server);
    strcat(effective_response ,"\n");
    //strcat(effective_response, response->Last_Modified);
    //strcat(effective_response ,"\n");
    strcat(effective_response, response->Content_Length);
    strcat(effective_response ,"\n");
    strcat(effective_response, response->Content_Type);
    strcat(effective_response ,"\n");
    strcat(effective_response, response->Connection);
    strcat(effective_response ,"\n");
    strcat(effective_response ,"\n");
    strcat(effective_response, response->Body_Response);

    //printf("%s \n", effective_response);

    //todo: aggiustare write nella socket
    writen(sockd, effective_response, strlen(effective_response) * sizeof(char));

    free(effective_response);


}