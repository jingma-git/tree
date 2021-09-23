#include "csapp.h"

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

void *thread(void *vargp)
{
    Pthread_detach(pthread_self());
    int connfd = *((int *)vargp);
    free(vargp);
    echo(connfd);
    Close(connfd);
    return NULL;
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        fprintf(stderr, "Please input port number");
        return -1;
    }

    int listenfd;
    int *connfd;
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;
    char client_hostname[MAXLINE], client_port[MAXLINE];

    listenfd = Open_listenfd(argv[1]);
    pthread_t tid;
    while (1)
    {
        clientlen = sizeof(struct sockaddr_storage);
        connfd = malloc(sizeof(int));
        *connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
        Getnameinfo((SA *)&clientaddr, clientlen, client_hostname,
                    MAXLINE, client_port, MAXLINE, 0);
        printf("Connected to (%s, %s)\n", client_hostname, client_port);

        Pthread_create(&tid, NULL, thread, connfd);
    }
    return 0;
}