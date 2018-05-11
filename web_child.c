//
// Created by ale96 on 11/05/18.
//

#include <fcntl.h>
#include "services/basics.h"
#include "server.h"

void web_child(int sockfd)
{
    printf("Io sono il lavoro del child\n");
}