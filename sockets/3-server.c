#include "sockets.h"

/**
 * main - entry point of program
 * Return: 0 upon success
 */
int main(void)
{
	int sock, in;
	socklen_t size = sizeof(struct sockaddr_in);
	struct sockaddr_in info, peer_info;
	char data_buf[512];
	ssize_t recvd = 0;

	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock == -1)
		perror("socket"), exit(EXIT_FAILURE);
	info.sin_family = AF_INET;
	info.sin_port = htons(12345);
	info.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(sock, (struct sockaddr *)&info, sizeof(struct sockaddr)) == -1)
		perror("bind"), exit(EXIT_FAILURE);
	if (listen(sock, 128) == -1)
		perror("not listening"), exit(EXIT_FAILURE);
	printf("Server listening on port %d\n", ntohs(info.sin_port));
	for (;;)
	{
		in = accept(sock, (struct sockaddr *)&info, &size);
		if (in == -1)
			perror("unacceptable"), exit(EXIT_FAILURE);
		if (getpeername(in, (struct sockaddr *)&peer_info, &size) == -1)
			perror("getpeername"), exit(EXIT_FAILURE);
		printf("Client connected: %s\n", inet_ntoa(peer_info.sin_addr));
		recvd = recv(in, data_buf, 512 - 1, 0);
		if (recvd == -1)
			perror("recv"), exit(EXIT_FAILURE);
		printf("Message received: \"%s\"\n", data_buf);
		close(in);
		break;
	}
	return (EXIT_SUCCESS);
}
