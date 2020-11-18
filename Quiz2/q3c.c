#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>

void check_error(int return_val, char *str) {
    if(return_val < 0) {
        perror(str);
        exit(return_val);
    }
}

int main() {
    int sd, tmp, len;
    struct sockaddr_in myaddr, client_addr;
    char r_buff[1024];

    // Create a socket
    sd = socket(PF_INET, SOCK_DGRAM, 0);
    check_error(sd, "socket");

    // Initialize myaddr
    myaddr.sin_family = PF_INET;
    myaddr.sin_port = htons(80);
    myaddr.sin_addr.s_addr = INADDR_ANY;
    bzero(myaddr.sin_zero, 8);

    // Bind to an address
    tmp = bind(sd, (struct sockaddr *)&myaddr, sizeof(myaddr));
    check_error(tmp, "bind");

    printf("Listening for messages at port 80...\n\n");

    while(1) {
        // Receive data from client
        tmp = recv(sd, r_buff, sizeof(r_buff), 0);
        check_error(tmp, "recv");

        printf("msg from client: %s\n", r_buff);
    }
}