#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <stdio.h>
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.h"

char *compute_get_request(char *host, char *url, char *query_params,
                            char **cookies, int cookies_count, char* token)
{
    char *message = (char*)calloc(BUFLEN, sizeof(char));
    char *line = (char*)calloc(LINELEN, sizeof(char));

    // metoda, url, protocol si parametrii (daca exista)
    if (query_params != NULL) {
        sprintf(line, "GET %s/%s HTTP/1.1", url, query_params);
    } else {
        sprintf(line, "GET %s HTTP/1.1", url);
    }
    compute_message(message, line);

    // host-ul
    sprintf(line, "Host: %s", host);
    compute_message(message, line);

    // headere
    if (token != NULL) {
        sprintf(line, "Authorization: Bearer %s", token);
        compute_message(message, line);
    }
    sprintf(line, "Connection: keep-alive");
    compute_message(message, line);

    // cookies
    if (cookies != NULL) {
        for (int i = 0; i < cookies_count; i++) {
            char *cookie = (char*)calloc(LINELEN, sizeof(char));
            sprintf(cookie, "Cookie: %s", cookies[i]);
            compute_message(message, cookie);
            free(cookie);
        }
    }

    // linie goala
    compute_message(message, "");

    free(line);

    return message;
}

char *compute_post_request(char *host, char *url, char* content_type, char **body_data,
                            int body_data_fields_count, char **cookies, int cookies_count, char* token)
{
    char *message = (char*)calloc(BUFLEN, sizeof(char));
    char *line = (char*)calloc(LINELEN, sizeof(char));
    char *body_data_buffer = (char*)calloc(LINELEN, sizeof(char));

    // metoda, url si protocol
    sprintf(line, "POST %s HTTP/1.1", url);
    compute_message(message, line);
    
    // host-ul
    sprintf(line, "Host: %s", host);
    compute_message(message, line);

    // tipul continutului
    sprintf(line, "Content-Type: %s", content_type);
    compute_message(message, line);

    // lungimea continutului
    int total_len = 0;
    for (int i = 0; i < body_data_fields_count; i++) {
        total_len += strlen(body_data[i]);
    }
    sprintf(line, "Content-Length: %d", total_len);
    compute_message(message, line);

    // token
    if (token != NULL) {
        sprintf(line, "Authorization: Bearer %s", token);
        compute_message(message, line);
    }

    // cookies
    if (cookies != NULL) {
        for (int i = 0; i < cookies_count; i++) {
            char *cookie = (char*)calloc(LINELEN, sizeof(char));
            sprintf(cookie, "Cookie: %s", cookies[i]);
            compute_message(message, cookie);
            free(cookie);
        }
    }

    // linie goala
    compute_message(message, "");

    // datele
    for (int i = 0; i < body_data_fields_count; i++) {
        strcat(body_data_buffer, body_data[i]);
    }
    compute_message(message, body_data_buffer);

    free(line);
    free(body_data_buffer);

    return message;
}

char* compute_delete_request(char *host, char *url, char *query_params,
                            char **cookies, int cookies_count, char* token)
{
    // alocare memorie
    char *message = (char*)calloc(BUFLEN, sizeof(char));
    char *line = (char*)calloc(LINELEN, sizeof(char));
    
    // metoda, url si protocol
    if (query_params != NULL) {
        sprintf(line, "DELETE %s/%s HTTP/1.1", url, query_params);
    } else {
        sprintf(line, "DELETE %s HTTP/1.1", url);
    }
    compute_message(message, line);
    
    // host-ul
    sprintf(line, "Host: %s", host);
    compute_message(message, line);

    // token
    if (token != NULL) {
        sprintf(line, "Authorization: Bearer %s", token);
        compute_message(message, line);
    }

    // cookies
    if (cookies != NULL) {
        for (int i = 0; i < cookies_count; i++) {
            char *cookie = (char*)calloc(LINELEN, sizeof(char));
            sprintf(cookie, "Cookie: %s", cookies[i]);
            compute_message(message, cookie);
            free(cookie);
        }
    }

    // linie goala
    compute_message(message, "");

    free(line);
    
    return message;
}
