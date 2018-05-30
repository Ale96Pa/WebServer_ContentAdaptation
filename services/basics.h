#ifndef SERVER_CONTENTADAPTATION_BASIC_H
#define SERVER_CONTENTADAPTATION_BASIC_H

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAXLINE 1024
#define DIM_PATH 100


ssize_t	writen(int fd, const void *buf, size_t n);
int	readline(int fd, void *vptr, int maxlen);
char *get_date(void);

#endif //SERVER_CONTENTADAPTATION_BASIC_H
