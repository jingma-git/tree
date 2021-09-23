#include "csapp.h"
// concurrent echo server based on process

// read and echo text lines until client closes connection
void echo(int connfd)
{
    size_t n;
    char buf[MAXLINE];
    rio_t rio;

    rio_readinitb(&rio, connfd);
    while ((n = rio_readlineb(&rio, buf, MAXLINE)) != 0)
    {
        printf("server received %d bytes\n", (int)n);
        rio_writen(connfd, buf, n); // write buf to fd
    }
}

void catch_child(int signo)
{
    while (waitpid(-1, NULL, WNOHANG) > 0) // reap all children
        continue;
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        fprintf(stderr, "Please input port number");
        return -1;
    }

    int listenfd, connfd;
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;
    char client_hostname[MAXLINE], client_port[MAXLINE];

    signal(SIGCHLD, catch_child);
    listenfd = Open_listenfd(argv[1]);
    while (1)
    {
        clientlen = sizeof(struct sockaddr_storage);
        connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
        Getnameinfo((SA *)&clientaddr, clientlen, client_hostname,
                    MAXLINE, client_port, MAXLINE, 0);
        printf("Connected to (%s, %s)\n", client_hostname, client_port);
        if (Fork() == 0) // child process serve client
        {
            close(listenfd); // Child close its listening descriptor
            echo(connfd);
            Close(connfd);
            exit(0);
        }
        Close(connfd); // Parent close its connection
    }
    return 0;
}