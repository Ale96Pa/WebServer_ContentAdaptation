#ifndef SERVER_CONTENTADAPTATION_BASIC_H
#define SERVER_CONTENTADAPTATION_BASIC_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define SERV_PORT	5193
#define BACKLOG		10
#define MAXLINE		1024
#define NUMBER_CHILD 5

ssize_t	writen(int fd, const void *buf, size_t n);
int	readline(int fd, void *vptr, int maxlen);

#endif //SERVER_CONTENTADAPTATION_BASIC_H
