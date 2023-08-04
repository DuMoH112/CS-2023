#include <time.h>
#include <stdio.h>
#include <string.h>

#include "../utils/utils.h"

#define MAX_LEN 256
#define MAX_LEN_TIME 12
#define MAX_LEN_USERNAME 64
#define MAX_LEN_MSG (MAX_LEN-MAX_LEN_USERNAME-16)

struct userStruct {
	char name[MAX_LEN_USERNAME];
} userDefault = {.name={0,}};
typedef struct userStruct user;


user auth() {
	user u;
	printf("Введите ваш логин: ");
	scanf("%s", u.name);
	printf("\nДля выхода напишите - exit");
	printf("\n=====START=====\n");

	return u;
};

void preparedMsg(user u, char *outStr, char msg[MAX_LEN_MSG]) {
	sprintf(outStr, "%s: %s", u.name, msg);

	char result[MAX_LEN] = {0,};
	addTimeToMsg(result, outStr);
	sprintf(outStr, "%s", result);
}