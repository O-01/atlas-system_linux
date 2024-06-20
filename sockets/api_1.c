#include "sockets.h"

static void query_parse(char *quer);

/**
 * main - entry point of program
 * Return: 0 upon success
 */
int main(void)
{
	int sock, in;
	socklen_t size = sizeof(sockaddr_in_t);
	sockaddr_in_t info, peer_info;
	char data_buf[BUFSIZ], meth[16], path[PATH_MAX], quer[512];
	ssize_t recvd = 0;

	sock = socket_init_in(8080, &info);
	if (listen(sock, 128) == -1)
		perror("not listening"), exit(EXIT_FAILURE);
	printf("Server listening on port %d\n", ntohs(info.sin_port));
	for (;;)
	{
		in = accept(sock, (sockaddr_t *)&info, &size);
		if (in == -1)
			perror("unacceptable"), exit(EXIT_FAILURE);
		if (getpeername(in, (sockaddr_t *)&peer_info, &size) == -1)
			perror("getpeername"), exit(EXIT_FAILURE);
		printf("Client connected: %s\n", inet_ntoa(peer_info.sin_addr));
		recvd = recv(in, data_buf, 4096 - 1, 0);
		if (recvd == -1)
			perror("recv"), exit(EXIT_FAILURE);
		printf("Raw request: \"%s\"\n", data_buf);
		if (sscanf(data_buf, "%s %[^?]?%s", meth, path, quer) > 0)
			printf("Path: %s\n", path);
		query_parse(quer);
		fflush(stdout);
		send(in, RESP_200_V DOPPEL, LEN(RESP_200_V) + DOPPEL_LEN, MSG_NOSIGNAL);
		close(in);
	}
	return (EXIT_SUCCESS);
}

/**
 * query_parse - parse path query variables & print keys/values
 * @quer: input string containing possible variables
 */
static void query_parse(char *quer)
{
	int iter = 0, added = 0;
	char *extract = NULL, *keyvals[32] = {0}, key[128], val[128];

	do {
		extract = strsep(&quer, "&");
		if (extract && extract[0])
			keyvals[iter++] = extract, added = 1;
	} while (extract && added--);

	for (iter = 0; keyvals[iter]; ++iter)
	{
		if (sscanf(keyvals[iter], "%[^=]=%s", key, val) > 0)
			printf("Query: \"%s\" -> \"%s\"\n", key, val);
		else
			break;
	}
}
