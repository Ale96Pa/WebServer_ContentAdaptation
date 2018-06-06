//todo: http come variabile (non stringa sempre uguale, se possibile!!) ==> stessa cosa q
//TODO: togli immagini anche dalla cartella della cache

#include "server.h"
#include "caching/caching.h"

void web_child(int sockfd, http_request *request, http_response *response)
{
    char *method, *img, *protocol, *host, *q, *user_agent;
    char *path_to_send = malloc(sizeof(char)*DIM_PATH);
    int id_to_catch;

    // Find current date for "Last-Modified" field
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    char date_str[64];
    strftime(date_str, sizeof(date_str), "%c", tm);
    long int last_modified = date_int(date_str);

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

        //TODO: la Q di merda mi manda a puttane la funzione !!!!!!!!!!!!!!!!!!! (inserisce \n)
        //printf("%s\n", q);
        id_to_catch = select_id_from_img(img, user_agent, "0.8");

        if(id_to_catch == 0) // The image isn't in DB
        {
            printf("The image isn't in DB\n");
            double quality;
            if((quality = atof(q)) <= 0)
            {
                fprintf(stderr, "Error in conversion image quality\n");
                exit(EXIT_FAILURE);
            }
            compress_image(img, quality, PATH_MEMORY_CACHE, FORMAT_IMG);
            int num_current_record = count();

            int id = sel_max_id();

            char *real_path = malloc(sizeof(char)*DIM_PATH);
            sprintf(real_path, "%s%s.%s", PATH_MEMORY_CACHE, img, FORMAT_IMG);

            if(num_current_record < MAX_RECORD_IN_DB)
            {
                insert(id, real_path, img, q, last_modified, user_agent);
            }
            else
            {
                char *id_of_older = malloc(sizeof(char)*7);
                older(id_of_older);
                delete(atoi(id_of_older));
                insert(id, real_path, img, q, last_modified, user_agent);
            }
        }
        else if(id_to_catch > 0) // The image is in DB
        {
            printf("The image is in DB\n");
            update_lastModified(img, user_agent, "0.8");
        } else {
            page_not_found(protocol, method, response);
            logging(request, response);
            parsing_response(sockfd, response);
            exit(EXIT_SUCCESS);
        }

        getcwd(path_to_send, DIM_PATH);
        sprintf(path_to_send, "%s/storage/cache_memory/%s.%s", path_to_send, img, FORMAT_IMG);

        page_default("HTTP/1.1", method, response, path_to_send, date_str);
        logging(request, response);
        parsing_response(sockfd, response);

        //printf("%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n", response->Header, response->Server, response->Connection, response->Content_Length, response->Content_Type, response->Date, response->Last_Modified, response->Body_Response);
    }

}