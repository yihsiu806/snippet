#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>

int main(void)
{
    int ret = 0;
    int sockfd = 0, connfd = 0;
    int optval = 0;
    struct sockaddr_in addr = {0}, client_addr = {0};
    socklen_t addr_size = 0;

    sockfd = socket(AF_INET, SOCKET_STREAM, 0);
    if (sockfd < 0) {
        perror("sock");
        exit(EXIT_FAILURE);
    }

    optval = 1;
    ret = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
    if (ret < 0) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    
    addr.sin_family = AF_INET;
    addr.sin_port = htons(6666);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    // addr.sin_addr.s_addr = inet_addr("74.125.230.20");
    ret = bind(sockfd, (struct sockaddr *)&addr, sizeof(addr));
    if (ret < 0) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    ret = listen(sockfd, 5);
    if (ret < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    for (;;) {
        connfd = accept(sockfd, NULL, NULL);
        if (client_sock < 0) {
            perror("accept");
            continue;
        }

	char *message = "GET / HTTP/1.1";
	ret = send(connfd, message, strlen(message), 0);
	if (ret < 0) {
	    puts("Send failed");
	}

	recv(connfd, buf, sizeof(buf), 0);

        close(connfd);
    }

    
}

void get_ip_by_hostname(char *hostname)
{
    char *host
}
