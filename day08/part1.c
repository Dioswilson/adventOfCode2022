#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define MAX_ROWS 101
#define MAX_COLS 101

typedef enum {
		NORTH,
		SOUTH,
		EAST,
		WEST,
} Direction;

bool isVisibleFrom(char trees[][MAX_COLS], int row, int col, int rowCount, int colCount,
				   Direction dir);

bool isVisible(char trees[][MAX_COLS], int row, int col, int rowCount, int colCount);

int main() {

	int fd;
	char trees[MAX_ROWS][MAX_COLS];
	char buff = 0;
	int rowCount = 0;
	int tempColCount = 0;
	int colCount = 0;

	int count = 0;

	memset(trees, 0, sizeof(trees));

	fd = open("sample.txt", O_RDONLY);

	while (read(fd, &buff, 1) != 0) {
		if (buff != '\n') {
			trees[rowCount][tempColCount] = buff;
			tempColCount++;
		}
		else {
			rowCount++;
			if (tempColCount > colCount) {
				colCount = tempColCount;
			}
			tempColCount = 0;
		}
	}

	for (int i = 1; i < rowCount - 1; i++) {
		for (int j = 1; j < colCount - 1; j++) {
			// printf("%c[%d][%d]\n", trees[i][j], i, j);
			if (isVisible(trees, i, j, rowCount, colCount)) {
				count++;
			}
		}
	}

	count+=((rowCount-1)*2+(colCount-1)*2);

	printf("count: %d\n", count);

	return 0;
}

bool isVisibleFrom(char trees[][MAX_COLS], int row, int col, int rowCount, int colCount,
				   Direction dir) {
	int iterInit;
	int maxIter;
	int i;
	int *rowCompare;
	int *colCompare;

	switch (dir) {
		case NORTH:
			iterInit = 0;
			maxIter = row;
			rowCompare = &i;
			colCompare = &col;
			break;
		case SOUTH:
			iterInit = row + 1;
			maxIter = rowCount;
			rowCompare = &i;
			colCompare = &col;
			break;
		case EAST:
			iterInit = 0;
			maxIter = col;
			rowCompare = &row;
			colCompare = &i;
			break;
		case WEST:
			iterInit = col + 1;
			maxIter = colCount;
			rowCompare = &row;
			colCompare = &i;
			break;
	}

	for (i = iterInit; i < maxIter; i++) {
		if (trees[row][col] <= trees[*rowCompare][*colCompare]) {
			return false;
		}
	}
	// printf("visible  [%d][%d]  in direction %d\n", row, col, dir);

	return true;
}

bool isVisible(char trees[][MAX_COLS], int row, int col, int rowCount, int colCount) {
	return isVisibleFrom(trees, row, col, rowCount, colCount, NORTH) ||
		   isVisibleFrom(trees, row, col, rowCount, colCount, SOUTH) ||
		   isVisibleFrom(trees, row, col, rowCount, colCount, EAST) ||
		   isVisibleFrom(trees, row, col, rowCount, colCount, WEST);
}
