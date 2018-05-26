//TODO: cambia nomi variabili

#include "server.h"
#include "caching/caching.h"

void web_child(int sockfd, http_request *request, http_response *response)
{
    int	ntowrite;
    ssize_t	nread;
    char line[MAXLINE];
    char *method, *img, *protocol, *host, *q, *user_agent;
    char *path_to_send, *last_modified;
    char *path_to_catch = malloc(sizeof(char)*DIM_PATH);

    for(;;)
    {
        parsing_request(sockfd, request);
        if((protocol = parse_protocol(request->Request)) == NULL)
        {
            page_bad_request("HTTP/1.1", "GET", response);
            logging(request, response);
            parsing_response(sockfd, response);
            exit(EXIT_SUCCESS);
        }
        if((method = parse_method(request->Request)) == NULL)
        {
            page_bad_request(protocol, "GET", response);
            logging(request, response);
            parsing_response(sockfd, response);
            exit(EXIT_SUCCESS);
        }
        if((host = parse_host(request->Host)) == NULL)
        {
            page_bad_request(protocol, method, response);
            logging(request, response);
            parsing_response(sockfd, response);
            exit(EXIT_SUCCESS);
        }
        if((img = parse_get(request->Request)) == NULL)
        {
            page_bad_request(protocol, method, response);
            logging(request, response);
            parsing_response(sockfd, response);
            exit(EXIT_SUCCESS);
        }
        if((q = parse_accept(request->Accept))== NULL)
        {
            page_bad_request(protocol, method, response);
            logging(request, response);
            parsing_response(sockfd, response);
            exit(EXIT_SUCCESS);
        }
        if((user_agent = parse_userAgent(request->User_agent))== NULL)
        {
            page_bad_request(protocol, method, response);
            logging(request, response);
            parsing_response(sockfd, response);
            exit(EXIT_SUCCESS);
        }

        // CACHING PER VERIFICARE SE C'E' IMMAGINE RICHIESTA
        select_path_from_img(img, user_agent, q, path_to_catch);

        if(path_to_catch == NULL)
        {
            double quality;

            if((quality = atof(q)) <= 0)
            {
                fprintf(stderr, "Error in conversion image quality\n");
                exit(EXIT_FAILURE);
            }
            compress_image(img, quality, PATH_MEMORY_CACHE, "jpg");
            int num_current_record = count();
            if(num_current_record < MAX_RECORD_IN_DB)
            {

            }
            //METTI IMMAGINE NEL DB
            // setta variabile last_modified
            printf("Entrato in IF\n");
        }
        else if(strlen(path_to_catch) > 5)
        { //immagine c'è nella cache
            // FUNZIONI NEL DB PER PRENDERE IMMAGINE, aggiornare last_modified, etc... : torna un percorso
            // setta variabile last_modified
            printf("Entrato in ELSE IF\n");

        } else {
            page_not_found(protocol, method, response);
            logging(request, response);
            parsing_response(sockfd, response);
            exit(EXIT_SUCCESS);
        }

        // TODO: MODIFICARE ASSOLUTAMENTE
        path_to_send = "cache_memory/img1.jpg";
        last_modified = "2018-8-8";

        page_default(protocol, method, response, path_to_send, last_modified);
        logging(request, response);
        parsing_response(sockfd, response);
        free(path_to_catch);
    }
}