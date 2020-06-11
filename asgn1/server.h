/**
 * server.h
 * 
 * Contains object and functio declarations for httpserver
 */
#ifndef server_h

#define server_h
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#include <stdbool.h>

//warn
#include <err.h>
#define PORT 8080 
#define BUFFER_SIZE 4096
#define BACKLOG 5

#define PUT 2
#define GET 1
#define HEAD 0
//#define response "

//isspace
#include <ctype.h>

//used for reading and processing http request
struct httpObject {
    int method;
    char filename[28];
    ssize_t content_length;
    bool valid;
    char buff[BUFFER_SIZE];
    char response[255];
};


int write_buffer(int file_size, int write_to);

//Debug function
void print(char *s);

void pint(int i);

void prints(char *s, char *a);

int request_type(char *req);

char* get_file_name(char req[]);

ssize_t get_file_size(char req[]);

void http_response(int client_fd, char * response_type, int filesize);

void error_response(int client_fd, int errnum);

int double_carrage_index(char* request);

void read_request(int client_fd, struct httpObject *message);

void process_request(struct httpObject *message);

void send_response(int client_fd, struct httpObject *message);

void create_http_response(struct httpObject *message, char * response_type, int filesize);

void create_error_response(struct httpObject *message, int errnum);

void valid_request(struct httpObject *message);

int acceptable_file_name(char *file_name);

void read_and_write(struct httpObject* message, int read_from, int write_to);
#endif
