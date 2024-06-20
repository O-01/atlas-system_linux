#include "sockets.h"

static int meth_verification(char *meth);
static int path_verification(char *meth, char *expected);
static int header_parse(char *body);
static int query_parse(char *quer);

todolist_t *todos;
int status;
int meth_code;

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
	for (;; status = 0, meth_code = 0)
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
		/* printf("Raw request: \"%s\"\n", data_buf); */
		if (sscanf(data_buf, "%s %s %*[^\r\n]%*[\r\n]%[^\b]", meth, path, body) == 0)
			continue;
		if (!meth_verification(meth) || !path_verification(path, "/todos"))
		{
			status = 404, sender_closer(in, ip, meth, path);
			continue;
		}
		status = meth_code == 1 ? header_parse(body) : 200;
		sender_closer(in, ip, meth, path);
	}
	return (EXIT_SUCCESS);
}

void __attribute__((destructor)) free_todos(void)
{
	todo_t *current = NULL, *next = NULL;

	for (current = todos->head; current; current = next)
	{
		next = current->next;
		free(current->description), current->description = NULL;
		free(current->title), current->title = NULL;
		free(current), current = NULL;
	}
	free(todos), todos = NULL;
}

/**
 * post_verification -
 */
static int meth_verification(char *meth)
{
	if (!strcmp(meth, "POST"))
		meth_code = 1;
	else if (!strcmp(meth, "GET"))
		meth_code = 2;
	return (meth_code ? 1 : 0);
}

/**
 * post_verification -
 */
static int path_verification(char *meth, char *expected)
{
	return (!strcmp(meth, expected) ? 1 : 0);
}

/**
 * header_parse - parse header variables and body content
 * @body: input string containing possible variables
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
		return (query_parse(body));
	return (411);
}

/**
 * query_parse -
 * @quer:
 */
static int query_parse(char *quer)
{
	int iter = 0, added = 0, istitle = 0, isdesc = 0;
	char *extract = NULL, *keyvals[32] = {0}, key[128], val[128];
	char title[128], description[128];

	do {
		extract = strsep(&quer, "&");
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
	/* printf("%d %d\n", istitle, isdesc); */
	/* printf("title: %s description: %s\n", title, description); */
	if (istitle && isdesc)
		return (add_todo(title, description));
	return (422);
}
