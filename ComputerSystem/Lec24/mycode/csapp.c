#include "csapp.h"

void unix_error(char *msg) /* Unix-style error */
{
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    exit(0);
}
/* $end unixerror */

void posix_error(int code, char *msg) /* Posix-style error */
{
    fprintf(stderr, "%s: %s\n", msg, strerror(code));
    exit(0);
}

void gai_error(int code, char *msg) /* Getaddrinfo-style error */
{
    fprintf(stderr, "%s: %s\n", msg, gai_strerror(code));
    exit(0);
}

void app_error(char *msg) /* Application error */
{
    fprintf(stderr, "%s\n", msg);
    exit(0);
}
/* $end errorfuns */

void dns_error(char *msg) /* Obsolete gethostbyname error */
{
    fprintf(stderr, "%s\n", msg);
    exit(0);
}

/* 
 * rio_read - This is a wrapper for the Unix read() function that
 *    transfers min(n, rio_cnt) bytes from an internal buffer to a user
 *    buffer, where n is the number of bytes requested by the user and
 *    rio_cnt is the number of unread bytes in the internal buffer. On
 *    entry, rio_read() refills the internal buffer via a call to
 *    read() if the internal buffer is empty.
 */
static ssize_t rio_read(rio_t *rp, char *usrbuf, size_t n)
{
    int cnt;
    while (rp->rio_cnt <= 0) // refill if buf is empty
    {
        rp->rio_cnt = read(rp->rio_fd, rp->rio_buf, sizeof(rp->rio_buf));
        if (rp->rio_cnt < 0)
        {
            if (errno != EINTR) // interrupted by a signal handler
                return -1;
        }
        else if (rp->rio_cnt == 0) // EOF
        {
            return 0;
        }
        else
        {
            rp->rio_bufptr = rp->rio_buf; // reset buffer ptr
        }
    }

    // Copy min(n, rp->rio_cnt) bytes from internal buf to user buf
    cnt = n;
    if (rp->rio_cnt < n)
        cnt = rp->rio_cnt;
    memcpy(usrbuf, rp->rio_bufptr, cnt);
    rp->rio_bufptr += cnt;
    rp->rio_cnt -= cnt;
    return cnt;
}

ssize_t rio_writen(int fd, void *usrbuf, size_t n)
{
    size_t nleft = n;
    ssize_t nwritten;
    char *bufp = usrbuf;

    while (nleft > 0)
    {
        if ((nwritten = write(fd, bufp, nleft)) <= 0)
        {
            if (errno == EINTR) /* Interrupted by sig handler return */
                nwritten = 0;   /* and call write() again */
            else
                return -1; /* errno set by write() */
        }
        nleft -= nwritten;
        bufp += nwritten;
    }
    return n;
}

void rio_readinitb(rio_t *rp, int fd)
{
    rp->rio_fd = fd;
    rp->rio_cnt = 0;
    rp->rio_bufptr = rp->rio_buf;
}

// robustly read n bytes
ssize_t rio_readnb(rio_t *rp, void *usrbuf, size_t n)
{
    size_t nleft = n;
    ssize_t nread;
    char *bufp = usrbuf;

    while (nleft > 0)
    {
        if ((nread = rio_read(rp, bufp, nleft)) < 0)
            return -1;
        else if (nread = 0)
            break;
        nleft -= nread;
        bufp += nread;
    }

    return (n - nleft);
}

// robustly read a text line (buffered)
ssize_t rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen)
{
    int n, rc;
    char c, *bufp = usrbuf;

    for (n = 1; n < maxlen; n++)
    {
        if ((rc = rio_read(rp, &c, 1)) == 1)
        {
            *bufp++ = c;

            if (c == '\n')
            {
                n++;
                break;
            }
        }
        else if (rc == 0)
        {
            if (n == 1)
                return 0;
            else
                break;
        }
        else
        {
            return -1;
        }
    }
    *bufp = 0;
    return n - 1;
}

/******************************** 
 * Client/server helper functions
 ********************************/
/*
 * open_clientfd - Open connection to server at <hostname, port> and
 *     return a socket descriptor ready for reading and writing. This
 *     function is reentrant and protocol-independent.
 *
 *     On error, returns: 
 *       -2 for getaddrinfo error
 *       -1 with errno set for other errors.
 */
