#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define MAX_LINE_LENGTH 50
#define MAX_REPEATED_LETTERS 400 //Maybe change to using malloc, but this is faster

uint8_t findRepetedCharacters(char* str1, char* str2, char* repeaterCharacters);
bool isCharInArray(char* array, int size, char c);

int main() {

    FILE* fp;
    char line[MAX_LINE_LENGTH];
    int halfIndex = 0;
    char secondHalf[MAX_LINE_LENGTH / 2];
    char repeatedLetters[MAX_REPEATED_LETTERS];
    int repeatedCount = 0;
    int points = 0;

    fp = fopen("sample.txt", "r");
    //I will asume I only get letters

    while (fgets(line, MAX_LINE_LENGTH, fp) != NULL) {

        halfIndex = strlen(line) / 2;
        strncpy(secondHalf, line + halfIndex, sizeof(secondHalf));

        line[halfIndex] = '\0';

        repeatedCount += findRepetedCharacters(line, secondHalf, repeatedLetters + repeatedCount);
    }

    printf("repetedCount: %d\n", repeatedCount);

    for (int i = 0; i < repeatedCount; i++) {
        if (repeatedLetters[i] >= 'a') {
            points += repeatedLetters[i] - 'a' + 1;
        }
        else {
            points += repeatedLetters[i] - 'A' + 27;
        }
        // printf("repeatedLetters[%d]: %c\n", i, repeatedLetters[i]);
    }
    printf("points: %d\n", points);

    fclose(fp);

    return 0;
}

uint8_t findRepetedCharacters(char* str1, char* str2, char* repeaterCharacters) {

    uint8_t arraySize = 0;
    char* ogStr2 = str2;
    char* ogRepeaterCharacters = repeaterCharacters;
    // NOTE: Could just save the position and not alter the original pointer

    while (*str1 != '\0') {

        while (*str2 != '\0') {
            if (*str1 == *str2) {
                if (isCharInArray(ogRepeaterCharacters, arraySize, *str2)) {
                    break;
                }
                *repeaterCharacters = *str1;
                arraySize++;
                repeaterCharacters++;
            }
            str2++;
        }

        str1++;
        str2 = ogStr2;
    }

    return arraySize;
}

bool isCharInArray(char* array, int size, char c) {
    bool isInArray = false;

    for (int i = 0; i < size; i++) {
        if (array[i] == c) {
            isInArray = true;
            break;
        }
    }
    return isInArray;
}
