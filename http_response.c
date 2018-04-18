//
// Created by ale96 on 10/04/18.
//

#include "http_management.h"

//TODO: VEDERE BENE SE E' UN PROBLEMA LA CONCORRENZA QUANDO SI APRONO I FILES (i CURSORI VENGONO CONIDVISI?!?)
//TODO: fare funzioni per allocazione e deallocazione

http_response *page_not_found(char *protocol)
{
    http_response *response;
    off_t dimension;
    int fd;
    char *body;
    //TODO: risolvere problem con strcat e path assoluto salvato in una variabile
    //char *path = strcpy(path, path_pages);
    //strcat(path, "pageNotFound.html");
    char *path = "/home/ale96/Documents/internetWeb/project/Server_ContentAdaptation/pages";


    body = malloc(sizeof(char)*1024);
    response = malloc(sizeof(http_response));

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

    char *header = "HTTP/1.1 404 Not Found";
    //header = strcat(protocol, " 404 Page Not Found");
    header = "HTTP/1.1 404 Not Found\n";
    response->Header = header;
    //response.Date = mettere data con una opportuna funzione
    char *server = "Server: nomeServer test\n";
    response->Server = server;
    //response->Content_Length = strcat("Content-Length: ", (char *)dimension);
    response->Content_Type = "Content-Type: text/html\n";
    response->Connection = "Connection: close\n";
    response->Body_Response = body;

    //free(body);

    return response;
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
    response = malloc(sizeof(http_response));

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

    //printf("%s\n", body);

    //free(body);


    return response;

}

void parsing_response(int sockd, http_response *response)
{
    char effective_response[MAXLINE];
    int n; //number of bytes read
    char *p;
/*
    response->Server = NULL;
    response->Header = NULL;
    response->Content_Type = NULL;
    response->Content_Length = NULL;
    response->Connection = NULL;
    response->Body_Response = NULL;
    response->Last_Modified = NULL;
    response->Date = NULL;
*/
    printf("qui arrivo, %d\n", 10);

    strcat(effective_response, response->Header);
    strcat(effective_response, response->Date);
    strcat(effective_response, response->Server);
    strcat(effective_response, response->Last_Modified);
    strcat(effective_response, response->Content_Length);
    strcat(effective_response, response->Content_Type);
    strcat(effective_response, response->Connection);
    strcat(effective_response, response->Body_Response);

    printf("%s\n", effective_response);
    printf("qui arrivo, %d\n", strlen(effective_response));

    //todo: mettere write nella socket
    writen(sockd, effective_response, strlen(effective_response)*sizeof(char));


}

/*
int main(void)
{
    fflush(stdout);
    http_response *res = page_default("HTTP/1.1");
    printf("%s\n%s\n", res->Content_Type, res->Body_Response);
}
*/

