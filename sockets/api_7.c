#include "sockets.h"

static int meth_verification(char *meth);
static int only_path(char *base, char *expected);
static int path_verification(char *base, char *quer, char *expected);
static int header_parse(char *headers);
static int body_parse(char *body);
static int query_parse(char *quer);

todolist_t *todos;
int status;
int meth_code;
size_t req_id;

/**
 * main - entry point of program
 * Return: 0 upon success
 */
int main(void)
{
	int sock, in;
	socklen_t size = sizeof(sockaddr_in_t);
	sockaddr_in_t info, peer_info;
	char *ip = NULL, base[PATH_MAX], quer[128];
	char data_buf[BUFSIZ], body[BUFSIZ], meth[16], path[PATH_MAX];
	ssize_t recvd = 0;

	todo_init();
	sock = socket_init_in(8080, &info);
	if (listen(sock, 128) == -1)
		perror("not listening"), exit(EXIT_FAILURE);
	printf("Server listening on port %d\n", ntohs(info.sin_port));
	for (;; status = 0, meth_code = 0, req_id = INT_MAX)
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
		if (sscanf(path, "%[^?]?%s", base, quer) == 0)
			continue;
		if (meth_verification(meth) == 404 ||
			only_path(base, "/todos") == 404)
		{
			status = 404, sender_closer_adv(in, ip, meth, base);
			continue;
		}
		status = meth_code == 1 ? header_parse(body)
			: path_verification(base, quer, "/todos");
		sender_closer_adv(in, ip, meth, base);
		nullify(ip), nullify(base), nullify(quer);
		nullify(data_buf), nullify(body), nullify(meth), nullify(path);
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
 * meth_verification - verifies that method is POST or GET and sets meth_code
 * @meth: ASCII string containing method name
 * Return: 1 if method is POST or GET, otherwise 0
 */
static int meth_verification(char *meth)
{
	if (!strcmp(meth, "POST"))
		meth_code = 1;
	else if (!strcmp(meth, "GET"))
		meth_code = 2;
	else if (!strcmp(meth, "DELETE"))
		meth_code = 3;
	return (meth_code ? 1 : 404);
}

/**
 * only_path - ONLY verifies that path is recognized
 * @base: ASCII string containing path name
 * @expected: ASCII string containing expected or recognized path name
 * Return: 1 if path is as expected/recognize, otherwise 404
 */
static int only_path(char *base, char *expected)
{
	return (!strcmp(base, expected) ? 1 : 404);
}

/**
 * path_verification - verifies that path is recognized, calling query parse if
 *                     queries exist
 * @base: ASCII string containing path name
 * @quer: ASCII string containing any queries, may be NULL or zero length
 * @expected: ASCII string containing expected or recognized path name
 * Return: output of query_parse if path is as expected/recognized and there
 *         are queries, 200 if path is as expected/recognize and there are no
 *         queries, otherwise 404
 */
static int path_verification(char *base, char *quer, char *expected)
{
	int valid = 0;

	if (!strcmp(base, expected))
		valid = 1;
	return (valid && LEN(quer) ? query_parse(quer) : valid ? 200 : 404);
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

/**
 * query_parse - parse path query variables & print keys/values
 * @quer: input string containing possible variables
 * Return: output of delete_todo if method is DELETE and query contains
 *         valid/existing todo item id, 200 if query contains
 *         valid/existing todo item id and method is not DELETE, otherwise 404
 */
static int query_parse(char *quer)
{
	int iter = 0, added = 0, has_id = 0;
	char *extract = NULL, *keyvals[32] = {0}, key[128], val[128];

	do {
		extract = strsep(&quer, "&");
		if (extract && extract[0])
			keyvals[iter++] = extract, added = 1;
	} while (extract && added--);

	for (iter = 0; keyvals[iter]; ++iter)
	{
		if (sscanf(keyvals[iter], "%[^=]=%s", key, val) > 0)
		{
			if (!strcmp(key, "id") && LEN(val))
				req_id = atoi(val), has_id = 1;
		}
		else
			break;
	}
	printf("has_id: %d\n", has_id);
	if (has_id && find_id(req_id))
		return (meth_code == 3 ? delete_todo(req_id) : 200);
	return (404);
}
