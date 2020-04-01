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

char *compute_get_request(char *host, char *url, char *url_params, char *cookies, char* token)
{
    char *message = calloc(BUFLEN, sizeof(char));
    char *line = calloc(BUFLEN, sizeof(char));
    /*
        PAS 1: Scriem numele metodei, calea, parametri din url (daca exista) si tipul protocolului
    */
    if (url_params != NULL)
    {
        sprintf(line, "GET %s?%s HTTP/1.1", url, url_params);
    }
    else
    {
        sprintf(line, "GET %s HTTP/1.1", url);
    }
    compute_message(message, line);

    /*
        PAS 2: Adaugam host-ul
    */
    sprintf(line, "Host: %s", host);
    compute_message(message, line);
    // TODO Ex 0

    /*
        PAS 3 (optional): Adaugam headere si/ sau cookies, respectand forma protocolului
    */
    // TODO BONUS COOKIE

    if(token != NULL){
        sprintf(line, "Authorization: Basic %s", token);
        compute_message(message, line);
    }

    if(cookies != NULL){
        sprintf(line, "Cookie: %s", cookies);
        compute_message(message, line);
    }
    
    /*
        PAS 4: Adaugam linia de final
    */
    strcat(message, "\r\n");
    free(line);
    //TODO Ex 0
    return message;
}
char *compute_post_request(char *host, char *url, char *form_data, char* cookies, char *token, char *type)
{

    char *message = calloc(BUFLEN, sizeof(char));
    char *line = calloc(BUFLEN, sizeof(char));
    /*
        PAS 1: Scriem numele metodei, calea si tipul protocolului
    */
    sprintf(line, "POST %s HTTP/1.1", url);
    compute_message(message, line);
    // TODO Ex 2

    /*
        PAS 2: Adaugam host-ul
    */
    sprintf(line, "Host: %s", host);
    compute_message(message, line);
    // TODO Ex 2

    /*
        PAS 3: Adaugam headere si/ sau cookies, respectand forma protocolului
        ATENTIE!! Headerele Content-Type si Content-Length sunt obligatorii
    */
    sprintf(line, "Content-Type: %s", type);
    compute_message(message, line);
    sprintf(line, "Content-Length: %ld", strlen(form_data));
    compute_message(message, line);

    if(token != NULL){
        sprintf(line, "Authorization: Basic %s", token);
        compute_message(message, line);
    }
    
    if(cookies != NULL){
        sprintf(line, "Cookie: %s", cookies);
        compute_message(message, line);
    }
    
    // TODO Ex 2

    /*
        PAS 4: Adaugam linia de final de antent
    */
    strcat(message, "\r\n");
    //
    // TODO Ex 2 

    /*
        PAS 5: Adaugam data
    */
    compute_message(message, form_data);
    free(line);
    // TODO Ex 2
    return message;
}
