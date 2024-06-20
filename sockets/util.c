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
