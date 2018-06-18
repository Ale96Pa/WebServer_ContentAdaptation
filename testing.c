#include "server.h"
#include "../Server_ContentAdaptation/caching/caching.h"

static size_t nChildren;
static uint16_t servPort;
static pid_t *pids;

void web_child(int sockfd, http_request *request, http_response *response);
void child_main(int listensd, int addrlen);

/**
 * Control of signal capture receiving a ctrl+C behaviour
 */
typedef void Sigfunc(int);
Sigfunc* signal(int signum, Sigfunc *handler);
Sigfunc *signal(int signum, Sigfunc *func)
{
    // The struct sigaction save info about signal manipulation
    struct sigaction act, oact;

    act.sa_handler = func;
    sigemptyset(&act.sa_mask);    // No other signals to block
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

    // End every child processes
    for (i = 0; i < nChildren; i++)
        kill(pids[i], SIGTERM);
    while (wait(NULL) > 0) ;	// Wait for all child

    if (errno != ECHILD)  {
        fprintf(stderr, "Error in wait\n");
        exit(1);
    }

    exit(EXIT_SUCCESS);
}

/**
 * This is the main  function of Server; here we inizialize the connection,
 * finding the socket descriptor to pass to other functions that work.
 *
 * @Param (cmd line): argv[1] == port number
 *                    argv[2] == number of child processes
 * @Return: EXIT_FAILURE or EXIT_SUCCESS
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
        if(nChildren >= 12)
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
    if (setsockopt(listensd, SOL_SOCKET, SO_REUSEADDR, &tr, sizeof(int)) == -1) {
        perror("setsockopt");
        exit(1);
    }

    memset((void *)&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(servPort);
    addrLen = sizeof(servaddr);

    // Assign an IP address
    if ((bind(listensd, (struct sockaddr *) &servaddr, sizeof(servaddr))) < 0) {
        perror("Error in bind\n");
        exit(EXIT_FAILURE);
    }

    // Keep server listening to request
    if (listen(listensd, BACKLOG) < 0 ) {
        perror("Error in listen\n");
        exit(EXIT_FAILURE);
    }

    my_lock_init("/tmp/lock.XXXXXX");

    // Allocation and creation of children processes
    pids = (pid_t *)calloc(nChildren, sizeof(pid_t));
    if (pids == NULL)
    {
        fprintf(stderr, "Error in calloc\n");
        exit(EXIT_FAILURE);
    }

    for (i=0; i < nChildren; i++)
    {
        pids[i] = child_make(i, listensd, addrLen);

    }

    //When SIGINT arrives (pressing of Ctrl-C) parents and children ends
    if (signal(SIGINT, sig_int) == SIG_ERR)
    {
        fprintf(stderr, "Error in signal\n");
        exit(1);
    }

    for (;;)
        pause();

}

/**
 * This functions are used to create processes (child_make) and to do their work
 * (child_main); in this way there is a modular organization of Server.
 */

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


/**
 * This function implements the whole work of a child, so it gets the request,
 * elaborate the response (also with caching mechanism) and mange the logging.
 * @Param: socked descriptor, request struct, response struct
 * @Return: void
 */
void web_child(int sockfd, http_request *request, http_response *response)
{
    fflush(stdout);

    char *method, *img, *protocol, *host, *q, *user_agent;
    char *path_to_send = malloc(sizeof(char)*DIM_PATH);
    int id_to_catch;

    // Find current date for "Last-Modified" field
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    char date_str[64];
    strftime(date_str, sizeof(date_str), "%c", tm);
    long int last_modified = date_int(date_str);

    for(;;)
    {
        strcpy(request->Request,"GET /ClientPage.html?img10.x=163&img10.y=87 HTTP/1.1\n");
        strcpy(request->Host,"Host: 127.0.0.1:5193\n");
        strcpy(request->User_agent,"User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:60.0) Gecko/20100101 Firefox/60.0\n");
        strcpy(request->Accept,"Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\n");
        strcpy(request->Connection,"Connection: keep-alive\n");

        //parsing_request(sockfd, request);

        if((protocol = parse_protocol(request->Request)) == NULL)
        {
            page_bad_request("HTTP/1.1", "GET", response);
            logging(request, response);
            parsing_response(sockfd, response);
            exit(EXIT_SUCCESS);
        }
        if((method = parse_method(request->Request)) == NULL)
        {
            page_bad_request(protocol, "GET", response);
            logging(request, response);
            parsing_response(sockfd, response);
            exit(EXIT_SUCCESS);
        }
        if((host = parse_host(request->Host)) == NULL)
        {
            page_bad_request(protocol, method, response);
            logging(request, response);
            parsing_response(sockfd, response);
            exit(EXIT_SUCCESS);
        }
        if((img = parse_get(request->Request)) == NULL)
        {
            page_bad_request(protocol, method, response);
            logging(request, response);
            parsing_response(sockfd, response);
            exit(EXIT_SUCCESS);
        }
        if((q = parse_accept(request->Accept))== NULL)
        {
            page_bad_request(protocol, method, response);
            logging(request, response);
            parsing_response(sockfd, response);
            exit(EXIT_SUCCESS);
        }
        if((user_agent = parse_userAgent(request->User_agent))== NULL)
        {
            page_bad_request(protocol, method, response);
            logging(request, response);
            parsing_response(sockfd, response);
            exit(EXIT_SUCCESS);
        }

        id_to_catch = select_id_from_img(img, user_agent, q);

        if(id_to_catch == 0) // The image isn't in DB
        {
            //printf("The image isn't in DB\n");
            double quality;
            if((quality = atof(q)) <= 0)
            {
                fprintf(stderr, "Error in conversion image quality\n");
                exit(EXIT_FAILURE);
            }
            /*
            if((compress_image(img, quality, PATH_MEMORY_CACHE, FORMAT_IMG)) == -1)
            {*/
            page_bad_request(protocol, method, response);
            logging(request, response);
            parsing_response(sockfd, response);
            //exit(EXIT_SUCCESS);
            //}
            int num_current_record = count();

            int id = sel_max_id();

            char *real_path = malloc(sizeof(char)*DIM_PATH);
            sprintf(real_path, "%s%s.%s", PATH_MEMORY_CACHE, img, FORMAT_IMG);

            if(num_current_record < MAX_RECORD_IN_DB)
            {
                insert(id, real_path, img, q, last_modified, user_agent);
            }
            else
            {
                char *id_of_older = malloc(sizeof(char)*7);
                older(id_of_older);
                //delete_remove(atoi(id_of_older));
                insert(id, real_path, img, q, last_modified, user_agent);
            }
        }
        else if(id_to_catch > 0) // The image is in DB
        {
            //printf("The image is in DB\n");
            //update_lastModified(img, user_agent, q);
        } else {
            page_not_found(protocol, method, response);
            logging(request, response);
            parsing_response(sockfd, response);
            exit(EXIT_SUCCESS);
        }

        getcwd(path_to_send, DIM_PATH);
        sprintf(path_to_send, "%s/storage/cache_memory/%s.%s", path_to_send, img, FORMAT_IMG);

        page_default(HTTP1, method, response, path_to_send, date_str);
        logging(request, response);
        parsing_response(sockfd, response);

        fflush(stdout);
    }

}
