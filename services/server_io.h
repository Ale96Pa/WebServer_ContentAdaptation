//
// Created by ale96 on 04/04/18.
//

#ifndef SERVER_CONTENTADAPTATION_SERVER_IO_H
#define SERVER_CONTENTADAPTATION_SERVER_IO_H

#include "basics.h"

ssize_t	writen(int fd, const void *buf, size_t n);
int	readline(int fd, void *vptr, int maxlen);

#endif //SERVER_CONTENTADAPTATION_SERVER_IO_H
