#include "sockets.h"

/**
 * main - entry point of program
 * Return: 0 upon success
 */
int main(void)
{
	int sock, in;
	socklen_t size = sizeof(sockaddr_in_t);
	sockaddr_in_t info, peer_info;
	char data_buf[BUFSIZ], meth[16], path[PATH_MAX], vers[16];
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
		if (sscanf(data_buf, "%s %s %s", meth, path, vers) > 0)
			printf("Method: %s\nPath: %s\nVersion: %s\n", meth, path, vers);
		fflush(stdout);
		send(in, RESP_200_V DOPPEL, LEN(RESP_200_V) + DOPPEL_LEN, MSG_NOSIGNAL);
		close(in);
	}
	return (EXIT_SUCCESS);
}