/* $begin open_clientfd */
int open_clientfd(char *hostname, char *port)
{
    int clientfd, rc;
    struct addrinfo hints, *listp, *p;

    // get a list of potential server address
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_socktype = SOCK_STREAM; // Open a connection
    hints.ai_flags = AI_NUMERICSERV; // using a numeric port
    hints.ai_flags |= AI_ADDRCONFIG; // Recommended for connections
    if ((rc = getaddrinfo(hostname, port, &hints, &listp)) != 0)
    {
        fprintf(stderr, "getaddrinfo failed (%s:%s): %s\n", hostname, port, gai_strerror(rc));
        return -2;
    }

    // walk the list for one that we can successfully connect to
    for (p = listp; p; p = p->ai_next)
    {
        // create a socket descriptor
        if ((clientfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0)
            continue; // create socket failed, try next one

        if (connect(clientfd, p->ai_addr, p->ai_addrlen) != -1)
            break; // success

        if (close(clientfd) < 0)
        { /* Connect failed, try another */ //line:netp:openclientfd:closefd
            fprintf(stderr, "open_clientfd: close failed: %s\n", strerror(errno));
            return -1;
        }
    }

    // cleanup
    freeaddrinfo(listp);
    if (!p) //All connects failed
        return -1;
    else
        return clientfd;
}

/*  
 * open_listenfd - Open and return a listening socket on port. This
 *     function is reentrant and protocol-independent.
 *
 *     On error, returns: 
 *       -2 for getaddrinfo error
 *       -1 with errno set for other errors.
 */

int open_listenfd(char *port)
{
    struct addrinfo hints, *listp, *p;
    int listenfd, rc, optval = 1;

    /* Get a list of potential server addresses */
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_socktype = SOCK_STREAM;             /* Accept connections */
    hints.ai_flags = AI_PASSIVE | AI_ADDRCONFIG; /* ... on any IP address */
    hints.ai_flags |= AI_NUMERICSERV;            /* ... using port number */
    if ((rc = getaddrinfo(NULL, port, &hints, &listp)) != 0)
    {
        fprintf(stderr, "getaddrinfo failed (port %s): %s\n", port, gai_strerror(rc));
        return -2;
    }

    /* Walk the list for one that we can bind to */
    for (p = listp; p; p = p->ai_next)
    {
        /* Create a socket descriptor */
        if ((listenfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0)
            continue; /* Socket failed, try the next */

        /* Eliminates "Address already in use" error from bind */
        setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, //line:netp:csapp:setsockopt
                   (const void *)&optval, sizeof(int));

        /* Bind the descriptor to the address */
        if (bind(listenfd, p->ai_addr, p->ai_addrlen) == 0)
            break; /* Success */
        if (close(listenfd) < 0)
        { /* Bind failed, try the next */
            fprintf(stderr, "open_listenfd close failed: %s\n", strerror(errno));
            return -1;
        }
    }

    /* Clean up */
    freeaddrinfo(listp);
    if (!p) /* No address worked */
        return -1;

    /* Make it a listening socket ready to accept connection requests */
    if (listen(listenfd, LISTENQ) < 0)
    {
        close(listenfd);
        return -1;
    }
    return listenfd;
}

int Open_clientfd(char *hostname, char *port)
{
    int rc;

    if ((rc = open_clientfd(hostname, port)) < 0)
        unix_error("Open_clientfd error");
    return rc;
}

int Open_listenfd(char *port)
{
    int rc;

    if ((rc = open_listenfd(port)) < 0)
        unix_error("Open_listenfd error");
    return rc;
}

int Accept(int s, struct sockaddr *addr, socklen_t *addrlen)
{
    int rc;

    if ((rc = accept(s, addr, addrlen)) < 0)
        unix_error("Accept error");
    return rc;
}

void Getnameinfo(const struct sockaddr *sa, socklen_t salen, char *host,
                 size_t hostlen, char *serv, size_t servlen, int flags)
{
    int rc;

    if ((rc = getnameinfo(sa, salen, host, hostlen, serv,
                          servlen, flags)) != 0)
        gai_error(rc, "Getnameinfo error");
}

void Close(int fd)
{
    int rc;

    if ((rc = close(fd)) < 0)
        unix_error("Close error");
}

char *Fgets(char *ptr, int n, FILE *stream)
{
    char *rptr;

    if (((rptr = fgets(ptr, n, stream)) == NULL) && ferror(stream))
        app_error("Fgets error");

    return rptr;
}

void Fputs(const char *ptr, FILE *stream)
{
    if (fputs(ptr, stream) == EOF)
        unix_error("Fputs error");
}

pid_t Fork(void)
{
    pid_t pid;

    if ((pid = fork()) < 0)
        unix_error("Fork error");
    return pid;
}

void Pthread_create(pthread_t *tidp, pthread_attr_t *attrp,
                    void *(*routine)(void *), void *argp)
{
    int rc;

    if ((rc = pthread_create(tidp, attrp, routine, argp)) != 0)
        posix_error(rc, "Pthread_create error");
}

void Pthread_detach(pthread_t tid)
{
    int rc;

    if ((rc = pthread_detach(tid)) != 0)
        posix_error(rc, "Pthread_detach error");
}