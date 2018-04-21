//
// Created by ale96 on 10/04/18.
//

#include "http_management.h"

//TODO: fare funzioni per allocazione e deallocazione
//TODO: RIVEREDE MISURE DELLE MALLOC
//TODO: CONTROLLA ERRORI
//TODO: PROBLEMI IN data (la taglia), content-length (schifezze prima)

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
    response->Header = header;

    // Data field
    char buf[1000];
    time_t now = time(0);
    struct tm tm = *gmtime(&now);
    strftime(buf, sizeof buf, "%a, %d %b %Y %H:%M:%S %Z", &tm);
    response->Date = malloc(sizeof(char)*100);
    char dataHeader[] = "Data: ";
    char *date = strcat(dataHeader, buf);
    response->Date = date;

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

    return;
}

http_response *page_default(char *protocol)
{
    http_response *response;
    off_t dimension;
    int fd;
    char body[1024]; //todo: trasformarlo in cahr *body !!!
    //TODO: risolvere problem con strcat e path assoluto salvato in una variabile
//    char *path = strcpy(path, path_pages);
//    strcat(path, "pageDefault.html");
    char *path = "/home/ale96/Documents/internetWeb/project/Server_ContentAdaptation/pages/pageDefault.html";

    //body = malloc(sizeof(char)*1024);
    //response = malloc(sizeof(http_response));

    if((fd = open(path, 0644, O_RDONLY))== -1)
    {
        fprintf(stderr, "Error on opening file\n");
    }
    if((dimension = lseek(fd, 0, SEEK_END)) == -1)
    {
        fprintf(stderr, "Error on calculating file dimension\n");
    }
    //TODO: mettere controllo sulla read e su close
    lseek(fd, 0, SEEK_SET);
    read(fd, body, dimension);
    close(fd);

    strcat(body, "test1.jpg\n");
    strcat(body, "</body>\n</html>");

    char *header = "HTTP/1.1 200 OK";
    //header = strcat(protocol, " 200 OK");
    header = "HTTP/1.1 200 OK";
    response->Header = header;
    //response.Date = mettere data con una opportuna funzione
    char *server = "Server: nomeServer test";
    response->Server = server;
    //response->Content_Length = strcat("Content-Length: ", (char *)dimension);
    response->Content_Type = "Content-Type: text/html";
    response->Connection = "Connection: close";
    response->Body_Response = body;


    return response;

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
    strcat(effective_response, response->Content_Length);
    strcat(effective_response ,"\n");
    strcat(effective_response, response->Content_Type);
    strcat(effective_response ,"\n");
    strcat(effective_response, response->Connection);
    strcat(effective_response ,"\n");
    strcat(effective_response ,"\n");
    strcat(effective_response, response->Body_Response);

    printf("%s \n", effective_response);

    //todo: mettere write nella socket
    writen(sockd, effective_response, strlen(effective_response) * sizeof(char));


}