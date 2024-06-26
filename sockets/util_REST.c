#include "sockets.h"

int todo_init(void);
int add_todo(char *title, char *description);
int insert_todo(todo_t *todo);
void sender_closer(int sock_fd, char *ip, char *meth, char *path);
char *added_json(void);
char *all_todos(void);
char *number_toa(size_t number);

/**
 * todo_init - create and initialize todo list
 * Return: 1 upon success, 0 upon memory allocation failure
 */
int todo_init(void)
{
	todos = calloc(1, sizeof(todolist_t));
	if (!todos)
		return (0);
	todos->count = 0;
	todos->head = NULL;
	todos->tail = NULL;
	return (1);
}

/**
 * add_todo - create new todo item to be added to end of todo list
 * @title: title for new todo item
 * @description: description for new todo item
 * Return: output of insert_todo (always 201)
 */
int add_todo(char *title, char *description)
{
	todo_t *todo = NULL;

	todo = calloc(1, sizeof(todo_t));
	if (!todo)
		return (0);
	todo->title = calloc(1, sizeof(char) * LEN(title) + 1);
	if (!todo->title)
	{
		free(todo);
		return (0);
	}
	todo->description = calloc(1, sizeof(char) * LEN(description) + 1);
	if (!todo->description)
	{
		free(todo->title), free(todo);
		return (0);
	}
	todo->id = todos->count++;
	memcpy(todo->title, title, LEN(title));
	memcpy(todo->description, description, LEN(description));
	todo->next = NULL;
	return (insert_todo(todo));
}

/**
 * insert_todo - adds newly created todo item to end of todo list
 * @todo: newly created todo item to be added to end of todo list
 * Return: 201 upon success
 */
int insert_todo(todo_t *todo)
{
	todo_t *tmp = NULL;

	if (!todos->head)
	{
		todo->prev = NULL;
		todos->head = todo;
		todos->tail = todo;
		return (201);
	}
	for (tmp = todos->head; tmp != todos->tail; tmp = tmp->next)
		;
	todos->tail->next = todo;
	todo->prev = todos->tail;
	todos->tail = todo;
	return (201);
}

/**
 * sender_closer - sends HTTP response related to request received from
 *                 connected socket along with relevant data, as applicable
 * @sock_fd: connected socket file descriptor
 * @ip: IP address associated with connected socket
 * @meth: ASCII string containing method name
 * @path: ASCII string containing path name
 */
void sender_closer(int sock_fd, char *ip, char *meth, char *path)
{
	char created[BUFSIZ] = {0}, *json = NULL;

	printf("%s %s %s -> ", ip, meth, path);
	switch (status)
	{
		case 200:
			printf(RESP_200 "\n");
			json = all_todos();
			sprintf(created, RESP_200_1 "%lu" RESP_200_2 "%s", LEN(json), json);
			free(json), json = NULL;
			send(sock_fd, created, LEN(created) + 1, MSG_NOSIGNAL);
			break;
		case 201:
			printf(RESP_201 "\n");
			json = added_json();
			sprintf(created, RESP_201_1 "%lu" RESP_201_2 "%s", LEN(json), json);
			free(json), json = NULL;
			send(sock_fd, created, LEN(created) + 1, MSG_NOSIGNAL);
			break;
		case 404:
			printf(RESP_404 "\n");
			send(sock_fd, RESP_404_V, strlen(RESP_404_V), MSG_NOSIGNAL);
			break;
		case 422:
			printf(RESP_422 "\n");
			send(sock_fd, RESP_422_V, strlen(RESP_422_V), MSG_NOSIGNAL);
			break;
		default:
			break;
	}
	fflush(stdout);
	close(sock_fd);
}

/**
 * added_json - retrieve most recently added todo item data in JSON format
 * Return: most recently added todo item data in JSON format
 */
char *added_json(void)
{
	char data[BUFSIZ] = {0}, *json = NULL, *id = NULL;
	todo_t *todo = todos->tail;

	data[0] = '{';
	data[1] = '\0';
	strcat(data, "\"id\":");
	id = number_toa(todo->id);
	strcat(data, id);
	free(id), id = NULL;
	strcat(data, ",\"title\":\"");
	strcat(data, todo->title);
	strcat(data, "\",\"description\":\"");
	strcat(data, todo->description);
	strcat(data, "\"}");

	json = calloc(1, sizeof(char) * LEN(data) + 1);
	if (!json)
		return ("");
	memcpy(json, data, LEN(data) + 1);
	return (json);
}

/**
 * all_todos - retrieve all todo data in JSON format
 * Return: all todo data in JSON format
 */
char *all_todos(void)
{
	char data[BUFSIZ] = {0}, *json = NULL, *id = NULL;
	todo_t *todo = NULL;

	data[0] = '[';
	data[1] = '\0';
	for (todo = todos->head; todo; todo = todo->next)
	{
		strcat(data, "{\"id\":");
		id = number_toa(todo->id);
		strcat(data, id);
		free(id), id = NULL;
		strcat(data, ",\"title\":\"");
		strcat(data, todo->title);
		strcat(data, "\",\"description\":\"");
		strcat(data, todo->description);
		strcat(data, "\"}");
		if (todo->next)
			strcat(data, ",");
	}
	strcat(data, "]");
	json = calloc(1, sizeof(char) * LEN(data) + 1);
	if (!json)
		return ("");
	memcpy(json, data, LEN(data) + 1);
	return (json);
}

/**
 * number_toa - converts number from int to ASCII string
 * @number: int to be converted to ASCII string
 * Return: ASCII string version of input int
 */
char *number_toa(size_t number)
{
	int digit = 15, len = 0;
	char parsed_str[16], *output;

	parsed_str[digit] = '\0';
	do {
		parsed_str[--digit] = number % 10 + '0';
		number /= 10;
		len++;
	} while (number > 0);

	output = calloc(1, sizeof(char) * len + 1);
	if (!output)
		return ("");
	memcpy(output, &parsed_str[digit], len + 1);
	return (output);
}
