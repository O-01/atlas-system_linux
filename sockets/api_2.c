#include "sockets.h"

static void header_parse(char *head);

/**
 * main - entry point of program
 * Return: 0 upon success
 */
int main(void)
{
	int sock, in;
	socklen_t size = sizeof(sockaddr_in_t);
	sockaddr_in_t info, peer_info;
	char data_buf[BUFSIZ], profil[PATH_MAX], header[1024];
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
		if (sscanf(data_buf, "%[^\r\n]%*[\r\n]%[^DOPPEL]", profil, header) > 0)
			/* printf("profile: %s\n", profile), */
			/* printf(LINE "HEADERS:\n%s" LINE, header), */
			header_parse(header);
		fflush(stdout);
		send(in, RESP_200_V, strlen(RESP_200_V), MSG_NOSIGNAL);
		close(in);
	}
	return (EXIT_SUCCESS);
}

/**
 * header_parse - parse HTTP request headers & print keys/values
 * @head: input string containing possible variables
 */
static void header_parse(char *head)
{
	int iter = 0, added = 0;
	char *extract = NULL, *keyvals[32] = {0}, key[128], val[128];

	do {
		extract = strsep(&head, CRLF);
		if (extract)
			keyvals[iter++] = extract, added = 1;
	} while (extract && added--);

	for (iter = 0; keyvals[iter]; ++iter)
	{
		if (sscanf(keyvals[iter], "%[^:]: %s", key, val) > 0)
			printf("Header: \"%s\" -> \"%s\"\n", key, val);
	}

}
