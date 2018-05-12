//
// Created by ale96 on 28/04/18.
//

//TODO: DECOMMENTARE syslog quando andra' bene (http management totalmente finito)
//TODO: dopo l'HOST va a capo

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
 * This function is used to get the current date according to common log format
 * @Param: none
 * @Return: string representing the current date
 */
char *get_date()
{
    time_t current_time;
    char *c_time_string;
    int len;

    // Get the current time
    current_time = time(NULL);

    if (current_time == ((time_t) -1))
    {
        (void) fprintf(stderr, "Failure to obtain the current time.\n");
        exit(EXIT_FAILURE);
    }

    // Convert to local time format
    c_time_string = ctime(&current_time);
    if (c_time_string == NULL) {
        (void) fprintf(stderr, "Failure to convert the current time.\n");
        exit(EXIT_FAILURE);
    }

    len = (int) strlen(c_time_string);
    c_time_string[len - 1] = '\0';


    return c_time_string;
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
    FILE *log_file;

    // Initialize the mutex used for the logging
    pthread_mutex_t mtx;
    pthread_mutex_init(&mtx, NULL);
    pthread_mutex_lock(&mtx);

    // Set the REAL log
    set_log();
    //syslog(LOG_INFO, "%s %s %s %s '%s' %s %s", req->Host, "-", "-", date, req->GET, res->Header, res->Content_Length);
    closelog();

    // Set the LOCAL FILE log
    memset(log_path, 0, 128);
    errno = 0;
    if (getcwd(log_path, 128) == NULL || errno != 0) {
        perror("getcwd error\n");
        return;
    }
    strcat(log_path, LOG_FILE);

    log_file = fopen(log_path, "a");
    if (log_file == NULL) {
        fprintf(stderr, "open logging file failed\n");
        return;
    }

    fprintf(log_file, "%s %s %s %s '%s' %s %s\n", req->Host, "-", "-", date, req->Request, res->Header, res->Content_Length);

    // Deallocation of file and mutex
    fclose(log_file);
    pthread_mutex_unlock(&mtx);
    pthread_mutex_destroy(&mtx);
}