#include "csapp.h"

int main(int argc, char **argv)
{
    int clientfd;
    char *host, *port, buf[MAXLINE];
    rio_t rio;

    // host = argv[1];
    // port = argv[2];

    // clientfd = Open_clientfd(host, port);
    clientfd = Open_clientfd("127.0.0.1", "8080");
    rio_readinitb(&rio, clientfd);

    while (Fgets(buf, MAXLINE, stdin) != NULL) // terminal read
    {
        // write buf to fd
        rio_writen(clientfd, buf, strlen(buf)); // write msg to socket: send
        rio_readlineb(&rio, buf, MAXLINE);      // read msg from socket: recv
        Fputs(buf, stdout);                     // terminal write
    }
    Close(clientfd);
    return 0;
}