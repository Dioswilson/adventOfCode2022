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

int getScoreFrom(char trees[][MAX_COLS], int row, int col, int rowCount, int colCount,
                 Direction dir);

int getScore(char trees[][MAX_COLS], int row, int col, int rowCount, int colCount);

int main() {

    int fd;
    char trees[MAX_ROWS][MAX_COLS];
    char buff = 0;
    int rowCount = 0;
    int tempColCount = 0;
    int colCount = 0;
    int score = 0;

    int maxScore = 0;

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
            score = getScore(trees, i, j, rowCount, colCount);
            if (maxScore < score) {
                maxScore = score;
            }
        }
    }

    printf("MaxScore: %d\n", maxScore);

    return 0;
}

int getScoreFrom(char trees[][MAX_COLS], int row, int col, int rowCount, int colCount,
                 Direction dir) {
    int maxIter;
    int rowMultiplier = 0;
    int colMultiplier = 0;
    int compareRow;
    int compareCol;

    int score = 1;

    switch (dir) {
        case NORTH:
            maxIter = row;
            rowMultiplier = -1;
            // colMultiplier = 0;
            break;
        case SOUTH:
            maxIter = rowCount - row;
            rowMultiplier = 1;
            // colMultiplier = 0;
            break;
        case EAST:
            maxIter = colCount - col;
            // rowMultiplier = 0;
            colMultiplier = 1;
            break;
        case WEST:
            maxIter = col;
            // rowMultiplier = 0;
            colMultiplier = -1;
            break;
    }

    for (int i = 1; i < maxIter; i++) {

        compareRow = row + i * rowMultiplier;
        compareCol = col + i * colMultiplier;

        if (trees[row][col] <= trees[compareRow][compareCol]) {
            break;
        }
        if ((compareCol < colCount - 1) && (compareRow < rowCount - 1)) {
            score++;
        }
    }
    // printf("visible  [%d][%d]  in direction %d\n", row, col, dir);

    return score;
}

int getScore(char trees[][MAX_COLS], int row, int col, int rowCount, int colCount) {

    int northScore = getScoreFrom(trees, row, col, rowCount, colCount, NORTH);

    int southScore = getScoreFrom(trees, row, col, rowCount, colCount, SOUTH);
    int eastScore = getScoreFrom(trees, row, col, rowCount, colCount, EAST);
    int westScore = getScoreFrom(trees, row, col, rowCount, colCount, WEST);
    return northScore * southScore * eastScore * westScore;
}
