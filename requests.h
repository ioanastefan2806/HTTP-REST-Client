#ifndef _REQUESTS_
#define _REQUESTS_

// returneaza un string cu un GET request
char *compute_get_request(char *host, char *url, char *query_params,
							char **cookies, int cookies_count, char* token);

// returneaza un string cu un POST request
char *compute_post_request(char *host, char *url, char* content_type, char **body_data,
							int body_data_fields_count, char** cookies, int cookies_count, char* token);

// returneaza un string cu un DELETE request
char* compute_delete_request(char *host, char *url, char *query_params,
                            char **cookies, int cookies_count, char* token);


#endif
