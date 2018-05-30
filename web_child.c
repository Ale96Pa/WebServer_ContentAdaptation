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
    char *path_to_catch = (char *) malloc(sizeof(char)*DIM_PATH);

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

        //printf("begin msg:\n%s\n%s\n%s\n%s\n%s\n%s\nend msg\n", protocol, method, host, img, q, user_agent);
        //printf("begin request:\n%s\n%s\n%s\n%s\n%s\n", request->Request, request->Accept, request->Host, request->User_agent, request->Connection);

/*
        printf("funzione caching\n");
        select_path_from_img(img, user_agent, q, path_to_catch);
        printf("path: %s\n", path_to_catch);

        if(strcmp(path_to_catch, "") == 0) // The image isn't in DB
        {
            printf("L'immagine NON e' nel DB\n");
            double quality;
            if((quality = atof(q)) <= 0)
            {
                fprintf(stderr, "Error in conversion image quality\n");
                exit(EXIT_FAILURE);
            }
            compress_image(img, quality, PATH_MEMORY_CACHE, "jpg");
            int num_current_record = count();

            // inserisci funzione per ottenere id
            //todo: togli quando ci sarà funzione
            int id = 45;
            //inserisci funzione per ottenere path
            //todo: modifica il path== PATH_MEMORY_CACHE + nome immagine
            last_modified = get_date();
            printf("Last modified: %s\n", last_modified);

            if(num_current_record < MAX_RECORD_IN_DB)
            {
                insert(id, PATH_MEMORY_CACHE, img, q, last_modified, user_agent);
            }
            else
            {
                char *id_of_older = malloc(sizeof(char)*7);
                older(id_of_older);
                delete(atoi(id_of_older));
                insert(id, PATH_MEMORY_CACHE, img, q, last_modified, user_agent);

            }
        }
        else if(strlen(path_to_catch) > 5) // The image is in DB
        {
            printf("L'immagine e' nel DB\n");
            update_lastModified(img, user_agent, q);
        } else {
            page_not_found(protocol, method, response);
            logging(request, response);
            parsing_response(sockfd, response);
            exit(EXIT_SUCCESS);
        }
*/
        // TODO: MODIFICARE ASSOLUTAMENTE
        path_to_send = "storage/cache_memory/img1.jpg";
        last_modified = "2018-8-8";
        page_default("HTTP/1.1", method, response, path_to_send, last_modified);
        logging(request, response);
        parsing_response(sockfd, response);

        //printf("%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n", response->Header, response->Server, response->Connection, response->Content_Length, response->Content_Type, response->Date, response->Last_Modified, response->Body_Response);
    }

}