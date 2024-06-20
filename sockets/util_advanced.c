#include "sockets.h"

void sender_closer_adv(int sock_fd, char *ip, char *meth, char *path);
char *req_todo(size_t requested_id);
todo_t *find_id(size_t id);

/**
 * sender_closer_adv - sends HTTP response related to request received from
 *                 connected socket along with relevant data, as applicable
 * @sock_fd: connected socket file descriptor
 * @ip: IP address associated with connected socket
 * @meth: ASCII string containing method name
 * @path: ASCII string containing path name
 */
void sender_closer_adv(int sock_fd, char *ip, char *meth, char *path)
{
	char created[BUFSIZ] = {0}, *json = NULL;

	printf("%s %s %s -> ", ip, meth, path);
	switch (status)
	{
		case 200:
			printf(RESP_200 "\n");
			json = req_id != INT_MAX ? req_todo(req_id) : all_todos();
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
 * req_todo - retrieve todo item data related to requested id in JSON format
 * @requested_id: id of todo item requested
 * Return: todo item data related to requested id in JSON format
 */
char *req_todo(size_t requested_id)
{
	char data[BUFSIZ] = {0}, *json = NULL, *id = NULL;
	todo_t *todo = find_id(requested_id);

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
 * find_id - checks if todo id is present in list
 * @id: requested todo id against which todo list item data is compared
 * Return: pointer to todo item with matching id if found, otherwise NULL
 */
todo_t *find_id(size_t id)
{
	todo_t *current = NULL;

	for (current = todos->head; current; current = current->next)
		if (current->id == id)
			return (current);
	return (NULL);
}
