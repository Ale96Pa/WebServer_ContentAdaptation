#include "server.h"

void web_child(int);
void child_main(int listensd, int addrlen);

pid_t child_make(int i, int listensd, int addrlen)
{
    pid_t pid;

    if ((pid = fork()) > 0)
    {
        return pid;		/* processo padre */
    }

    pid = getpid();
    child_main(listensd, addrlen);	/* non ritorna mai */
    return pid;
}

// TODO: VEDI se aggiungere int i tra parametri (vedi file lo presti)
void child_main(int listensd, int addrlen)
{
    int	connsd;
    socklen_t clilen;
    struct sockaddr *cliaddr;

    if ((cliaddr = (struct sockaddr *)malloc(addrlen)) == NULL)
    {
        fprintf(stderr, "Error in malloc\n");
        exit(EXIT_FAILURE);
    }

    for ( ; ; ) {
        clilen = addrlen;
        my_lock_wait();
        if ((connsd = accept(listensd, cliaddr, &clilen)) < 0) {
            perror("errore in accept");
            exit(1);
        }
        my_lock_release();
        web_child(connsd);		/* processa la richiesta */
        if (close(connsd) == -1) {
            perror("errore in close");
            exit(1);
        }
    }
}


