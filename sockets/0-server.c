#include "sockets.h"

/**
 * main - entry point of program
 * Return: 0 upon success
 */
int main(void)
{
	int sock;
	struct sockaddr_in info;

	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock < 0)
		perror("socket"), exit(EXIT_FAILURE);
	info.sin_family = AF_INET;
	info.sin_port = htons(12345);
	info.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(sock, (struct sockaddr *)&info, sizeof(struct sockaddr)) < 0)
		perror("bind"), exit(EXIT_FAILURE);
	if (listen(sock, 128) < 0)
		perror("not listening"), exit(EXIT_FAILURE);
	printf("PORT [%d] : LISTENING O_O\n", ntohs(info.sin_port));
	for (;;)
		;
	return (EXIT_SUCCESS);
}
