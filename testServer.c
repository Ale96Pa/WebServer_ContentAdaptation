#include "services/basics.h"
#include "http/http_management.h"
#include "logging/logging.h"


int main(int argc, char **argv)
{
    pid_t			pid;
    int			listensd, connsd;
    struct sockaddr_in	servaddr, cliaddr;
    unsigned int		len;
    http_request *request;
    http_response *response;
    char *msg0, *msg1, *msg2, *msg3, *msg4, *msg5;

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
            //printf("%s:%d connesso\n", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));


// BEGIN PARSING REQUEST
            request = alloc_request();

            parsing_request(connsd, request);	/* svolge il lavoro del server */
            msg0 = parse_method(request->Method);
            msg1 = parse_protocol(request->Method);
            msg2 = parse_host(request->Host);
            msg3 = parse_get(request->Method);
            msg4 = parse_accept(request->Accept);
            msg5 = parse_userAgent(request->User_agent);

// END PARSING REQUEST

// BEGIN RESPONSE
            response = alloc_response();
            //page_not_found("HTTP/1.1", response);
            page_bad_request(msg1, response);
            //page_default("HTTP/1.1", response, "images/test1.jpg", "2018-5-78");
            parsing_response(connsd, response);

// END RESPONSE

// BEGIN LOGGING
            logging(request, response);
            printf("%s\n", msg0);
/*
            printf("begin msg: %s\n%s\n%s\n%s\n%s\n end msg\n", msg1, msg2, msg3, msg4, msg5);
            printf("begin field: %s\n%s\n%s\n%s\n%s\n%s\nend field\n", request->User_agent,
                request->GET, response->Last_Modified, response->Header, response->Date, request->Protocol);
*/
            free(request);
            free(response);
// END LOGGING


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
