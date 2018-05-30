#include "basics.h"
#include <time.h>
#include <fcntl.h>

static struct flock	lock_it, unlock_it;
static int lock_fd = -1;

/**
 * This function write n Bytes from a buffer
 * @Param: file_descriptor, buffer in wich write, size of buffer
 * @Return: Bytes written
 */
ssize_t writen(int fd, const void *buf, size_t n)
{
    size_t nleft;
    ssize_t nwritten;
    const char *ptr;

    ptr = buf;
    nleft = n;
    while (nleft > 0) {
        if ((nwritten = write(fd, ptr, nleft)) <= 0) {
            if ((nwritten < 0) && (errno == EINTR)) nwritten = 0;
            else return(-1); //Error
        }
        nleft -= nwritten;
        ptr += nwritten;
    }
    return(n-nleft);
}

/**
 * This function read a line from a block
 * @Param: file_descriptor, pointer to block to read, size of line
 * @Return: Bytes read
 */
int readline(int fd, void *vptr, int maxlen)
{
    int  n, rc;
    char c, *ptr;

    ptr = vptr;
    for (n = 1; n < maxlen; n++) {
        if ((rc = read(fd, &c, 1)) == 1) {
            *ptr++ = c;
            if (c == '\n') break;
        }
        else
        if (rc == 0) {              // The EOF is read
            if (n == 1) return(0);	// Quit but read nothing
            else break;
        }
        else return(-1); // Error
    }

    *ptr = 0;	// To indicate end of the input
    return(n);
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
    if (c_time_string == NULL)
    {
        (void) fprintf(stderr, "Failure to convert the current time.\n");
        exit(EXIT_FAILURE);
    }

    len = (int) strlen(c_time_string);
    c_time_string[len - 1] = '\0';

    return c_time_string;
}

/**
 * This function is used to initializa a lock: fcntl will fail if thi function is not called
 * @Param path to lock
 * @Return: void
 */
void my_lock_init(char *pathname)
{
    char lock_file[1024];

    // Must copy caller's string, in case it's a constant
    strncpy(lock_file, pathname, sizeof(lock_file));
    if ((lock_fd = mkstemp(lock_file)) < 0)
    {
        fprintf(stderr, "Error in mkstemp\n");
        exit(1);
    }

    if (unlink(lock_file) == -1)  // But lock_fd remains open
    {
        fprintf(stderr, "Error in unlink for %s\n", lock_file);
        exit(1);
    }
    lock_it.l_type = F_WRLCK;
    lock_it.l_whence = SEEK_SET;
    lock_it.l_start = 0;
    lock_it.l_len = 0;

    unlock_it.l_type = F_UNLCK;
    unlock_it.l_whence = SEEK_SET;
    unlock_it.l_start = 0;
    unlock_it.l_len = 0;
}

/**
 * This function is used to wait a lock
 * @Param: void
 * @Return: void
 */
void my_lock_wait()
{
    int rc;

    while ( (rc = fcntl(lock_fd, F_SETLKW, &lock_it)) < 0)
    {
        if (errno == EINTR)
            continue;
        else {
            fprintf(stderr, "Error fcntl in my_lock_wait\n");
            exit(1);
        }
    }
}

/**
 * This function is used to release a lock
 * @Param: void
 * @Return: void
 */
void my_lock_release()
{
    if (fcntl(lock_fd, F_SETLKW, &unlock_it) < 0)
    {
        fprintf(stderr, "Error fcntl in my_lock_release\n");
        exit(1);
    }
}