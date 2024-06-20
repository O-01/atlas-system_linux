#include "sockets.h"

static int post_verification(char *meth);
static int path_verification(char *path, char *expected);
static int header_parse(char *headers);
static int body_parse(char *body);

todolist_t *todos;
int status;

/**
 * main - entry point of program
 * Return: 0 upon success
 */
int main(void)
{
	int sock, in;
	socklen_t size = sizeof(sockaddr_in_t);
	sockaddr_in_t info, peer_info;
	char *ip = NULL;
	char data_buf[BUFSIZ], body[BUFSIZ], meth[16], path[PATH_MAX];
	ssize_t recvd = 0;

	todo_init();
	sock = socket_init_in(8080, &info);
	if (listen(sock, 128) == -1)
		perror("not listening"), exit(EXIT_FAILURE);
	printf("Server listening on port %d\n", ntohs(info.sin_port));
	for (;; status = 0)
	{
		in = accept(sock, (sockaddr_t *)&info, &size);
		if (in == -1)
			perror("unacceptable"), exit(EXIT_FAILURE);
		if (getpeername(in, (sockaddr_t *)&peer_info, &size) == -1)
			perror("getpeername"), exit(EXIT_FAILURE);
		ip = inet_ntoa(peer_info.sin_addr);
		recvd = recv(in, data_buf, 4096 - 1, 0);
		if (recvd == -1)
			perror("recv"), exit(EXIT_FAILURE);
		if (sscanf(data_buf, "%s %s %*[^\r\n]%*[\r\n]%[^\b]", meth, path, body) == 0)
			continue;
		if (!post_verification(meth) || !path_verification(path, "/todos"))
		{
			status = 404, sender_closer(in, ip, meth, path);
			continue;
		}
		status = header_parse(body);
		sender_closer(in, ip, meth, path);
	}
	return (EXIT_SUCCESS);
}

/**
 * free_todos - frees todo list data
 */
void __attribute__((destructor)) free_todos(void)
{
	todo_t *current = NULL, *next = NULL;

	if (todos)
	{
		for (current = todos->head; current; current = next)
		{
			next = current->next;
			free(current->description), current->description = NULL;
			free(current->title), current->title = NULL;
			free(current), current = NULL;
		}
		free(todos), todos = NULL;
	}
}

/**
 * post_verification - verifies that method is POST
 * @meth: ASCII string containing method name
 * Return: 1 if method is POST, otherwise 0
 */
static int post_verification(char *meth)
{
	return (!strcmp(meth, "POST") ? 1 : 0);
}

/**
 * path_verification - verifies that path is recognized
 * @path: ASCII string containing path name
 * @expected: ASCII string containing expected or recognized path name
 * Return: 1 if path is as expected/recognize, otherwise 0
 */
static int path_verification(char *path, char *expected)
{
	return (!strcmp(path, expected) ? 1 : 0);
}

/**
 * header_parse - parse header variables and retrieve body content
 * @headers: input string containing possible variables
 * Return: output of body_parse (201 upon success or 422 upon failure),
 *         otherwise 411 (Length Required)
 */
static int header_parse(char *headers)
{
	int iter = 0, parsed = 0, haslen = 0;
	char *extract = NULL, *body = NULL;
	char *keyvals[32] = {0}, key[128], val[128];

	do {
		extract = strsep(&headers, CRLF);
		if (extract)
			keyvals[iter++] = extract,
			parsed = 1;
	} while (extract && parsed--);
	for (iter = 0; keyvals[iter]; ++iter)
		if (sscanf(keyvals[iter], "%[^:]: %s", key, val) > 0)
		{
			if (!strcmp(key, "Content-Length"))
				haslen = 1;
		}
	body = keyvals[iter - 1];
	if (haslen && body)
		return (body_parse(body));
	return (411);
}

/**
 * body_parse - parse body content and add todo list item, as applicable
 * @body: body content to be parsed
 * Return: output of add_todo (201 upon success or 0 upon failure),
 *         otherwise 422
 */
static int body_parse(char *body)
{
	int iter = 0, added = 0, istitle = 0, isdesc = 0;
	char *extract = NULL, *keyvals[32] = {0}, key[128], val[128];
	char title[128], description[128];

	do {
		extract = strsep(&body, "&");
		if (extract && extract[0])
			keyvals[iter++] = extract,
			added = 1;
	} while (extract && added--);
	for (iter = 0; keyvals[iter]; ++iter)
	{
		if (sscanf(keyvals[iter], "%[^=]=%s", key, val) > 0)
		{
			if (!strcmp(key, "title"))
				memcpy(title, val, LEN(val) + 1), istitle = 1;
			else if (!strcmp(key, "description"))
				memcpy(description, val, LEN(val) + 1), isdesc = 1;
		}
		else
			break;
	}
	if (istitle && isdesc)
		return (add_todo(title, description));
	return (422);
}
