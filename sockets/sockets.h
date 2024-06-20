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

#define TO_STR(x) #x
#define STR(x) TO_STR(x)

#define LEN(x) strlen(x)

#define CRLF "\r\n"
#define DOPPEL "\r\n\r\n"

/* FOR TESTING */
#define LINE "------------------------------------------------------------\n"

#define RESP_200 "200 OK"
#define RESP_201 "201 Created"
#define RESP_404 "404 Not Found"
#define RESP_411 "411 Length Required"
#define RESP_422 "422 Unprocessable Entity"

#define VERSER "HTTP/1.1"

#define RESP_200_V VERSER " " RESP_200
#define RESP_201_V VERSER " " RESP_201
#define RESP_404_V VERSER " " RESP_404 DOPPEL
#define RESP_411_V VERSER " " RESP_411 DOPPEL
#define RESP_422_V VERSER " " RESP_422 DOPPEL

#define CONTENTLEN "Content-Length"

#define CONTENTTYPE_JSON "Content-Type: application/json"

#define RESP_200_1 RESP_200_V CRLF CONTENTLEN ": "
#define RESP_200_2 CRLF CONTENTTYPE_JSON "" DOPPEL

#define RESP_201_1 RESP_201_V CRLF CONTENTLEN ": "
#define RESP_201_2 CRLF CONTENTTYPE_JSON "" DOPPEL

#define CRLF_LEN 2
#define DOPPEL_LEN 4

/* NO LONGER USED - KEPT FOR REFERENCE */
#define RESPLEN_201(l, json) (\
	LEN(RESP_201_V) + CRLF_LEN +\
	LEN(CONTENTLEN) + LEN(": ") + LEN(STR(l)) + CRLF_LEN +\
	LEN(CONTENTTYPE_JSON) + DOPPEL_LEN +\
	LEN(json))

/* #include <stddef.h> */
/* #include <sys/un.h> */
/* #include <net/if.h> */

/**
 * struct to_do - todo list item data (doubly-linked list node)
 * @id: todo list item id
 * @title: todo list item title
 * @description: todo list item description
 * @next: pointer to next todo list item - NULL if item is only/last todo
 * @prev: pointer to previous todo list item - NULL if item is first todo
 */
typedef struct to_do
{
	size_t id;
	char *title;
	char *description;
	struct to_do *next;
	struct to_do *prev;
} todo_t;

/**
 * struct to_do_list - todo list data (doubly-linked list container)
 * @count: count of todo list items
 * @head: first item in todo list
 * @tail: last item in todo list
 */
typedef struct to_do_list
{
	size_t count;
	struct to_do *head;
	struct to_do *tail;
} todolist_t;

extern todolist_t *todos;
extern int status;

/* util.c */
int socket_init_in(int sock, sockaddr_in_t *info);

/* util_REST.c */
int todo_init(void);
int add_todo(char *title, char *description);
int insert_todo(todo_t *todo);
void sender_closer(int sock_fd, char *ip, char *meth, char *path);
char *added_json(void);
char *all_todos(void);
char *number_toa(size_t number);

#endif