#include "server.h"

//So I can place these all over my code, when debugging
int print_all = 1;

void print(char *s){
    if (print_all != 0) {
        printf("%s\n",s);
    }
}
void printint(char *s, int a){
    if (print_all != 0) {
        printf("%s%i\n",s,a);
    }
}
void prints(char *s, char *a){
    if (print_all != 0) {
        printf("%s%s\n",s,a);
    }
}
void pint(int i){
    if (print_all != 0) {
        printf("%d\n",i);
    }
}

//type and name
void type_and_name(struct httpObject *message, char* req){
    char* type = malloc(5);
    char* filename = malloc(255);
    char* httptype = malloc(10);

    //divides up the first string
    if(sscanf(req, "%s %s %s", type, filename, httptype) != 3){
        message->valid = false;
    }

    //classify method
    if(strcmp(type, "GET") == 0){
        message->method = GET;
    }else if(strcmp(type, "HEAD") == 0){
        message->method = HEAD;
    }else if(strcmp(type, "PUT") == 0){
        message->method = PUT;
    }else{
        message->valid = false;
    }

    //filename
    if(filename[0] != '/')
        message->valid = false;
    
    filename += 1;
    if(acceptable_file_name(filename) == 0){
        strcpy(message->filename, filename);
    }else{
        message->valid = false;
    }
    filename -= 1;

    //Check http version
    if(strcmp(httptype, "HTTP/1.1") != 0){
        message->valid = false;
    }

    free(type);
    free(filename);
    free(httptype);
}


//Checks string, returns 0 if good. 
int acceptable_file_name(char *file_name){
    for(int i = 0; i < (int)strlen(file_name); i++){
        char letter = file_name[i];
        if((letter >= 'A' && letter <= 'Z') || (letter >= 'a' && letter <= 'z') || (letter >= '0' && letter <= '9') || letter == '_' || letter == '-'){
            continue;
        }else{
            return letter;
        }
    }
    return 0;
}

//Collaborated with Eddie Chan and Rishika Patel on strategy of this function
int double_carrage_index(char* request){
    for(int i = 0; request[i] != '\0'; i++){
        if(request[i] == '\r' && request[i+1] == '\n' && request[i+2] == '\r' && request[i+3] == '\n'){
            return i+4;
        }
    }
    return -1;
}

//https://www.tutorialspoint.com/c_standard_library/c_function_strtok.htm for tokenizing strings
//https://stackoverflow.com/questions/7887003/c-sscanf-not-working for [^:] (only that)
void valid_request(struct httpObject *message){
    char *token;
    token = strtok(message->buff, "\r\n");
    //classifies GET, HEAD or PUT and gets the filename and verifies HTTP/1.1
    type_and_name(message, token);

    //each part of the http request is a key: value pair
    char key[64];
    char value[64];

    if(message->method == -1 || strlen(message->filename) == 0){
        message->valid = false;
        return;
    }
    message->content_length = -1;
    token = strtok(NULL, "\r\n");
    while( token != NULL ) {
        if(sscanf(token, "%[^:]: %s", key, value) != 2){
            printf("Token:%s\n", token);
            warn("Request not match format of: string:string");
            message->valid = false;
            return;
        }
        if(strstr(key, "Content-Length")!= NULL){
            message->content_length = atoi(value);
        }
        token =  strtok(NULL, "\r\n");
    }
    return;
}


//read_write
void read_and_write(struct httpObject* message, int read_from, int write_to){
    int amount;
    while(message->content_length > 0){
        amount = read(read_from, message->buff, BUFFER_SIZE);
        if(amount < 0){
            warn("invalid read");
            create_error_response(message, errno);
            break;
        }else{
            message->content_length -= amount;
            if(write(write_to, message->buff, amount) < 0){
                warn("writing to file error");
                create_error_response(message, errno);
                break;
            }
        }
    }
}
