#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_WIDTH 10
#define MAX_POSITIONS 10000
#define KNOTS_AMMOUNT 10

typedef struct {
    int x;
    int y;
} Pos;

void initPos(Pos* pos);
bool isPosInArray(Pos* pos, Pos* array, int arraySize);

int main() {

    FILE* fp;
    char line[MAX_LINE_WIDTH];
    char* token;
    Pos headPos;
    Pos tailPos;
    int posCount = 1;
    Pos occupiedPositions[MAX_POSITIONS]; // Liked the grid more :(
    Pos knots[KNOTS_AMMOUNT];

    int moveCount = 0;
    int distX = 0;
    int distY = 0;

    memset(occupiedPositions, 0, sizeof(occupiedPositions));

    initPos(&headPos);
    initPos(&tailPos);
    for (int i = 0; i < KNOTS_AMMOUNT; i++) {
        initPos(&knots[i]);
    }

    fp = fopen("sample.txt", "r");

    while (fgets(line, MAX_LINE_WIDTH, fp) != NULL) {
        if (line[strlen(line) - 1] == '\n') {
            line[strlen(line) - 1] = '\0';
        }
        token = strtok(line, " ");
        token = strtok(NULL, " ");

        moveCount = atoi(token);

        for (int j = 0; j < moveCount; j++) {
            switch (line[0]) {
                case 'U':
                    knots[0].y += 1;
                    break;
                case 'D':
                    knots[0].y -= 1;
                    break;
                case 'L':
                    knots[0].x -= 1;
                    break;
                case 'R':
                    knots[0].x += 1;
                    break;
            }

            for (int i = 1; i < KNOTS_AMMOUNT; i++) {

                distX = abs(knots[i - 1].x - knots[i].x);
                distY = abs(knots[i - 1].y - knots[i].y);

                while (distX > 1 || distY > 1) {
                    if (knots[i - 1].x > knots[i].x) {
                        knots[i].x++;
                    }
                    else if (knots[i - 1].x < knots[i].x) {
                        knots[i].x--;
                    }
                    if (knots[i - 1].y > knots[i].y) {
                        knots[i].y++;
                    }
                    else if (knots[i - 1].y < knots[i].y) {
                        knots[i].y--;
                    }

                    distX = abs(knots[i - 1].x - knots[i].x);
                    distY = abs(knots[i - 1].y - knots[i].y);
                }
                if (!isPosInArray(&knots[KNOTS_AMMOUNT - 1], occupiedPositions, posCount)) {
                    occupiedPositions[posCount] = knots[KNOTS_AMMOUNT - 1];
                    posCount++;
                }
            }
        }
    }

    fclose(fp);

    printf("headPos: %d, %d\n", knots[0].x, knots[0].y);
    printf("tailPos: %d, %d\n", knots[KNOTS_AMMOUNT - 1].x, knots[KNOTS_AMMOUNT - 1].y);

    printf("posCount: %d\n", posCount);

    return 0;
}

void initPos(Pos* pos) {
    pos->x = 0;
    pos->y = 0;
}

bool isPosInArray(Pos* pos, Pos* array, int arraySize) {
    for (int i = 0; i < arraySize; i++) {
        if (pos->x == array[i].x && pos->y == array[i].y) {
            return true;
        }
    }
    return false;
}
