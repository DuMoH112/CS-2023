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

bool is_equal_number(int input_number, int rand_number) {
	if (input_number > rand_number) {
		printf("Моё число меньше\n\n");
		return false;
	} else if (input_number < rand_number) {
		printf("Моё число больше\n\n");
		return false;
	} else {
		printf("\nВы угадали, загаданное число - %d", rand_number);
		return true;
	}
}

int main() {
	printf("Введите q если хотите выйти\n\n");
	int r_number = get_random_number();
	bool isLoop = true;
	int number;
	int number_attempt = MAX_ATTEMPTS;

	do {
		printf("Кол-во оставшихся попыток: %d\n", number_attempt);
		if (--number_attempt == -1) {
			printf("\nВы проиграли, количество попыток исчерпано. Загаданное число - %d", r_number);
			return 0;
		}
		number = get_input_number(&isLoop);
		if (isLoop == false)
			return 0;
	} while (!is_equal_number(number, r_number));

	return 0;
}
