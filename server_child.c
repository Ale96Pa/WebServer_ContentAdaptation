#include "server.h"

/**
 * This functions are used to create processes (child_make) and to do their work
 * (child_main); in this way there is a modular organization of Server.
 */
void web_child(int sockfd, http_request *request, http_response *response);
void child_main(int listensd, int addrlen);

pid_t child_make(int i, int listensd, int addrlen)
{
    pid_t pid;

    if ((pid = fork()) > 0)
    {
        return pid;		// Parent process
    }

    pid = getpid();
    child_main(listensd, addrlen);

    return pid;
}

void child_main(int listensd, int addrlen)
{
    int	connsd;
    socklen_t clilen;
    struct sockaddr *cliaddr;
    http_request *request = alloc_request();
    http_response *response = alloc_response();

    if ((cliaddr = (struct sockaddr *)malloc(addrlen)) == NULL)
    {
        fprintf(stderr, "Error in malloc\n");
        exit(EXIT_FAILURE);
    }

    for ( ; ; ) {
        clilen = addrlen;
        my_lock_wait();
        if ((connsd = accept(listensd, cliaddr, &clilen)) < 0) {
            perror("Error in accept");
            exit(1);
        }
        my_lock_release();
        web_child(connsd, request, response); // Function that works
        if (close(connsd) == -1) {
            perror("Error in close");
            exit(1);
        }
    }
}