#include <sys/socket.h>
#include <sys/wait.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "../auth/auth.h"

#define IP_ADDR "127.0.0.1"
#define PORT "8088"

int conn(struct addrinfo *res) {
	struct addrinfo hints;

	memset(&hints, 0, sizeof hints); // Обнуляем все байты в структуре

	hints.ai_family = AF_INET;       // IPv4
	hints.ai_socktype = SOCK_STREAM; // TCP (Stream Sockets)

	getaddrinfo(IP_ADDR, PORT, &hints, &res);

	int status, socket_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	status = connect(socket_fd, res->ai_addr, res->ai_addrlen);
	if (status < 0) {
		printf("\nConnection Failed \n");
		exit(1);
	}

	return socket_fd;
}

void EXIT(int socket_fd, user u) {
	char msg[MAX_LEN] = {0,};
	preparedMsg(u, msg, "Left chat.\n");

	write(socket_fd, msg, sizeof(msg));

	close(socket_fd);
	printf("\nYou have left the chat.\n");
	exit(0);
}

void send_msg(int socket_fd, user u, char msg[MAX_LEN_MSG]) {
	if (strcmp(msg, "exit\n") == 0) {
		EXIT(socket_fd, u);
	}

	char result[MAX_LEN] = {0,};
	preparedMsg(u, result, msg);

	write(socket_fd, result, sizeof(result));
	clearLastLine();
	printf("%s", result);
}

int main() {
	struct addrinfo *res = {0,};
	struct timeval tv;    /* переменная, указывающая, как долго ждать возврата select() */

	long int bytes;
	int socket_fd = conn(res);

	user u = auth();
	fd_set fds;        /* набор файловых дескрипторов, используемый в select() */

	do {
		char buf[MAX_LEN] = {0,};
		/* Установите переменную fds в набор файловых дескрипторов для отслеживания ввода */
		FD_ZERO(&fds);
		FD_SET(0, &fds);
		FD_SET(socket_fd, &fds);
		tv.tv_sec = 1;
		tv.tv_usec = 0;

		if (select(socket_fd + 1, &fds, NULL, NULL, &tv) > 0) {
			/* Отвечает за отправку сообщения */
			if (FD_ISSET(0, &fds)) {
				if (read(0, buf, sizeof(buf)) < 0) {
					perror("read");
					break;
				}
				send_msg(socket_fd, u, buf);
			}
			/* Отвечает за приём сообщения */
			if (FD_ISSET(socket_fd, &fds)) {
				if ((bytes = read(socket_fd, buf, sizeof(buf))) < 0) {
					perror("read");
					break;
				}

				write(1, buf, bytes);
			}
		}
	} while (1);

	EXIT(socket_fd, u);
}
