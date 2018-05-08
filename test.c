//
// Created by ale96 on 10/04/18.
//
#include "services/basics.h"
#include "services/server_io.h"
#include <fcntl.h>

int main(void)
{
    char *buf1;
    char *buf2;

    buf1 = malloc(sizeof(char)*50);
    strcpy(buf1, "Ciao ");
    strcpy(buf2, "a tutti.");

    strcat(buf1, buf2);

    printf("%s\n", buf1);
    free(buf1);
}
