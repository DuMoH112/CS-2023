#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <sys/wait.h>

#include "../auth/auth.h"

#define PORT "8088"
#define MAX_CLIENTS 2
#define TIMEOUT 120

int conn(struct addrinfo *res) {
	struct addrinfo hints;

	memset(&hints, 0, sizeof hints); // Обнуляем все байты в структуре

	hints.ai_family = AF_INET;       // IPv4
	hints.ai_socktype = SOCK_STREAM; // TCP (Stream Sockets)
	hints.ai_flags = AI_PASSIVE;     // Автоматическое определение нашего IP

	getaddrinfo(NULL, PORT, &hints, &res);

	int reuse, socketFd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	reuse = 1;
	if (setsockopt(socketFd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
		perror("set_sock_opt");
		exit(1);
	}

	if (bind(socketFd, res->ai_addr, res->ai_addrlen) < 0) {
		perror("bind");
		exit(1);
	}

	/* Настройка количества резервных клиентов */
	if (listen(socketFd, MAX_CLIENTS) < 0) {
		perror("listen");
		exit(1);
	}

	return socketFd;
}

void EXIT(int sockets[MAX_CLIENTS], int top) {
	for (int i = 0; i <= top; i++) {
		write(sockets[i], "\nServer: Server shutdown.", 26);
		close(sockets[i]);
	}
	printf("\nServer: Server shutdown.\n");
	exit(0);
}

void connectNewUsers(int sockets[MAX_CLIENTS], int sock, int *countUsers) {
	// find a new place for socket connect
	int newIdx = -1;
	for (int i = 0; i < MAX_CLIENTS; i++)
		if (sockets[i] == -1) {
			newIdx = i;
			break;
		}

	struct hostent *cp[MAX_CLIENTS];
	struct sockaddr_in clt;
	socklen_t clen = sizeof(clt);

	if (*countUsers >= MAX_CLIENTS || (sockets[newIdx] = accept(sock, (struct sockaddr *) &clt, &clen)) < 0) {
		write(sockets[newIdx], "REQUEST REJECTED. Expect free space.\n", 38);
		close(sockets[newIdx]);
	} else {
		cp[newIdx] = gethostbyaddr((char *) &clt.sin_addr, sizeof(struct in_addr), AF_INET);
		write(sockets[newIdx], "REQUEST ACCEPTED\n", 17);
		printf("Connected ip [%s] to slot %d\n", cp[newIdx]->h_name, newIdx);

		*countUsers = *countUsers + 1;
	}
}

void disconnectUser(int sockets[MAX_CLIENTS], int index, int *countUsers) {
	sockets[index] = -1;
	*countUsers = *countUsers - 1;
	fprintf(stderr, "\nUser disconnected from slot %d\n", index);
}

int isDisconnectedUser(char buf[MAX_LEN - MAX_LEN_TIME]) {
	return strstr(buf, ": Left chat.") != NULL ? 1 : 0;
}

void listenUserMessages(
		int sockets[MAX_CLIENTS],
		char buf[MAX_LEN - MAX_LEN_TIME],
		fd_set fds, long int bytes,
		int *countUsers
) {
	for (int i = 0; i < MAX_CLIENTS; i++) {
		if (sockets[i] == -1) continue;

		if (FD_ISSET(sockets[i], &fds)) {
			if ((bytes = read(sockets[i], buf, MAX_LEN - MAX_LEN_TIME)) < 0) {
				perror("read client");
				continue;
			}
			if (bytes > 1) {
				if (isDisconnectedUser(buf)) disconnectUser(sockets, i, countUsers);

				write(1, buf, strlen(buf));

				for (int j = 0; j < MAX_CLIENTS; j++) {
					if (j != i) {
						write(sockets[j], buf, strlen(buf));
					}
				}
			}
		}
	}
}

int main() {

	struct addrinfo *res = {0,};

	struct timeval tv;


	long int bytes;
	int sockets[MAX_CLIENTS] = {[0 ... MAX_CLIENTS - 1] = -1};
	int max, countUsers = 0, shut = TIMEOUT / 2, top = -1;
	int sock = conn(res);
	time_t inactive = time(NULL);
	fd_set fds;

	do {
		char buf[MAX_LEN - MAX_LEN_TIME] = {0,};

		/* Установите переменную fds в набор файловых дескрипторов для отслеживания ввода */
		FD_ZERO(&fds);
		FD_SET(0, &fds);
		FD_SET(sock, &fds);
		tv.tv_sec = 1;
		tv.tv_usec = 0;
		max = sock;

		if (countUsers == 0 && time(NULL) - inactive >= TIMEOUT) { break; }
		else if (countUsers == 0 && time(NULL) - inactive >= TIMEOUT / 2) {
			printf("\nServer: Shutdown in %d seconds", shut--);
		}

		//checkConnections
		for (int i = 0; i < MAX_CLIENTS; i++) {
			if (sockets[i] == -1) continue;

			int error;
			socklen_t errorLen = sizeof(error);
			int retVal = getsockopt(sockets[i], SOL_SOCKET, SO_ERROR, &error, &errorLen);

			if (retVal != 0) {
				/* there was a problem getting the error code */
				fprintf(stderr, "error getting socket error code: %s\n", strerror(retVal));
			}

			if (error != 0) {
				/* socket has a non zero error status. Disconnected client */
				disconnectUser(sockets, i, &countUsers);
			}

			FD_SET(sockets[i], &fds);
			if (sockets[i] > max) { max = sockets[i]; }
		}
		//---

		if (select(max + 1, &fds, NULL, NULL, &tv) > 0) {
			inactive = time(NULL);

			// Чтение сообщений сервера
			if (FD_ISSET(0, &fds)) {
				if ((bytes = read(0, buf, sizeof(buf))) < 0) {
					perror("read server");
					continue;
				}
				if (strcmp(buf, "exit\n") == 0) {
					break;
				}

				for (int i = 0; i < MAX_CLIENTS; i++) {
					if (sockets[i] == -1) continue;

					if (bytes > 0) {
						char result[MAX_LEN] = {0,};
						char msg[MAX_LEN_MSG] = {0,};
						sprintf(msg, "Server: %s", buf);
						addTimeToMsg(result, msg);
						write(sockets[i], result, sizeof(result));
						clearLastLine();
						printf("%s", result);
					}
				}
			}

			// Прослушивание сообщений пользователей
			listenUserMessages(sockets, buf, fds, bytes, &countUsers);

			// Авторизация новых подключений
			if (FD_ISSET(sock, &fds))
				connectNewUsers(sockets, sock, &countUsers);
		}
	} while (1);

	EXIT(sockets, top);
	return 0;
}