#include "basics.h"

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