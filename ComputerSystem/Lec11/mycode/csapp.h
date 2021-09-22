#ifndef __CSAPP_H__
#define __CSAPP_H__
#include <netdb.h>
#include <string.h>
#include <stdio.h>

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

/* Reentrant protocol-independent client/server helpers */
int open_clientfd(char *hostname, char *port);
int open_listenfd(char *port);

/* Wrappers for reentrant protocol-independent client/server helpers */
int Open_clientfd(char *hostname, char *port);
int Open_listenfd(char *port);

#endif