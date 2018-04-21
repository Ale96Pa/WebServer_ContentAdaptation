//
// Created by ale96 on 04/04/18.
//

#include "basics.h"
#include "http_management.h"


int main(int argc, char **argv)
{
    pid_t			pid;
    int			listensd, connsd;
    struct sockaddr_in	servaddr, cliaddr;
    unsigned int		len;
    http_request *request;
    http_response *response;
    char *msg;

    if ((listensd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("errore in socket");
        exit(1);
    }

    // MESSO PER EVITARE ERRORE IN BIND
    int tr=1;
    // kill "Address already in use" error message
    if (setsockopt(listensd,SOL_SOCKET,SO_REUSEADDR,&tr,sizeof(int)) == -1) {
        perror("setsockopt");
        exit(1);
    }

    memset((void *)&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port	   = htons(SERV_PORT);

    if ((bind(listensd, (struct sockaddr *)&servaddr, sizeof(servaddr))) < 0) {
        perror("errore in bind");
        exit(1);
    }

    if (listen(listensd, BACKLOG) < 0 ) {
        perror("errore in listen");
        exit(1);
    }

    for ( ; ; ) {
        len = sizeof(cliaddr);
        if ((connsd = accept(listensd, (struct sockaddr *)&cliaddr, &len)) < 0) {
            perror("errore in accept");
            exit(1);
        }

        if ((pid = fork()) == 0) {
            if (close(listensd) == -1) { //close the socket that only LISTEN (passive socket)
                perror("errore in close");
                exit(1);
            }
            printf("%s:%d connesso\n", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));


// BEGIN PARSING REQUEST
            request = alloc_request();
            parsing(connsd, request);	/* svolge il lavoro del server */
            //printf("%s\n%s\n ", request->GET, request->Host);
            //msg = parse_get(request->GET);
            //printf("%s \n", msg);
            //free_request(request);
            free(request);
// END PARSING REQUEST

// BEGIN RESPONSE
            response = alloc_response();
            page_not_found("HTTP/1.1", response);
            printf("%s \n", response->Date);
            parsing_response(connsd, response);
            free(response);
// END RESPONSE


            if (close(connsd) == -1) {
                perror("errore in close");
                exit(1);
            }
            exit(0);
        } /* end fork */

        if (close(connsd) == -1) {	/* processo padre */
            perror("errore in close");
            exit(1);
        }
    } /* end for */
}
