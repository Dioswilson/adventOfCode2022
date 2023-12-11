#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 20
#define PAIR_SIZE 2

int main() {
    FILE* fp;
    char line[MAX_LINE_LENGTH];
    char* elvesPair[MAX_LINE_LENGTH];
    char originalLine[MAX_LINE_LENGTH];

    int begin[PAIR_SIZE];
    int end[PAIR_SIZE];
    int overlappingCount = 0;
    bool alreadyOverlapping = false;

    memset(elvesPair, 0, MAX_LINE_LENGTH);
    memset(begin, 0, sizeof(begin));
    memset(end, 0, sizeof(end));

    fp = fopen("sample.txt", "r");

    while (fgets(line, sizeof(line), fp) != NULL) {

        line[strlen(line) - 1] = '\0';
        strcpy(originalLine, line);
        // HARDCODED AF
        elvesPair[0] = strtok(line, ",");
        for (int i = 1; i < PAIR_SIZE; i++) {
            elvesPair[i] = strtok(NULL, ",");
        }
        // printf("First Elf: %s\n", elvesPair[0]);
        // printf("Second Elf: %s\n", elvesPair[1]);

        for (int i = 0; i < PAIR_SIZE; i++) {
            begin[i] = atoi(strtok(elvesPair[i], "-"));
            end[i] = atoi(strtok(NULL, "-"));
        }

        alreadyOverlapping = false;
        for (int i = 0; i < PAIR_SIZE; i++) {
            for (int j = 0; j < PAIR_SIZE; j++) {
                if (i != j) {
                    if (begin[i] >= begin[j] && end[i] <= end[j] && !alreadyOverlapping) {
                        overlappingCount++;
                        alreadyOverlapping = true;
                        // printf("Overlapping: %s\n", originalLine);
                    }
                }
            }
        }
    }

    printf("Overlapping Count: %d\n", overlappingCount);

    fclose(fp);
}
