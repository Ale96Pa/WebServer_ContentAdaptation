#include "client.h"

int find_ip_address(char *hostname, char *ip_address)
{
    struct hostent *host_name;
    struct in_addr **ipaddress;
    int count;
    if((host_name = gethostbyname(hostname)) == NULL)
    {
        herror("\nIP Address Not Found\n");
        return 1;
    } else {
        ipaddress = (struct in_addr **) host_name->h_addr_list;
        for(count = 0; ipaddress[count] != NULL; count++)
        {
            strcpy(ip_address, inet_ntoa(*ipaddress[count]));
            return 0;
        }
    }
    return 1;
}


void read_file(int fd, char *f)
{
    int rc;

    rc=read(fd,f,4096);
    if(rc==-1)
    {
        fprintf(stderr,"Error read file\n");
        exit(EXIT_FAILURE);
    }
}

void set_client_page(char *p1, char *p2)
{
    int fd;
    int dim;
    char *f;
    int dim_1;
    char *fp;
    int i;

    fd = open("storage/client/startClient.html", O_RDONLY | O_CREAT, 0644);
    if (fd == -1) {
        fprintf(stderr, "Error open file\n");
        exit(EXIT_FAILURE);
    }

    /*dimesione file*/
    dim = (int) lseek(fd, 0, SEEK_END);
    if (dim == -1) {
        fprintf(stderr, "Error Lseek\n");
        exit(EXIT_FAILURE);
    }
    f = (char *) malloc(sizeof(char) * dim);

    if (f == NULL) {
        fprintf(stderr, "Error malloc\n");
        exit(EXIT_FAILURE);
    }

    i = (int) lseek(fd, 0, SEEK_SET);
    if (i == -1) {
        fprintf(stderr, "Error Lseek\n");
        exit(EXIT_FAILURE);
    }

    read_file(fd, f);
    f[strlen(f)] = '\0';
    memcpy(p1, f, strlen(f));



    if (close(fd) == -1) {
        fprintf(stderr, "Error close\n");
        exit(EXIT_FAILURE);
    }

    fd = open("storage/client/endClient.html", O_RDONLY | O_CREAT, 0644);

    if (fd == -1)
    {
        fprintf(stderr, "Error open\n");
        exit(EXIT_FAILURE);
    }

    dim_1 = (int) lseek(fd, 0, SEEK_END);
    if (dim_1 == -1)
    {
        fprintf(stderr, "Error lseek\n");
        exit(EXIT_FAILURE);
    }

    fp = (char *) malloc(sizeof(char) * dim_1);
    if (fp == NULL)
    {
        fprintf(stderr, "Error malloc\n");
        exit(EXIT_FAILURE);
    }

    i = (int) lseek(fd, 0, SEEK_SET);
    if (i == -1)
    {
        fprintf(stderr, "Error lseek\n");
        exit(EXIT_FAILURE);
    }

    read_file(fd, fp);
    fp[strlen(fp)] = '\0';
    memcpy(p2, fp, strlen(fp));


    if (close(fd) == -1)
    {
        fprintf(stderr, "Error close\n");
        exit(EXIT_FAILURE);
    }

    free(f);
    free(fp);
}



void create_page(char *url,char *final)
{

    int fd;
    int w;

    fd=open("storage/client/ClientPage.html", O_RDWR|O_CREAT|O_TRUNC, 0644);

    if(fd==-1){

        fprintf(stderr,"Error open create final\n");
        exit(EXIT_FAILURE);}


    w=write(fd,final,strlen(final));
    if(w==-1)
    {
        fprintf(stderr,"Error write\n");
        exit(EXIT_FAILURE);
    }

}

int open_page(int port_number)
{
    char hostname[50], ip_address[50];
    char *s;
    char *p=":";
    char *str;
    char p1[4096];
    char port[10];
    char p2[4096];
    char final[4096];
    char port_n[10];

    sprintf(port_n,"%d",port_number);
    strcat(port,p);
    strcat(port,port_n);
    str=port;

    sprintf(hostname,"%s","localhost");
    find_ip_address(hostname , ip_address);
    //printf("\nIP Address of Hostname %s:\t%s\n", hostname, ip_address);

    char url[200];
    memset(url, 0, sizeof(char)*100);

    char *client_path = "storage/client/ClientPage.html";
    char *cmd = "xdg-open ";
    char *preamble="http://";
    char *tail="/ClientPage.html";

    char pre[32];
    char t[32];

    memcpy(pre,preamble,strlen(preamble));
    memcpy(t,tail,strlen(tail));


    char client[100];
    char comm[100];

    memcpy(client,client_path,strlen(client_path));
    memcpy(comm,cmd,strlen(cmd));

    strcat(url, comm);
    strcat(url, client);
    //printf("url: %s\n", url);

    set_client_page(p1,p2);
    memset(final,0,4096);
    strcat(final,p1);
    strcat(final,pre);
    strcat(final,ip_address);
    s=strdup(str);
    strcat(final,s);
    strcat(final,t);
    strcat(final,p2);
    final[strlen(final)]='\0';
    //printf("%s\n",final);

    create_page(url,final);

    system(url);
    return 0;
}