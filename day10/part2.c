#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_WIDTH 30

#define CYCLE_PERIOD 40
#define CYCLE_START 20

#define CRT_HEIGHT 6
#define CRT_WIDTH 40

void updateCrt(char crtScreen[CRT_HEIGHT][CRT_WIDTH], int xReg, int cycle);

int main() {
    FILE* fp;
    char line[MAX_LINE_WIDTH];
    char* token;
    int cycleCount = 0;
    int xValue = 1;
    int addValue = 0;
    int periodCount = 0;

    char crtScreen[CRT_HEIGHT][CRT_WIDTH];
    int crtRow = 0;
    int crtCol = 0;

    int totalSS = 0;

    memset(crtScreen, '.', CRT_HEIGHT * CRT_WIDTH);

    fp = fopen("sample.txt", "r");

    while (fgets(line, MAX_LINE_WIDTH, fp) != NULL) {
        if (line[strlen(line) - 1] == '\n') {
            line[strlen(line) - 1] = '\0';
        }

        token = strtok(line, " ");
        token = strtok(NULL, " ");

        updateCrt(crtScreen, xValue, cycleCount % (CRT_HEIGHT * CRT_WIDTH));

        cycleCount++;

        if (cycleCount % (CYCLE_START + CYCLE_PERIOD * periodCount) == 0) {
            totalSS += xValue * cycleCount;
            periodCount++;
        }

        if (token != NULL) {
            addValue = atoi(token);

            updateCrt(crtScreen, xValue, cycleCount % (CRT_HEIGHT * CRT_WIDTH));
            cycleCount++;

            if (cycleCount % (CYCLE_START + CYCLE_PERIOD * periodCount) == 0) {
                totalSS += xValue * cycleCount;
                periodCount++;
            }


            xValue += addValue;
        }

    }

    fclose(fp);

    for (crtRow = 0; crtRow < CRT_HEIGHT; crtRow++) {
        for (crtCol = 0; crtCol < CRT_WIDTH; crtCol++) {
            printf("%c", crtScreen[crtRow][crtCol]);
        }
        printf("\n");
    }

    printf("Total SS: %d\n", totalSS);
}

void updateCrt(char crtScreen[CRT_HEIGHT][CRT_WIDTH], int xReg, int cycle) {
    int row = cycle / CRT_WIDTH;
    int col = cycle % CRT_WIDTH;

    if (row >= CRT_HEIGHT) {
        printf("CRT overflow! cycle=%d\n", cycle);
        return;
    }
    if ((xReg - 1) <= col && col <= (xReg + 1)) {
        crtScreen[row][col] = '#';
    }
    else {
        crtScreen[row][col] = '.';
    }
}
