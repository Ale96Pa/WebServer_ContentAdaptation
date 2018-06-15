//TODO: DECOMMENTARE syslog quando tutto e' finito

#include "logging.h"

/**
 * This function is used to set the log to use it
 * @Param: none
 * @Return: void
 */
void set_log()
{
    setlogmask(LOG_UPTO(LOG_INFO));
    openlog("WebServer", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_USER);
    return;
}

/**
 * This function write log on two files: a local file "logServer.txt" and the Unix syslog
 * @Param: both structures of request and response (from them we get data)
 * @Return: void
 */
void logging(http_request *req, http_response *res)
{
    char *date = get_date();
    char log_path[128];
    char *request = malloc(sizeof(char)*DIM_LONG);
    FILE *log_file;

    // Set the request field
    strcpy(request, req->Request);
    strtok(request, "H");

    // Initialize the mutex used for the logging
    pthread_mutex_t mtx;
    pthread_mutex_init(&mtx, NULL);
    pthread_mutex_lock(&mtx);

    // Set the REAL log
    set_log();
    //syslog(LOG_INFO, "%s %s '%s' %s %s", req->Host, date, request, res->Header, res->Content_Length);
    closelog();

    // Set the LOCAL FILE log
    memset(log_path, 0, 128);
    errno = 0;
    if (getcwd(log_path, 128) == NULL || errno != 0)
    {
        perror("getcwd error\n");
        return;
    }
    strcat(log_path, LOG_FILE);

    log_file = fopen(log_path, "a");
    if (log_file == NULL)
    {
        fprintf(stderr, "Error on opening logging file\n");
        return;
    }
    fprintf(log_file, "%s - %s '%s' %s %s\n", req->Host, date, request, res->Header, res->Content_Length);

    // Deallocation of file and mutex
    fclose(log_file);
    pthread_mutex_unlock(&mtx);
    pthread_mutex_destroy(&mtx);
}