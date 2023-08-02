#include <time.h>
#include <string.h>

void concatStr(char *result, char *a, char *b) {
	int lenA = strlen(a), lenB = strlen(b);
	for (int i = 0; i < lenA; i++)
		result[i] = a[i];
	int j = 0;
	for (int i = lenA; i < (lenA + lenB); i++) {
		result[i] = b[j];
		j++;
	}
}

void addTimeToMsg(char *result, char *msg) {
	struct tm *locTime;
	time_t curTime;

	char tStr[12];
	curTime = time(NULL); //Getting current time of a system
	locTime = localtime(&curTime);// Converting current time to local time
	strftime(tStr, 12, "[%I:%M %p] ", locTime);
	concatStr(result, tStr, msg);
}

void clearLastLine() {
	write(1, "\x1b[1F\x1b[2K", 9); // Move to a beginning of previous line and clear the entire line
}