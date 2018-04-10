//
// Created by ale96 on 10/04/18.
//

#include "http_management.h"

http_response *page_not_found(char *protocol)
{
    http_response *response;
    off_t dimension;
    int fd;
    char *body;
    //char *path;

    body = malloc(sizeof(char)*1024);
    response = malloc(sizeof(http_response));

    if((fd = open("/home/ale96/Documents/internetWeb/project/Server_ContentAdaptation/pages/pageNotFound.html", 0644, O_RDONLY))== -1)
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

    printf("%s\n", body);

    char *header = "HTTP/1.1 404 Not Found";
    //header = strcat(protocol, " 404 Page Not Found");
    header = "HTTP/1.1 404 Not Found";
    response->Header = header;
    //response.Date = mettere data con una opportuna funzione
    char *server = "Server: nomeServer test";
    response->Server = server;
    //response->Content_Length = strcat("Content-Length: ", (char *)dimension);
    response->Content_Type = "Content-Type: text/html";
    response->Connection = "Connection: close";
    response->Body_Response = body;

    free(body);

    return response;
}

int main(void)
{
    http_response *res = page_not_found("HTTP/1.1");
    printf("%s\n", res->Body_Response);
}


