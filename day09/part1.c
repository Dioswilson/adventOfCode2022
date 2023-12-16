#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_WIDTH 10
#define MAX_POSITIONS 10000

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

    int moveCount = 0;
    int distX = 0;
    int distY = 0;

    memset(occupiedPositions, 0, sizeof(occupiedPositions));

    initPos(&headPos);
    initPos(&tailPos);

    fp = fopen("sample.txt", "r");

    while (fgets(line, MAX_LINE_WIDTH, fp) != NULL) {
        if (line[strlen(line) - 1] == '\n') {
            line[strlen(line) - 1] = '\0';
        } 
        token = strtok(line, " ");
        token = strtok(NULL, " ");

        moveCount = atoi(token);

        switch (line[0]) {
            case 'U':
                headPos.y += moveCount;
                break;
            case 'D':
                headPos.y -= moveCount;
                break;
            case 'L':
                headPos.x -= moveCount;
                break;
            case 'R':
                headPos.x += moveCount;
                break;
        }

        distX = abs(headPos.x - tailPos.x);
        distY = abs(headPos.y - tailPos.y);

        while (distX > 1 || distY > 1) {
            if (headPos.x > tailPos.x) {
                tailPos.x++;
            }
            else if (headPos.x < tailPos.x) {
                tailPos.x--;
            }
            if (headPos.y > tailPos.y) {
                tailPos.y++;
            }
            else if (headPos.y < tailPos.y) {
                tailPos.y--;
            }

            distX = abs(headPos.x - tailPos.x);
            distY = abs(headPos.y - tailPos.y);

            if (!isPosInArray(&tailPos, occupiedPositions, posCount)) {
                occupiedPositions[posCount] = tailPos;
                posCount++;
            }
        }
    }

    fclose(fp);

    printf("headPos: %d, %d\n", headPos.x, headPos.y);
    printf("tailPos: %d, %d\n", tailPos.x, tailPos.y);

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
