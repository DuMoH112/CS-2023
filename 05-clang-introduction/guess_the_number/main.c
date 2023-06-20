#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int get_random_number() {
	// Use current time as seed for random generator
	srand(time(NULL)); // NOLINT(cert-msc51-cpp)
	return rand() % 100; // NOLINT(cert-msc50-cpp)
}

int get_input_number(bool *isLoop) { // NOLINT(misc-no-recursion)
	char num1[256];

	printf("Введите число: ");
	scanf("%s", num1);
	if (strcmp(num1, "q") == 0) {
		*isLoop = false;
		return 0;
	}
	int _scanf = sscanf(num1, "%[0-9]d", num1);
	if (_scanf != 1) {
		printf("Number not found\n");
		return get_input_number(isLoop);
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
	while (1) {
		int number = get_input_number(&isLoop);
		if (isLoop == false) {
			return 0;
		}
		if (is_equal_number(number, r_number))
			break;
	}

	return 0;
}
