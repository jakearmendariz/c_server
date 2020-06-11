#include "server.h"

/**
 * reads request
 * 
 * Handles all bad request possibilities, calling on the other functions in this file to do so
 */
void read_request(int client_fd, struct httpObject *message){
    message->valid = true;
    
    ssize_t bytes = recv(client_fd, message->buff, BUFFER_SIZE, 0);
    if(bytes < 0){ warn("recv"); message->valid = false; return; }
    message->buff[bytes] = 0; // null terminate
    printf("[+] received %ld bytes from client\n[+] response: ", bytes);

    //null terminate
    message->buff[bytes] = '\0';

    prints("request:", message->buff);
    return;
}

/**
 * validates the request, if theres a problem, its a 400 error
 * 
 * uses stat to check file for get, put and head. Creates the response header
 * Does not reading or writing to files
 */
void process_request(struct httpObject *message){

    valid_request(message);
    
    if(message->valid == false){
        create_http_response(message, "400 BAD REQUEST", 0);
        return; }
    
    struct stat st;         //to get the info on a file
    
    //switch for GET, PUT, HEAD (constant values)
    switch(message->method)
    {
        case HEAD:
        case GET:
            //https://www.includehelp.com/c-programs/find-size-of-file.aspx to find the size of a file
            if(stat(message->filename,&st)==0) {message->content_length = (st.st_size);}
            else { warn("stat caught"); create_error_response(message, errno); break;}

            //send valid header
            create_http_response(message, "200 OK", message->content_length);
            break;
        case PUT:
            //will be overwritten in the case of errors
            if(message->content_length == -1){
                message->valid = false;
                warn("Content length was not found in put request");
                create_http_response(message, "400 BAD REQUEST", 0);
            }
            create_http_response(message, "201 CREATED", 0);

            if(stat(message->filename,&st) != 0){
                if(errno == 13){
                    create_error_response(message, errno);
                }
            }
            break;
    }
}

/**
 * send_response to client_fd
 * 
 * if get, it will read and send the file contenets as well
 * 
 * if head it will send message
 * 
 * if put, it will write to a file, then send the message
 */
void send_response(int client_fd, struct httpObject *message){
    if(!message->valid){
        send(client_fd, message->response, strlen(message->response), 0);
        return;
    }
    int file;               //File descriptor
    //switch for GET, PUT, HEAD (constant values)
    switch(message->method)
    {
        case HEAD:
            send(client_fd, message->response, strlen(message->response), 0);
            break;
        case GET:
            file = open(message->filename, O_RDONLY);
            if(file < 0){ create_error_response(message, errno); break; }

            send(client_fd, message->response, strlen(message->response), 0);
            
            //while any space is left in file continue to read
            read_and_write(message, file, client_fd);
            close(file);
            break;
        case PUT:
            file = open(message->filename,  O_CREAT | O_WRONLY | O_TRUNC, 0644);
            if(file < 0) { warn("put open error");create_error_response(message, errno); break; }

            //Writes data to server
            read_and_write(message, client_fd, file);
            send(client_fd, message->response, strlen(message->response), 0);
            close(file);
            break;
    }
}

//Creates the http response!
void create_http_response(struct httpObject *message, char* response_type, int filesize){
    sprintf(message->response, "HTTP/1.1 %s\r\nContent-Length: %d\r\n\r\n", response_type, filesize);
    
}


//Hands in the client file descriptor and error type
void create_error_response(struct httpObject *message, int errnum){
    message->valid = false;
    if(errnum == 2){
        create_http_response(message, "404 NOT FOUND", 0);
    }else if(errnum == 13){
        create_http_response(message, "403 FORBIDDEN", 0);
    }else{
        create_http_response(message, "500 INTERNAL SERVER ERROR", 0);
    }
    return;
}
