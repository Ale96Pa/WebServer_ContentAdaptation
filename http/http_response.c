#include "http_management.h"
#include "../server.h"

char *header_html = "<!DOCTYPE html>\n"
                    "<html>\n"
                    "<head>\n"
                    "\t<title> Adapt Image </title>\n"
                    "</head>\n"
                    "\n"
                    "<body bgcolor=\"#f0f8ff\">\n"
                    "\t<h2> Dynamic image adaptation </h2>\n"
                    "\t<br><br>\n";

/**
  * This function allocate memory for the whole response
  * @Param: none
  * @Return: pointer to allocated struct for the request
  */
http_response *alloc_response()
{
    http_response *message;
    if((message = malloc(sizeof(http_response))) == NULL)
    {
        perror("Error in malloc: struct for http_msg not allocated\n");
        exit(EXIT_FAILURE);
    }
    return message;
}

/**
  * This function manages the page-not-found response, setting the response
  * @Param: protocol, struct to set
  * @Return: void
  */
void page_not_found(char *protocol, char *method, http_response *response)
{
    // Header field
    char *protocol_allocate=malloc(sizeof(char)*DIM_SHORT);
    strcpy(protocol_allocate, protocol);
    char *header = strcat(protocol_allocate, " 404 Not Found");
    strcpy(response->Header, header);
    free(protocol_allocate);

    // Server field
    char server[] = "Server: AGA server";
    strcpy(response->Server, server);

    // Content-Type field
    char type[] = "Content-Type: text/html";
    strcpy(response->Content_Type, type);

    // Connection field
    char conn[] = "Connection: close";
    strcpy(response->Connection, conn);

    // Last-modified field
    char lm[] = "Last-Modified: NULL";
    strcpy(response->Last_Modified, lm);

    // Body message
    char *body = malloc(sizeof(char)*DIM_HTML);
    if (strcmp(method, "HEAD") == 0) {
        strcpy(response->Body_Response, "");
    } else {
        strcpy(body, header_html);
        strcat(body, "<br><h1><b><center>error 404 PAGE NOT FOUND</center></b></h1></body></html>");
        strcpy(response->Body_Response, body);
    }

    // Content-Length field
    /*size_t len = strlen(body);
    char lenTxt[DIM_SHORT];
    char lenHeader[] = "Content-Length: ";
    char lenStr[8];
    snprintf(lenStr, sizeof lenStr, "%zu", len);
    strcat(lenTxt, lenHeader);
    strcat(lenTxt, lenStr);
    strcpy(response->Content_Length, lenTxt);*/
    strcpy(response->Content_Length, "Content-Length: 212");

    // Data field
    char buf[DIM_LONG];
    time_t now = time(0);
    struct tm tm = *gmtime(&now);
    strftime(buf, sizeof buf, "%a, %d %b %Y %H:%M:%S %Z", &tm);
    char dataHeader[] = "Data: ";
    char *date = strcat(dataHeader, buf);
    strcpy(response->Date, date);
}

/**
  * This function manages the bad-request response, setting the response
  * @Param: protocol, struct to set
  * @Return: void
  */
void page_bad_request(char *protocol, char *method, http_response *response)
{
    // Header field
    char *protocol_allocate=malloc(sizeof(char)*DIM_SHORT);
    strcpy(protocol_allocate, protocol);
    char *header = strcat(protocol_allocate, " 300 Bad Request");
    strcpy(response->Header, header);

    // Server field
    char server[] = "Server: AGA server";
    strcpy(response->Server, server);

    // Content-Type field
    char type[] = "Content-Type: text/html";
    strcpy(response->Content_Type, type);

    // Connection field
    char conn[] = "Connection: close";
    strcpy(response->Connection, conn);

    // Last-modified field
    char lm[] = "Last-Modified: NULL";
    strcpy(response->Last_Modified, lm);

    // Body message
    char *body = malloc(sizeof(char)*DIM_HTML);
    if (strcmp(method, "HEAD") == 0) {
        strcpy(response->Body_Response, "");
    } else {
        strcpy(body, header_html);
        strcat(body, "<br><h1><b><center>error 300 BAD REQUEST</center></b></h1></body></html>");
        strcpy(response->Body_Response, body);
    }

    // Content-Length field
    /*
    size_t len = strlen(body);
    char lenTxt[DIM_SHORT];
    char lenHeader[] = "Content-Length: ";
    char lenStr[8];
    snprintf(lenStr, sizeof lenStr, "%zu", len);
    strcat(lenTxt, lenHeader);
    strcat(lenTxt, lenStr);
    strcpy(response->Content_Length, lenTxt);*/
    strcpy(response->Content_Length, "Content-Length: 213");


    // Data field
    char buf[DIM_LONG];
    time_t now = time(0);
    struct tm tm = *gmtime(&now);
    strftime(buf, sizeof buf, "%a, %d %b %Y %H:%M:%S %Z", &tm);
    char dataHeader[] = "Data: ";
    char *date = strcat(dataHeader, buf);
    strcpy(response->Date, date);
}

