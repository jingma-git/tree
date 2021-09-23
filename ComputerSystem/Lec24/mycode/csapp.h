#ifndef __CSAPP_H__
#define __CSAPP_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <setjmp.h>
#include <signal.h>
#include <dirent.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>
#include <math.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

typedef struct sockaddr SA;

// Persistent state for the robust I/O (Rio) package
#define RIO_BUFSIZE 8192
typedef struct
{
    int rio_fd;                // descriptor for this internal buf
    int rio_cnt;               // unread bytes in internal buf
    char *rio_bufptr;          // next unread byte in internal buf
    char rio_buf[RIO_BUFSIZE]; // internal buf
} rio_t;

/* Misc constants */
#define MAXLINE 8192 /* Max text line length */
#define MAXBUF 8192  /* Max I/O buffer size */
#define LISTENQ 1024 /* Second argument to listen() */

/* Our own error-handling functions */
void unix_error(char *msg);
void posix_error(int code, char *msg);
void dns_error(char *msg);
void gai_error(int code, char *msg);
void app_error(char *msg);

// rio
ssize_t rio_writen(int fd, void *usrbuf, size_t n);
void rio_readinitb(rio_t *rp, int fd);
ssize_t rio_readnb(rio_t *rp, void *usrbuf, size_t n);
ssize_t rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen);

/* Reentrant protocol-independent client/server helpers */
int open_clientfd(char *hostname, char *port);
int open_listenfd(char *port);

/* Wrappers for reentrant protocol-independent client/server helpers */
int Open_clientfd(char *hostname, char *port);
int Open_listenfd(char *port);

// socket interface wrappers
int Accept(int s, struct sockaddr *addr, socklen_t *addrlen);

// protocol independent wrappers
void Getnameinfo(const struct sockaddr *sa, socklen_t salen, char *host,
                 size_t hostlen, char *serv, size_t servlen, int flags);

// unix I/O wrappers
void Close(int fd);

// std I/Wrappers
char *Fgets(char *ptr, int n, FILE *stream);
void Fputs(const char *ptr, FILE *stream);

// concurrent programming: create subprocess by 2^n - 1
pid_t Fork(void);

// threads
void Pthread_create(pthread_t *tidp, pthread_attr_t *attrp,
                    void *(*routine)(void *), void *argp);

void Pthread_detach(pthread_t tid);
#endif