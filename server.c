//TODO: aggiusta header
//TODO: cambia nomi file

#include "server.h"


static size_t nChildren;
static uint16_t servPort;
static pid_t *pids;

typedef void Sigfunc(int);
Sigfunc* signal(int signum, Sigfunc *handler);
Sigfunc *signal(int signum, Sigfunc *func)
{
    struct sigaction      act, oact;
    /* la struttura sigaction memorizza informazioni riguardanti la
    manipolazione del segnale */

    act.sa_handler = func;
    sigemptyset(&act.sa_mask);    /* non occorre bloccare nessun altro segnale */
    act.sa_flags = 0;
    if (signum != SIGALRM)
        act.sa_flags |= SA_RESTART;
    if (sigaction(signum, &act, &oact) < 0)
        return(SIG_ERR);
    return(oact.sa_handler);
}

void sig_int(int signo)
{
    int	i;
//    void pr_cpu_time(void);

    /* termina tutti i processi child */
    for (i = 0; i < nChildren; i++)
        kill(pids[i], SIGTERM);
    while (wait(NULL) > 0) ;	/* attende per tutti i processi child */

    if (errno != ECHILD)  {
        fprintf(stderr, "errore in wait");
        exit(1);
    }

//    pr_cpu_time();
    exit(EXIT_SUCCESS);
}

/*
 * argv[1] == NUMERO DI PORTA
 * argv[2] == NUMERO DI FIGLI
 *
 */
int main(int argc, char *argv[])
{
    int listensd, i;
    struct sockaddr_in servaddr;
    socklen_t addrLen;

    // Check and set values for the Server installation
    if(argc == 1)
    {
        servPort = SERV_PORT;
        nChildren = NUMBER_CHILD;
    }
    else if(argc == 2)
    {
        if((servPort = (uint16_t) atoi(argv[1])) <= 0 )
        {
            fprintf(stderr, "Error on assign port number\n"
                            "Instruction: ./server <port_number> <number_child>\n");
            exit(EXIT_FAILURE);
        }
        nChildren = NUMBER_CHILD;
    }
    else if(argc == 3)
    {
        if((servPort = (uint16_t) atoi(argv[1])) <= 0 )
        {
            fprintf(stderr, "Error on assign port number\n"
                            "Instruction: ./server <port_number> <number_child>\n");
            exit(EXIT_FAILURE);
        }
        if((nChildren = (size_t) atoi(argv[2])) <= 0 )
        {
            fprintf(stderr, "Error on assign number of child\n"
                            "Instruction: ./server <port_number> <number_child>\n");
            exit(EXIT_FAILURE);
        }
        if(nChildren >= 11)
        {
            fprintf(stderr, "Error: maximum number of children is 10\n"
                            "Instruction: ./server <port_number> <number_child>\n");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        fprintf(stderr, "Error on running server\nInstruction: ./server <port_number> <number_child>\n");
        exit(EXIT_FAILURE);
    }

    // Put the server listening to request
    if ((listensd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Error in socket\n");
        exit(EXIT_FAILURE);
    }

    int tr=1;
    // kill "Address already in use" error message
    if (setsockopt(listensd,SOL_SOCKET,SO_REUSEADDR,&tr,sizeof(int)) == -1) {
        perror("setsockopt");
        exit(1);
    }

    memset((void *)&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(servPort);
    addrLen = sizeof(servaddr);

    //
    if ((bind(listensd, (struct sockaddr *) &servaddr, sizeof(servaddr))) < 0) {
        perror("Error in bind\n");
        exit(EXIT_FAILURE);
    }

    //
    if (listen(listensd, BACKLOG) < 0 ) {
        perror("Error in listen\n");
        exit(EXIT_FAILURE);
    }

    my_lock_init("/tmp/lock.XXXXXX");

    //
    pids = (pid_t *)calloc(nChildren, sizeof(pid_t));
    if (pids == NULL)
    {
        fprintf(stderr, "Error in calloc\n");
        exit(EXIT_FAILURE);
    }

    for (i=0; i<nChildren; i++)
    {
        pids[i] = child_make(i, listensd, addrLen);
    }

    //When SIGINT arrives (pressing of Ctrl-C) parents and children ends
    if (signal(SIGINT, sig_int) == SIG_ERR)
    {
        fprintf(stderr, "errore in signal");
        exit(1);
    }

    for (;;)
        pause();

}