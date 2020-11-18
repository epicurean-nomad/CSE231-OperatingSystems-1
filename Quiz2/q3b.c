#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <unistd.h>

const char* filename = "para2.txt";

void check_error(int return_val, char *str) {
    if(return_val < 0) {
        perror(str);
        exit(return_val);
    }
}

int main() {
    int sd, tmp, len;
    struct sockaddr_in myaddr, server;
    char s_buff[1024];
    FILE *fd;

    // Create a socket
    sd = socket(PF_INET, SOCK_DGRAM, 0);
    check_error(sd, "socket");

    // Initialize myaddr
    myaddr.sin_family = PF_INET;
    myaddr.sin_port = htons(60);
    myaddr.sin_addr.s_addr = INADDR_ANY;
    bzero(myaddr.sin_zero, 8);

    // Initialize server
    server.sin_family = PF_INET;
    server.sin_port = htons(80);
    server.sin_addr.s_addr = INADDR_ANY;
    bzero(server.sin_zero, 8);

    // Bind to an address
    tmp = bind(sd, (struct sockaddr *)&myaddr, sizeof(myaddr));
    check_error(tmp, "bind");

    len = sizeof(server);
    connect(sd, (struct sockaddr *)&server, len);

    fd = fopen(filename, "r");
    if(fd == NULL) {
        perror("fopen");
        exit(-1);
    }

    while(fscanf(fd, "%s", s_buff) > 0) {        
        // Send data from client
        tmp = send(sd, (void *)s_buff, sizeof(s_buff), 0);
        check_error(tmp, "send");
    }
    close(sd);
}