#ifndef _SOCKETS_H_
#define _SOCKETS_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <linux/limits.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <netinet/in.h>
#include <arpa/inet.h>

#ifndef __USE_XOPEN2K
#define __USE_XOPEN2K
#endif

#include <netdb.h>

#ifndef __USE_MISC
#define __USE_MISC
#endif

#include <string.h>

typedef struct sockaddr_in sockaddr_in_t;
typedef struct sockaddr sockaddr_t;

#define CRLF "\r\n"
#define DOPPEL "\r\n\r\n"
#define LINE "------------------------------------------------------------\n"

#define RESP_200_V "HTTP/1.1 200 OK" DOPPEL

/* #include <stddef.h> */
/* #include <sys/un.h> */
/* #include <net/if.h> */

int socket_init_in(int sock, sockaddr_in_t *info);

#endif