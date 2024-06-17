#include "sockets.h"

/**
 * main - entry point of program
 * @argc: count of arguments supplied at launch
 * @argv: string vector of arguments supplied at launch
 * Return: 0 upon success
 */
int main(int argc, char **argv)
{
	int sock, status;
	char *host = NULL, *port = NULL, ip[INET_ADDRSTRLEN];
	socklen_t size = sizeof(struct sockaddr_in);
	struct addrinfo hints, *host_info, *tmp_host;
	struct sockaddr_in connection, *address;

	if (argc != 3)
	{
		fprintf(stderr, "Usage: %s <host> <port>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	host = argv[1], port = argv[2];
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;
	status = getaddrinfo(host, port, &hints, &host_info);
	if (status)
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status)),
		exit(EXIT_FAILURE);
	for (tmp_host = host_info; tmp_host; tmp_host = tmp_host->ai_next)
	{
		address = (struct sockaddr_in *)tmp_host->ai_addr;
		sock = socket(PF_INET, SOCK_STREAM, 0);
		if (sock == -1)
			continue;
		connection.sin_family = AF_INET;
		connection.sin_port = htons(atoi(port));
		inet_ntop(AF_INET, &(address->sin_addr), ip, INET_ADDRSTRLEN);
		connection.sin_addr.s_addr = inet_addr(ip);
		if (connect(sock, (struct sockaddr *)&connection, size) == -1)
			perror("connect"), exit(EXIT_FAILURE);
		else
			break;
	}
	freeaddrinfo(host_info);
	printf("CONNECTED TO %s:%s (%s:%s) \\('-`)/\n", host, port, ip, port);
	return (0);
}
