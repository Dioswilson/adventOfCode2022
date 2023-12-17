#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_WIDTH 30

#define CYCLE_PERIOD 40
#define CYCLE_START 20

int main() {
    FILE* fp;
    char line[MAX_LINE_WIDTH];
    char* token;
    int cycleCount = 0;
    int xValue = 1;
    int addValue = 0;
    int periodCount = 0;

    int totalSS = 0;

    fp = fopen("sample.txt", "r");

    while (fgets(line, MAX_LINE_WIDTH, fp) != NULL) {
        if (line[strlen(line) - 1] == '\n') {
            line[strlen(line) - 1] = '\0';
        }

        token = strtok(line, " ");
        token = strtok(NULL, " ");

        cycleCount++;

        if (cycleCount % (CYCLE_START + CYCLE_PERIOD * periodCount) == 0) {
            totalSS += xValue * cycleCount;
            periodCount++;
        }

        if (token != NULL) {
            addValue = atoi(token);

            cycleCount++;
            if (cycleCount % (CYCLE_START + CYCLE_PERIOD * periodCount) == 0) {
                totalSS += xValue * cycleCount;
                periodCount++;
            }

            xValue += addValue;
        }
    }

    fclose(fp);

    printf("Total SS: %d\n", totalSS);
}
