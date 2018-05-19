//TODO: VEDERE QUALI FILE HEADER
//TODO: INSERISCI FUNZIONI PER SAPERE INDIRIZZO IP E NUMERO DI PORTA
//TODO: VEDI COME INSERIRE QUESTO FILE PER IL FUNZIONAMENTO DEL SERVER
//TODO: INSERISCI ALTRE IMMAGINI
//TODO: inserisci controllo degli errori

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


void set_client_page(char *ip_addr_port)
{
    FILE *fdStart = fopen("client/startClient.html", "rb");
    fseek(fdStart, 0, SEEK_END);
    long fsizeStart = ftell(fdStart);
    fseek(fdStart, 0, SEEK_SET);  //same as rewind(fdStart);
    char *header = malloc(fsizeStart + 1);
    fread(header, fsizeStart, 1, fdStart);
    fclose(fdStart);
    header[fsizeStart] = 0;

    FILE *fdEnd = fopen("client/endClient.html", "rb");
    fseek(fdEnd, 0, SEEK_END);
    long fsizeEnd = ftell(fdEnd);
    fseek(fdEnd, 0, SEEK_SET);  //same as rewind(fdStart);
    char *tile = malloc(fsizeEnd + 1);
    fread(tile, fsizeEnd, 1, fdEnd);
    fclose(fdEnd);
    tile[fsizeEnd] = 0;

    char *action = malloc(sizeof(char)*70);
    memset(action,0, sizeof(char)*strlen(action));
    strcat(action, "http://");
    strcat(action, ip_addr_port);
    strcat(action, "//client.html");
    char *html = malloc(fsizeStart+fsizeEnd+strlen(action)+1);
    memset(html,0,sizeof(char)*strlen(html));
    strcat(html, header);
    strcat(html, action);
    strcat(html, tile);

    FILE *f = fopen("client/ClientPage.html", "w");
    if (f == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }
    fprintf(f, "%s\n", html);
    fclose(f);

    free(header);
    free(tile);
    free(html);
    free(action);
}

int main()
{
    char *url = malloc(sizeof(char)*100);
    memset(url, 0, sizeof(char)*100);

    char *client_path = "/client/ClientPage.html";
    char *header = "file://";
    char *dir = get_current_dir_name();
    char *cmd = "xdg-open ";

    strcat(url, cmd);
    strcat(url, header);
    strcat(url, dir);
    strcat(url, client_path);
    system (url);
    set_client_page("127.0.0.1:5193");
    return 0;
}