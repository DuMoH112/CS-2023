#include <stdio.h>

#define MAX_ATTEMPTS 10
#define MAX_NUMBER 1000

int get_input_prompt() { // NOLINT(misc-no-recursion)
	char prompt;

	printf("Введите подсказку: ");
	scanf(" %c", &prompt);
	switch (prompt) {
		case 108:         // l
			return 0;
		case 101:         // e
			return 1;
		case 109:         // m
			return 2;
		case 113:         // q
			return -1;
		default: {
			printf("\nКоманда не распознана\n");
			return get_input_prompt();
		}
	}
}

void calculate_new_number(int prompt, int *l_n, int *r_n, int *n) {
	switch (prompt) {
		case 0: {
			*r_n = *n;
			*n = (*l_n + *r_n) / 2;
			break;
		}
		case 2: {
			*l_n = *n;
			*n = (*l_n + *r_n) / 2;
			break;
		}
		case -1: {
			*n = *r_n / 2;
			break;
		}
		default: {
			break;
		}
	}
}

int main() {
	printf("Введите q если хотите выйти\n");
	printf("Инструкция введите знак, если:\nm(more) число больше того, что я вывел\nl(less) число меньше того, что я вывел\ne(equal) число равно загаданному\n\n");
	printf("Загадайте число от 1 до %d\n\n", MAX_NUMBER);

	int number_attempt = MAX_ATTEMPTS;
	int r_number = MAX_NUMBER;
	int l_number = 1;
	int number;
	int prompt = -1;

	do {
		calculate_new_number(prompt, &l_number, &r_number, &number);
		if (r_number - l_number == 1) {
			printf("\nТы что забыл число?");
			return 0;
		}
		printf("Кол-во оставшихся попыток: %d\n", number_attempt);
		if (--number_attempt == -1) {
			printf("\nУпс, я проиграл, количество попыток исчерпано");
			return 0;
		}

		printf("\nВаше число: %d?\n", number);
		prompt = get_input_prompt();
		if (prompt == -1)
			return 0;
		else if (prompt == 1) {
			printf("\nУра, я выиграл!");
		}
	} while (prompt != 1);

	return 0;
}
