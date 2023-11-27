#include "main.h"
#include <string.h>
#include <sys/socket.h>

int main(int argc, char **argv){
    if (argc < 2){
        printf("Usage: %s <port>\n", argv[0]);
        return 1;
    }
    
    int port = atoi(argv[1]);
    
    char *resp = "HTTP/1.0 200 OK\r\n"
                 "Content-Type: text/text\r\n"
                 "\r\n";

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1){
        perror("socket");
        return 1;
    }
    printf("socket connected successfully\n");
    
    struct sockaddr_in host_addr;
    int host_addr_len = sizeof(host_addr);

    host_addr.sin_family = AF_INET;
    host_addr.sin_port = htons(port);
    host_addr.sin_addr.s_addr = htons(INADDR_ANY);
   

    struct sockaddr_in client_addr;
    int client_addr_len = sizeof(client_addr);


    if (bind(sockfd, (struct sockaddr *)&host_addr, host_addr_len) != 0 ){
        perror("webserver (bind)");
        return 2;
    }
    printf("socket successfully bound to address\n");

    if (listen(sockfd, SOMAXCONN) != 0){
        perror("webserver (listen)");
        return 3;
    }
    printf("socket listening on port %d\n", port);
    
    for(;;){
        int new_sockfd = accept(sockfd, (struct sockaddr *)&host_addr, (socklen_t *)&host_addr_len);
        if (new_sockfd == -1){
            perror("webserver (accept)");
            continue;
        }
        printf("socket accepted connection\n");
        
        int sockn = getsockname(new_sockfd, (struct sockaddr *)&client_addr, (socklen_t *)&client_addr_len);
        if (sockn == -1){
            perror("webserver (getsockname)");
            continue;
        }

        char buffer[BUFFER_SIZE*3 + 4];

        int valread = read(new_sockfd, buffer, BUFFER_SIZE);
        if (valread < 0){
            perror("webserver (read)");
            continue;
        }
        char method[BUFFER_SIZE], uri[BUFFER_SIZE], version[BUFFER_SIZE];

        sscanf(buffer, "%s %s %s", method, uri, version);
        printf("[%s:%u] %s %s %s\n", inet_ntoa(client_addr.sin_addr),
               ntohs(client_addr.sin_port), method, version, uri);

        char *base = strtok(uri, "/");
        
        int valwrite = write(new_sockfd, resp, strlen(resp));
        if (valwrite < 0){
            perror("webserver (write)");
            continue;
        }
        if (strcmp(base, "health") == 0){
            char *resp = "HTTP/1.0 200 OK\r\n"
                         "Content-Type: text/text\r\n"
                         "\r\n"
                         "OK";
            valwrite = write(new_sockfd, resp, strlen(resp));
        }else{
            char *resp = "HTTP/1.0 404 NOT FOUND\r\n"
                         "Content-Type: text/text\r\n"
                         "\r\n"
                         "NOT FOUND";
            valwrite = write(new_sockfd, resp, strlen(resp));
        }

        valwrite = write(new_sockfd, "\r\n", 2);
        close(new_sockfd);
    }

    return 0;
}
