#include <stdio.h>      /* printf, sprintf */
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.h"
#include "parson.h"

int main(int argc, char *argv[])
{
    char *message;
    char *response;
    int sockfd;

    /*
    *   
    *   Pas 1: Se deschide conexiunea (open_connection)
    *   Pas 2: Se creaza mesajul de request (compute_get_request)
    *   Pas 3: Se trimite la server mesajul (send_to_server)
    *   Pas 4: Se primeste raspuns de la server (receive_from_server)
    *   Pas 5: Se inchide conexiunea cu serverul (close_connection)
    */

   /*****************   TASK 1      ***********/

    sockfd = open_connection("185.118.200.35", 8081, AF_INET, SOCK_STREAM, 0);
    message = compute_get_request("185.118.200.35", "/task1/start", NULL, NULL, NULL);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    printf("%s\n", response);
    close_connection(sockfd);

    /* se prelucreaza raspunsul de la server */

    /* se consturieste string-ul pentru cookies */
    char *cookie1 = strchr(response, ':');
    memmove(cookie1, cookie1 + 1, strlen(cookie1) - 1);
    cookie1 = strchr(response, ':');
    memmove(cookie1, cookie1 + 2, strlen(cookie1) - 1);
    char aux[BUFLEN];
    strcpy(aux, cookie1);
    char *token1 = strtok(aux, ";");
    char c1[BUFLEN];
    strcpy(c1, token1);
    memset(aux, 0, BUFLEN);
    char *cookie2 = strchr(cookie1, ':');
    memmove(cookie2, cookie2 + 4, strlen(cookie2) - 1);
    cookie2 = strchr(cookie1, ':');
    memmove(cookie2, cookie2 + 2, strlen(cookie2) - 1);
    strcpy(aux, cookie2);
    char *token2 = strtok(aux, ";");
    char c2[BUFLEN];
    strcpy(c2, token2);
    char compute_cookies[100000];
    sprintf(compute_cookies, "%s; %s", c1, c2);

    /* se selecteaza din mesaj doar textul json {...} */
    char *json = strchr(response, '{');
    const JSON_Value *val1 = json_parse_string(json);
    const JSON_Object *obj1 = json_value_get_object(val1);

    // URL
    const char *u = "url";
    const char *url = json_object_dotget_string(obj1, u);

    // METODA
    const char *mth = "method";
    const char *method = json_object_dotget_string(obj1, mth);

    // TIP
    const char *tp = "type";
    const char *type = json_object_dotget_string(obj1, tp);

    /* se muta cu o pozitie dupa { textului json pentru a putea cauta urmatorul text json (incorporat in primul) */
    memmove(json, json + 1, strlen(json));
    json = strchr(json, '{');
    const JSON_Value *val2 = json_parse_string(json);
    const JSON_Object *obj2 = json_value_get_object(val2);

    // USERNAME
    const char *user = "username";
    const char *username = json_object_dotget_string(obj2, user);

    //PASSWORD
    const char *pass = "password";
    const char *password = json_object_dotget_string(obj2, pass);

    /* se construieste mesajul pentru a fi trimis catre server */
    char buffer[BUFLEN];
    memset(buffer, 0, BUFLEN);
    strcpy(buffer, "username=");
    strcat(buffer, username);
    strcat(buffer, "&password=");
    strcat(buffer, password);
    char namepass[BUFLEN];
    strcpy(namepass, buffer);

    /*****************   TASK 2      ***********/
    sockfd = open_connection("185.118.200.35", 8081, AF_INET, SOCK_STREAM, 0);

    if(strcmp(method, "POST") == 0)
        message = compute_post_request("185.118.200.35",(char*) url, namepass, compute_cookies, NULL,(char*) type);
    
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);

    printf("%s\n", response);
    close_connection(sockfd);

    /* se prelucreaza raspunsul de la server */

    /* se consturieste string-ul pentru cookies */
    cookie1 = strchr(response, ':');
    memmove(cookie1, cookie1 + 1, strlen(cookie1) - 1);
    cookie1 = strchr(response, ':');
    memmove(cookie1, cookie1 + 2, strlen(cookie1) - 1);
    strcpy(aux, cookie1);
    token1 = strtok(aux, ";");
    char co1[BUFLEN];
    strcpy(co1, token1);
    memset(aux, 0, BUFLEN);
    cookie2 = strchr(cookie1, ':');
    memmove(cookie2, cookie2 + 4, strlen(cookie2) - 1);
    cookie2 = strchr(cookie1, ':');
    memmove(cookie2, cookie2 + 2, strlen(cookie2) - 1);
    strcpy(aux, cookie2);
    token2 = strtok(aux, ";");
    char co2[BUFLEN];
    strcpy(co2, token2);
    sprintf(compute_cookies, "%s; %s", co1, co2);

    /* se selecteaza din mesaj doar textul json {...} */
    json = strchr(response, '{');
    const JSON_Value *val3 = json_parse_string(json);
    const JSON_Object *obj3 = json_value_get_object(val3);

    //URL
    const char *u2 = "url";
    const char *url2 = json_object_dotget_string(obj3, u2);

    //METODA
    const char *mth2 = "method";
    const char *method2 = json_object_dotget_string(obj3, mth2);
    memmove(json, json + 1, strlen(json));

    /* se muta cu o pozitie dupa { textului json pentru a putea cauta urmatorul text json (incorporat in primul) */
    json = strchr(response, '{');
    const JSON_Value *val4 = json_parse_string(json);
    const JSON_Object *obj4 = json_value_get_object(val4);

    //TOKEN 
    const char *tkn = "token";
    const char *tok = json_object_dotget_string(obj4, tkn);
    memmove(json, json + 1, strlen(json));

    /* se muta cu o pozitie dupa { textului json pentru a putea cauta urmatorul text json (incorporat in al doilea) */
    json = strchr(response, '{');
    const JSON_Value *val5 = json_parse_string(json);
    const JSON_Object *obj5 = json_value_get_object(val5);

    //ID
    const char *d = "id";
    const char *id = json_object_dotget_string(obj5, d);

    /* se construieste mesajul pentru a fi trimis catre server */
    char buffer_task3[BUFLEN];
    sprintf(buffer_task3, "raspuns1=Omul&raspuns2=Numele&id=%s", id);

    /*****************   TASK 3      ***********/
    sockfd = open_connection("185.118.200.35", 8081, AF_INET, SOCK_STREAM, 0);

    if(strcmp(method2, "GET") == 0)
        message = compute_get_request("185.118.200.35", (char *)url2, buffer_task3, compute_cookies,(char*) tok);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);

    printf("%s\n", response);
    close_connection(sockfd);

    /* se prelucreaza raspunsul de la server */

    /* se consturieste string-ul pentru cookies */
    cookie1 = strchr(response, ':');
    memmove(cookie1, cookie1 + 1, strlen(cookie1) - 1);
    cookie1 = strchr(response, ':');
    memmove(cookie1, cookie1 + 2, strlen(cookie1) - 1);
    strcpy(aux, cookie1);
    token1 = strtok(aux, ";");
    strcpy(co1, token1);
    memset(aux, 0, BUFLEN);
    cookie2 = strchr(cookie1, ':');
    memmove(cookie2, cookie2 + 4, strlen(cookie2) - 1);
    cookie2 = strchr(cookie1, ':');
    memmove(cookie2, cookie2 + 2, strlen(cookie2) - 1);
    strcpy(aux, cookie2);
    token2 = strtok(aux, ";");
    strcpy(co2, token2);
    sprintf(compute_cookies, "%s; %s", co1, co2);

    /* se selecteaza din mesaj doar textul json {...} */
    json = strchr(response, '{');
    const JSON_Value *val6 = json_parse_string(json);
    const JSON_Object *obj6 = json_value_get_object(val6);

    //URL
    const char *u3 = "url";
    const char *url3 = json_object_dotget_string(obj6, u3);

    //METODA
    const char *mth3 = "method";
    const char *method3 = json_object_dotget_string(obj6, mth3);

    /*****************   TASK 4      ***********/
    sockfd = open_connection("185.118.200.35", 8081, AF_INET, SOCK_STREAM, 0);
    if(strcmp(method3, "GET") == 0)
        message = compute_get_request("185.118.200.35", (char *)url3, NULL, compute_cookies,(char*) tok);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);

    printf("%s\n", response);
    close_connection(sockfd);

    /* se prelucreaza raspunsul de la server */

    /* se consturieste string-ul pentru cookies */
    cookie1 = strchr(response, ':');
    memmove(cookie1, cookie1 + 1, strlen(cookie1) - 1);
    cookie1 = strchr(response, ':');
    memmove(cookie1, cookie1 + 2, strlen(cookie1) - 1);
    strcpy(aux, cookie1);
    token1 = strtok(aux, ";");
    strcpy(co1, token1);
    memset(aux, 0, BUFLEN);
    cookie2 = strchr(cookie1, ':');
    memmove(cookie2, cookie2 + 4, strlen(cookie2) - 1);
    cookie2 = strchr(cookie1, ':');
    memmove(cookie2, cookie2 + 2, strlen(cookie2) - 1);
    strcpy(aux, cookie2);
    token2 = strtok(aux, ";");
    strcpy(co2, token2);
    sprintf(compute_cookies, "%s; %s", co1, co2);

    /* se selecteaza din mesaj doar textul json {...} */
    json = strchr(response, '{');
    const JSON_Value *val7 = json_parse_string(json);
    const JSON_Object *obj7 = json_value_get_object(val7);

    //URL pt mesajul POST
    const char *u4 = "url";
    const char *url_post = json_object_dotget_string(obj7, u4);

    //METODA
    const char *mth4 = "method";
    const char *method4 = json_object_dotget_string(obj7, mth4);

    //TIP
    const char *typo = "type";
    const char *type_task5 = json_object_dotget_string(obj7, typo);
    memmove(json, json + 1, strlen(json));

    /* se muta cu o pozitie dupa { textului json pentru a putea cauta urmatorul text json (incorporat in primul) */
    json = strchr(response, '{');
    const JSON_Value *val8 = json_parse_string(json);
    const JSON_Object *obj8 = json_value_get_object(val8);

    //URL pt mesajul GET
    const char *u5 = "url";
    const char *url_get = json_object_dotget_string(obj8, u5);
    memmove(json, json + 1, strlen(json));

    /* se muta cu o pozitie dupa { textului json pentru a putea cauta urmatorul text json (incorporat in al doilea) */
    json = strchr(response, '{');
    const JSON_Value *val9 = json_parse_string(json);
    const JSON_Object *obj9 = json_value_get_object(val9);

    //Q
    const char *qq = "q";
    const char *q = json_object_dotget_string(obj9, qq);

    //APPID
    const char *apid = "APPID";
    const char *APPID = json_object_dotget_string(obj9, apid);


    /* se imparte url-ul pentru a putea afla adresa ip */
    char auxi[BUFLEN], auxi2[BUFLEN];
    strcpy(auxi, url_get);
    strcpy(auxi2, url_get);
    char* todns = strtok(auxi, "/");
    char *rest_url = strchr(auxi2, '/');
    struct addrinfo hints, *result;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    
    /*se afla adresa ip */
    getaddrinfo(todns, "http", &hints, &result);

    /* se construieste mesajul pt a fi trimis la server */
    char parse[BUFLEN];
    sprintf(parse, "%s?q=%s&APPID=%s", rest_url, q, APPID);
    char str[BUFLEN];
    struct sockaddr_in *addr = (struct sockaddr_in *) result->ai_addr;
    inet_ntop(AF_INET, &addr->sin_addr, str, sizeof(str));

    /* se afla portul pe care se va trimite mesajul */
    int port = ntohs(addr->sin_port);

    /*****************   TASK 5      ***********/
    sockfd = open_connection(str, port, AF_INET, SOCK_STREAM, 0);
    message = compute_get_request(str, (char *)parse, NULL, compute_cookies,(char*) tok);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    printf("%s\n", response);
    close_connection(sockfd);
    freeaddrinfo(result);

    /* se delimiteaza mesajul pt a ajunge la textul json care trebuie trimis inapoi catre server */
    char *r = strchr(response, '{');
    sockfd = open_connection("185.118.200.35", 8081, AF_INET, SOCK_STREAM, 0);
    if(strcmp(method4, "POST") == 0)
        message = compute_post_request("185.118.200.35",(char*) url_post, r, compute_cookies, (char*)tok, (char*)type_task5);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);

    printf("%s\n", response);
    close_connection(sockfd);
    free(message);
    return 0;
}
