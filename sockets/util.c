#include "sockets.h"

/**
 * socket_init_in - initialize socket fd & sockaddr_in info
 * @port: desired port
 * @info: sockaddr_in info
 * Return: file descriptor for initialized socket
 */
int socket_init_in(int port, sockaddr_in_t *info)
{
	int sock;

	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock == -1)
		perror("socket"), exit(EXIT_FAILURE);
	info->sin_family = AF_INET;
	info->sin_port = htons(port);
	info->sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(sock, (sockaddr_t *)info, sizeof(sockaddr_t)) == -1)
		perror("bind"), exit(EXIT_FAILURE);
	return (sock);
}

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

void sender_closer(int sock_fd, char *ip, char *meth, char *path)
{
	char created[BUFSIZ] = {0}, *json = NULL;

	printf("%s %s %s -> ", ip, meth, path);
	switch (status)
	{
		case 200:
			printf(RESP_200 "\n");
			send(sock_fd, RESP_200_V, strlen(RESP_200_V), MSG_NOSIGNAL);
			break;
		case 201:
			printf(RESP_201 "\n");
			json = added_json();
			sprintf(created, RESP_201_1 "%lu" RESP_201_2 "%s", LEN(json), json);
			free(json), json = NULL;
			send(sock_fd, created, LEN(created) + 128, MSG_NOSIGNAL);
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
	return(json);
}

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