/**
  * This function manages the well done response, setting the response
  * @Param: protocol, struct to set, path of the image to send, last modified field
  * @Return: void
  */
void page_default(char *protocol, char *method, http_response *response, char *path, char *last_modified)
{
    // Header field
    char protocol_allocate[DIM_SHORT];
    strcpy(protocol_allocate, protocol);
    char *header = strcat(protocol_allocate, " 200 OK\r");
    strcpy(response->Header, header);

    // Server field
    char server[] = "Server: AGA server\r";
    strcpy(response->Server, server);

    // Content-Type field
    char type[] = "Content-Type: text/html\r";
    strcpy(response->Content_Type, type);

    // Connection field
    char conn[] = "Connection: close\r";
    strcpy(response->Connection, conn);

    // Body message
    char *body = malloc(sizeof(char)*DIM_HTML);
    if (strcmp(method, "HEAD") == 0) {
        strcpy(response->Body_Response, "");
    }/* else {
        strcpy(body, header_html);
        char img_tag[DIM_PATH];
        sprintf(img_tag, "<img src= \"%s\" />\n</body>\n</html>\r", path);
        strcat(body, img_tag);
        strcpy(response->Body_Response, body);
    }*/
    else {
        strcpy(body, header_html);
        char img_tag[DIM_PATH];
        sprintf(img_tag, "<a href= \"%s\" > Download image </a>\n</body>\n</html>\r", path);
        strcat(body, img_tag);
        strcpy(response->Body_Response, body);
    }

    // Date field
    char buf[DIM_LONG];
    time_t now = time(0);
    struct tm tm = *gmtime(&now);
    strftime(buf, sizeof buf, "%a, %d %b %Y %H:%M:%S %Z", &tm);
    char dataHeader[10] = "Data: ";
    char *date = strcat(dataHeader, buf);
    strcpy(response->Date, date);

    // Content-Length field
    size_t len = strlen(body);
    char lenTxt[DIM_SHORT];
    char lenHeader[] = "Content-Length: ";
    char lenStr[8];
    snprintf(lenStr, sizeof lenStr, "%zu", len);
    strcat(lenTxt, lenHeader);
    strcat(lenTxt, lenStr);
    strcpy(response->Content_Length, lenTxt);

    // Last-modified field
    char lm[DIM_SHORT] = "Last-Modified: ";
    strcat(lm, last_modified);
    strcpy(response->Last_Modified, lm);
}

/**
 * This function parses the response struct and send the http response to socket
 * @Param socket id of client, response to parse
 * @Return: void
 */
void parsing_response(int sockd, http_response *response)
{
    char *effective_response = malloc(sizeof(char)*DIM_HTML);
    memset(effective_response, 0, sizeof(char)*DIM_HTML);

    strcat(effective_response, response->Header);
    strcat(effective_response ,"\r\n");
    strcat(effective_response, response->Date);
    strcat(effective_response ,"\r\n");
    strcat(effective_response, response->Server);
    strcat(effective_response ,"\r\n");
    strcat(effective_response, response->Last_Modified);
    strcat(effective_response ,"\r\n");
    strcat(effective_response, response->Content_Length);
    strcat(effective_response ,"\r\n");
    strcat(effective_response, response->Connection);
    strcat(effective_response ,"\r\n");
    strcat(effective_response, response->Content_Type);
    strcat(effective_response ,"\r\n");
    strcat(effective_response ,"\r\n");
    strcat(effective_response, response->Body_Response);

    writen(sockd, effective_response, strlen(effective_response) * sizeof(char));
    free(effective_response);
}