#include "server.h"
//Jake Armendariz
//Main code for the server

int main(int argc, char **argv) {
    if(argc != 2){
        char *msg = "Needs 1 argument\nto run  !./httpserver [port]\n\0";
        write(1,msg, strlen(msg));
        return -1;
    }
    //Creates sockaddr with informtion
    int port_addr = atoi(argv[1]);
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port_addr);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    socklen_t addrlen = sizeof(server_addr);

    //Create server socket
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    // Need to check if server_fd < 0, meaning an error
    if (server_fd < 0) { warn("socket");}

    //used to configure the server socket
    int enable = 1;

    //binds the address, handles "already in use"
    int ret = setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable));
    if(ret != 0){ warn("set sockopt error");}
    
    //Binds the socket to the address
    ret = bind(server_fd, (struct sockaddr *) &server_addr, addrlen);
    if(ret != 0){ warn("bind error"); }
    
    //marks the socket referred to buy socketfd as a passive socket, will be used by accept to get requests
    ret = listen(server_fd, 10); // 10 should be enough, if not use SOMAXCONN
    if (ret != 0) { warn("listen error"); return -1; }

    //Contains the contents of the message
    struct httpObject *message;
    while(true){
        struct sockaddr client_addr;
        socklen_t client_addrlen = sizeof(client_addr);
        printf("[+] server is waiting...\n");

        int client_fd = accept(server_fd, &client_addr, &client_addrlen);
        if(client_fd < 0){ warn("accept error");  continue; }

        message = malloc(sizeof(struct httpObject));

        read_request(client_fd, message);

        process_request(message);

        send_response(client_fd, message);

        free(message);
        close(client_fd);
    }

    close(server_fd);
    
    return 0;
}
