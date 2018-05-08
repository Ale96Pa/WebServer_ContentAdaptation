#ifndef SERVER_CONTENTADAPTATION_HTTP_MANAGEMENT_H
#define SERVER_CONTENTADAPTATION_HTTP_MANAGEMENT_H

#include "../services/basics.h"
#include "../services/server_io.h"
#include <fcntl.h>
#include <time.h>

#define DIM_LONG 256    // Length of "relatively" long fields
#define DIM_SHORT 64    // Length of "relatively" short fields
#define DIM_HTML 1024   // Length of html page for the response

typedef struct http_request{
    char GET[DIM_LONG];
    char Protocol[DIM_SHORT];
    char Host[DIM_SHORT];
    char User_agent[DIM_LONG];
    char Connection[DIM_SHORT];
    char Accept[DIM_LONG];
} http_request;

typedef struct http_response{
    char Header[DIM_SHORT];
    char Date[DIM_LONG];
    char Server[DIM_SHORT];
    char Last_Modified[DIM_SHORT];
    char Content_Length[DIM_SHORT];
    char Content_Type[DIM_SHORT];
    char Connection[DIM_SHORT];
    char Body_Response[DIM_HTML];
} http_response;

http_request *alloc_request();
char *parse_get(char *whole_msg);
char *parse_protocol(char *whole_msg);
char *parse_host(char *whole_msg);
char *parse_accept(char *whole_msg);
char *parse_userAgent(char *msg_userAgent);
void parsing_request(int sockd, http_request *request);


http_response *alloc_response();
void page_not_found(char *protocol, http_response *response);
void page_bad_request(char *protocol, http_response *response);
void page_no_content(char *protocol, http_response *response);
void page_default(char *protocol, http_response *response, char *path, char *last_modified);
void parsing_response(int sockd, http_response *response);


#endif //SERVER_CONTENTADAPTATION_HTTP_MANAGEMENT_H