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
#include "json.hpp"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int find_status_code(char* response)
{
    char* status_code = strstr(response, "HTTP/1.1 ");
    status_code += 9;
    return atoi(status_code);
}

bool validate_response(char* response)
{
    int status_code = find_status_code(response);
    if (status_code / 100 == 2)
    {
        return true;
    }
    return false;
}

int main(int argc, char *argv[])
{
    char host[] = "34.246.184.49";
    char urls[10][50] = {
        "/api/v1/tema/auth/register",
        "/api/v1/tema/auth/login",
        "/api/v1/tema/library/access",
        "/api/v1/tema/library/books",
        "/api/v1/tema/library/books",
        "/api/v1/tema/library/books",
        "/api/v1/tema/library/books",
        "/api/v1/tema/auth/logout"
    };

    char content_type[] = "application/json";
    char* cookie = NULL;
    char* web_token = NULL;

    char* cookies[10];
    char* body_data[10];

    char *command = (char*)calloc(LINELEN, sizeof(char));

    // asteptam input de la tastatura
    while  (1)
    {
        // citim comanda
        cin.getline(command, LINELEN);

        // daca comanda este exit, iesim din program
        if (strcmp(command, "exit") == 0)
        {
            if (cookie != NULL)
            {
                free(cookie);
            }
            if (web_token != NULL)
            {
                free(web_token);
            }
            web_token = NULL;
            cookie = NULL;
            break;
        }

        // trebuie sa nu fiu logat pentru a putea sa ma inregistrez sau sa ma loghez
        if ((strcmp(command, "register") == 0 || strcmp(command, "login") == 0) && cookie != NULL)
        {
            cout << "EROARE: User logat" << endl;
            continue;
        }

        // trebuie sa fiu logat pentru a putea sa ma deloghez
        if (strcmp(command, "logout") == 0 && cookie == NULL)
        {
            cout << "EROARE: User neautentificat" << endl;
            continue;
        }

        // trebuie sa fiu logat pentru a putea primi acces la biblioteca
        if (strcmp(command, "enter_library") == 0 && cookie == NULL)
        {
            cout << "EROARE: User neautentificat" << endl;
            continue;
        }

        // trebuie sa nu am token pentru a putea obtine unul
        if (strcmp(command, "enter_library") == 0 && web_token != NULL && cookie != NULL)
        {
            cout << "EROARE: User deja in biblioteca" << endl;
            continue;
        }

        if (strcmp(command, "get_books") == 0
            || strcmp(command, "get_book") == 0
            || strcmp(command, "add_book") == 0
            || strcmp(command, "delete_book") == 0)
        {
            // trebuie sa fiu logat si sa am token pentru a putea accesa biblioteca
            if (cookie == NULL || web_token == NULL)
            {
                cout << "EROARE: User neautentificat" << endl;
                continue;
            }
        }

        if (strcmp(command, "register") == 0) {
            // daca comanda este register
            // citim username si parola
            char username[100];
            char password[100];

            cout << "username=";
            cin.getline(username, 50);
            username[strlen(username)] = '\0';

            cout << "password=";
            cin.getline(password, 50);
            password[strlen(password)] = '\0';

            // construim json-ul
            nlohmann::json reg_info;
            reg_info["username"] = username;
            reg_info["password"] = password;

            body_data[0] = strdup(reg_info.dump().c_str());

            // facem request-ul
            char* message = compute_post_request(host, urls[0], content_type, body_data, 1, NULL, 0, NULL);

            // deschidem conexiunea
            int sockfd = open_connection(host, 8080, AF_INET, SOCK_STREAM, 0);

            // trimitem request-ul
            send_to_server(sockfd, message);

            // primim raspunsul
            char* response = receive_from_server(sockfd);

            // inchidem conexiunea
            close_connection(sockfd);

            // verific daca operatia a fost cu succes
            if (validate_response(response))
            {
                cout << "SUCCES: Inregistrat" << endl;
            } else {
                cout << "EROARE: User existent" << endl;
            }

            // eliberam memoria
            free(message);
            free(response);
            free(body_data[0]);
        } else if (strcmp(command, "login") == 0) {
            // daca comanda este login
            // citim username si parola
            char username[100];
            char password[100];

            cout << "username=";
            cin.getline(username, 50);
            username[strlen(username)] = '\0';

            cout << "password=";
            cin.getline(password, 50);
            password[strlen(password)] = '\0';

            // construim json-ul
            nlohmann::json login_info;
            login_info["username"] = username;
            login_info["password"] = password;

            body_data[0] = strdup(login_info.dump().c_str());

            // facem request-ul
            char* message = compute_post_request(host, urls[1], content_type, body_data, 1, NULL, 0, NULL);

            // deschidem conexiunea
            int sockfd = open_connection(host, 8080, AF_INET, SOCK_STREAM, 0);

            // trimitem request-ul
            send_to_server(sockfd, message);

            // primim raspunsul
            char* response = receive_from_server(sockfd);

            // inchidem conexiunea
            close_connection(sockfd);

            // verific daca operatia a fost cu succes
            if (validate_response(response))
            {
                // extragem cookie-ul
                cookie = strdup(strstr(response, "Set-Cookie") + 12);
                cookie = strtok(cookie, ";");
                cookie[strlen(cookie)] = '\0';

                cookies[0] = cookie;

                cout << "SUCCES: Logat" << endl;
            } else {
                cout << "EROARE: User sau parola gresite" << endl;
            }

            // eliberam memoria
            free(message);
            free(response);
            free(body_data[0]);
        } else if (strcmp(command, "enter_library") == 0) {
            // daca comanda este enter_library
            // facem request-ul
            char* message = compute_get_request(host, urls[2], NULL, cookies, 1, NULL);

            // deschidem conexiunea
            int sockfd = open_connection(host, 8080, AF_INET, SOCK_STREAM, 0);

            // trimitem request-ul
            send_to_server(sockfd, message);

            // primim raspunsul
            char* response = receive_from_server(sockfd);

            // inchidem conexiunea
            close_connection(sockfd);

            // verific daca operatia a fost cu succes
            if (validate_response(response))
            {
                // extragem token-ul
                web_token = strdup(strstr(response, "token") + 8);
                web_token = strtok(web_token, "\"");
                web_token[strlen(web_token)] = '\0';

                cout << "SUCCES: Acces la biblioteca" << endl;
            } else {
                cout << "EROARE: Token Invalid" << endl;
            }

            // eliberam memoria
            free(message);
            free(response);
        } else if (strcmp(command, "get_books") == 0) {
            // daca comanda este get_books
            // facem request-ul
            char* message = compute_get_request(host, urls[3], NULL, cookies, 1, web_token);

            // deschidem conexiunea
            int sockfd = open_connection(host, 8080, AF_INET, SOCK_STREAM, 0);

            // trimitem request-ul
            send_to_server(sockfd, message);

            // primim raspunsul
            char* response = receive_from_server(sockfd);

            // inchidem conexiunea
            close_connection(sockfd);

            // verific daca operatia a fost cu succes
            if (validate_response(response))
            {
                cout << "SUCCES: Carti primite" << endl;

                // folosind nlohmann::json, extragem cartile
                nlohmann::json books = nlohmann::json::parse(basic_extract_json_response(response));
                cout << books.dump(4) << endl;
            } else {
                cout << "EROARE: Token Invalid" << endl;
            }

            // eliberam memoria
            free(message);
            free(response);
        } else if (strcmp(command, "get_book") == 0) {
            // daca comanda este get_book
            // citim id-ul cartii
            char id[100];

            cout << "id=";
            cin.getline(id, 100 );
            id[strlen(id)] = '\0';

            // verificam daca id-ul este valid
            if (atoi(id) == 0)
            {
                cout << "EROARE: Id invalid" << endl;
                continue;
            }

            // construim url-ul
            char url[55];
            strcpy(url, urls[4]);
            strcat(url, "/");
            strcat(url, id);

            // facem request-ul
            char* message = compute_get_request(host, url, NULL, cookies, 1, web_token);

            // deschidem conexiunea
            int sockfd = open_connection(host, 8080, AF_INET, SOCK_STREAM, 0);

            // trimitem request-ul
            send_to_server(sockfd, message);

            // primim raspunsul
            char* response = receive_from_server(sockfd);

            // inchidem conexiunea
            close_connection(sockfd);

            // verific daca operatia a fost cu succes
            if (validate_response(response))
            {
                cout << "SUCCES: Carte primita" << endl;

                // folosind nlohmann::json, extragem cartea
                nlohmann::json book = nlohmann::json::parse(basic_extract_json_response(response));
                cout << book.dump(4) << endl;
            } else {
                cout << "EROARE: Carte inexistenta" << endl;
            }

            // eliberam memoria
            free(message);
            free(response);
        } else if (strcmp(command, "add_book") == 0) {
            // daca comanda este add_book
            // citim titlul, autorul, genul, publisher-ul si numarul de pagini
            char title[100];
            char author[100];
            char genre[100];
            char publisher[100];
            char page_count[100];

            cout << "title=";
            cin.getline(title, 100);
            title[strlen(title)] = '\0';

            cout << "author=";
            cin.getline(author, 100);
            author[strlen(author)] = '\0';

            cout << "genre=";
            cin.getline(genre, 100);
            genre[strlen(genre)] = '\0';

            cout << "publisher=";
            cin.getline(publisher, 100);
            publisher[strlen(publisher)] = '\0';

            cout << "page_count=";
            cin.getline(page_count, 100);
            page_count[strlen(page_count)] = '\0';

            // verificam daca page_count este valid
            if (atoi(page_count) == 0)
            {
                cout << "EROARE: Numar de pagini invalid" << endl;
                continue;
            }

            // construim json-ul
            nlohmann::json book_info;
            book_info["title"] = title;
            book_info["author"] = author;
            book_info["genre"] = genre;
            book_info["publisher"] = publisher;
            book_info["page_count"] = atoi(page_count);

            body_data[0] = strdup(book_info.dump().c_str());

            // facem request-ul
            char* message = compute_post_request(host, urls[5], content_type, body_data, 1, cookies, 1, web_token);

            // deschidem conexiunea
            int sockfd = open_connection(host, 8080, AF_INET, SOCK_STREAM, 0);

            // trimitem request-ul
            send_to_server(sockfd, message);

            // primim raspunsul
            char* response = receive_from_server(sockfd);

            // inchidem conexiunea
            close_connection(sockfd);

            // verific daca operatia a fost cu succes
            if (validate_response(response))
            {
                cout << "SUCCES: Carte adaugata" << endl;
            } else {
                cout << "EROARE: Carte invalida" << endl;
            }

            // eliberam memoria
            free(message);
            free(response);
            free(body_data[0]);
        } else if (strcmp(command, "delete_book") == 0) {
            // daca comanda este delete_book
            // citim id-ul cartii
            char id[100];

            cout << "id=";
            cin.getline(id, 100);
            id[strlen(id)] = '\0';

            // verificam daca id-ul este valid
            if (atoi(id) == 0)
            {
                cout << "EROARE: Id invalid" << endl;
                continue;
            }

            // construim url-ul
            char url[50];
            strcpy(url, urls[6]);
            strcat(url, "/");
            strcat(url, id);

            // facem request-ul
            char* message = compute_delete_request(host, url, NULL, cookies, 1, web_token);

            // deschidem conexiunea
            int sockfd = open_connection(host, 8080, AF_INET, SOCK_STREAM, 0);

            // trimitem request-ul
            send_to_server(sockfd, message);

            // primim raspunsul
            char* response = receive_from_server(sockfd);

            // inchidem conexiunea
            close_connection(sockfd);

            // verific daca operatia a fost cu succes
            if (validate_response(response))
            {
                cout << "SUCCES: Carte stearsa" << endl;
            } else {
                cout << "EROARE: Carte inexistenta" << endl;
            }

            // eliberam memoria
            free(message);
            free(response);
        } else if (strcmp(command, "logout") == 0) {
            // daca comanda este logout
            // facem request-ul
            char* message = compute_get_request(host, urls[7], NULL, cookies, 1, web_token);

            // deschidem conexiunea
            int sockfd = open_connection(host, 8080, AF_INET, SOCK_STREAM, 0);

            // trimitem request-ul
            send_to_server(sockfd, message);

            // primim raspunsul
            char* response = receive_from_server(sockfd);

            // inchidem conexiunea
            close_connection(sockfd);

            // verific daca operatia a fost cu succes
            if (validate_response(response))
            {
                cout << "SUCCES: Delogat" << endl;
            } else {
                cout << "EROARE: User neautentificat" << endl;
            }

            // eliberam memoria
            free(message);
            free(response);

            // eliberam cookie-ul si token-ul
            free(cookie);
            free(web_token);
            cookie = NULL;
            web_token = NULL;
        } else {
            cout << "EROARE: Comanda invalida" << endl;
        }
    }
    
    free(command);

    return 0;
}
