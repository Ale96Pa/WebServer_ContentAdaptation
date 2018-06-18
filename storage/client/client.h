//
// Created by ale96 on 18/06/18.
//

#ifndef SERVER_CONTENTADAPTATION_CLIENT_H
#define SERVER_CONTENTADAPTATION_CLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ifaddrs.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <errno.h>
#include <arpa/inet.h>
#include <netdb.h>

int find_ip_address(char *hostname, char *ip_address);
void read_file(int fd, char *f);
void set_client_page(char *p1, char *p2);
void create_page(char *url,char *final);
int open_page(int port_number);

#endif //SERVER_CONTENTADAPTATION_CLIENT_H
