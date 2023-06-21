#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_ATTEMPTS 10

int get_random_number() {
	// Use current time as seed for random generator
	srand(time(NULL)); // NOLINT(cert-msc51-cpp)
	return rand() % 1000; // NOLINT(cert-msc50-cpp)
}

int get_input_number(bool *isLoop) { // NOLINT(misc-no-recursion)
	char num1[4];

	printf("Введите число: ");
	scanf("%s", num1);
	if (strcmp(num1, "q") == 0) {
		*isLoop = false;
		return 0;
	}

	return atoi(num1); // convert the string to int NOLINT(cert-err34-c)
}

bool is_not_equal_number(int input_number, int rand_number, int number_attempt) {
	if (input_number > rand_number) {
		printf("(%d)Моё число меньше\n\n", number_attempt);
		return true;
	} else if (input_number < rand_number) {
		printf("(%d)Моё число больше\n\n", number_attempt);
		return true;
	} else {
		printf("\nВы угадали, загаданное число - %d. Количество попыток - %d", rand_number,
			   MAX_ATTEMPTS - number_attempt);
		return false;
	}
}

int main() {
	printf("Введите q если хотите выйти\n\n");
	int r_number = get_random_number();
	bool isLoop = true;
	int number;
	int number_attempt = MAX_ATTEMPTS;

	do {
		if (--number_attempt == -1) {
			printf("\nВы проиграли, количество попыток исчерпано. Загаданное число - %d", r_number);
			return 0;
		}
		number = get_input_number(&isLoop);
		if (isLoop == false)
			return 0;
	} while (is_not_equal_number(number, r_number, number_attempt));

	return 0;
}
