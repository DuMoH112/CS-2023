#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Размеры лабиринта - желательно нечётные числа (начиная от 5)
#define SIZE_Y 13
#define SIZE_X 43
#define WALL_CHAR '*'
#define DOOR_CHAR ' '
#define SEED 15485863

// Структура для представления клетки лабиринта
typedef struct {
	int8_t wall;
} Cell;

int getRandomPointOnBorder(int minValue, int maxValue) {
	int random_value = rand() % 2; // NOLINT(cert-msc50-cpp)
	return random_value == 1 ? maxValue : minValue;
}

int getRandom(int start, int stop, int step) {
	int random_value = (rand() % ((++stop - start) / step)) * step + start; // NOLINT(cert-msc50-cpp)
	return random_value;
}

int getMax(int num1, int num2) {
	return num1 > num2 ? num1 : num2;
}

void grow_branch(Cell maze[SIZE_Y][SIZE_X], int y, int x) {
	while (1) {
		int directions[4][2] = {0};
		int count = 0;

		if (y < SIZE_Y - 2 && maze[y + 2][x].wall == 0) {
			directions[count][0] = 1;
			directions[count][1] = 0;
			count++;
		}
		if (y > 0 && maze[y - 2][x].wall == 0) {
			directions[count][0] = -1;
			directions[count][1] = 0;
			count++;
		}
		if (x < SIZE_X - 2 && maze[y][x + 2].wall == 0) {
			directions[count][0] = 0;
			directions[count][1] = 1;
			count++;
		}
		if (x > 0 && maze[x][y - 2].wall == 0) {
			directions[count][0] = 0;
			directions[count][1] = -1;
			count++;
		}
		if (count == 0) {
			break;
		}
		int index = getRandom(0, count, 1);
		int delta_y = directions[index][0];
		int delta_x = directions[index][1];

		for (int k = 0; k < 2; k++) {
			maze[y][x].wall = 1;
			y += delta_y;
			x += delta_x;
		}
	}
	maze[y][x].wall = 1;
}

void generateMaze(Cell maze[SIZE_Y][SIZE_X]) {
	int half_x = (SIZE_X + 1) / 2;
	int half_y = (SIZE_Y + 1) / 2;

	// случайные ответвления от границ
	grow_branch(maze, 0, getRandom(2, SIZE_X - 1, 2));
	grow_branch(maze, SIZE_Y - 1, getRandom(2, SIZE_X - 1, 2));
	grow_branch(maze, getRandom(2, SIZE_Y - 1, 2), 0);
	grow_branch(maze, getRandom(2, SIZE_Y - 1, 2), SIZE_X - 1);

	// заполнение середины ветвями
	for (int k = 0; k < getMax(half_y, half_x); k++) {
		int y = getRandom(0, SIZE_Y, 2);
		int x = getRandom(0, SIZE_X, 2);
		grow_branch(maze, y, x);
	}

	// заполнение оставшихся свободных комнат
	int k = 0;
	for (int iter = 0; iter < half_x * half_y; iter++) {
		k = (k + SEED) % (half_x * half_y);
		int y = 2 * (k / half_y);
		int x = 2 * (k % half_y);
		if (maze[y][x].wall == 1) {
			grow_branch(maze, y, x);
		}
	}

}


void initMaze(Cell maze[SIZE_Y][SIZE_X]) {
	for (int y = 0; y < SIZE_Y; y++) {
		for (int x = 0; x < SIZE_X; x++) {
			maze[y][x].wall = (x == 0 || y == 0 || x == SIZE_X - 1 || y == SIZE_Y - 1) ? 1 : 0;
		}
	}
}

void createEntryExit(Cell maze[SIZE_Y][SIZE_X]) {
	int seed = getRandom(0, 1, 1);
	int R_X = SIZE_X - 1;
	int R_Y = SIZE_Y - 1;
	int entryX = seed == 0 ? getRandom(1, SIZE_X - 2, 1) : getRandomPointOnBorder(0, R_X);
	int entryY = seed == 1 ? getRandom(1, SIZE_Y - 2, 1) : getRandomPointOnBorder(0, R_Y);
	int exitX;
	int exitY;

	if (entryX == 0)
		exitX = R_X;
	else if (entryX == R_X)
		exitX = 0;
	else
		exitX = getRandom(1, SIZE_X - 2, 1);

	if (entryY == 0)
		exitY = R_Y;
	else if (entryY == R_Y)
		exitY = 0;
	else
		exitY = getRandom(1, SIZE_Y - 2, 1);
	
	maze[entryY][entryX].wall = 2;
	maze[exitY][exitX].wall = 2;
}

void drawMaze(Cell maze[SIZE_Y][SIZE_X]) {
	for (int y = 0; y < SIZE_Y; y++) {
		for (int x = 0; x < SIZE_X; x++) {
			if (maze[y][x].wall == 0)
				printf("  ");
			else if (maze[y][x].wall == 1)
				printf("%c ", WALL_CHAR);
			else if (maze[y][x].wall == 2)
				printf("%c ", DOOR_CHAR);
		}
		printf("\n");
	}
}

int main() {
	srand(time(NULL)); // NOLINT(cert-msc51-cpp)
	Cell maze[SIZE_Y][SIZE_X];

	initMaze(maze);
	generateMaze(maze);
	createEntryExit(maze);
	drawMaze(maze);

	return 0;
}