#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 60
#define MAX_REPEATED_LETTERS 500 // Maybe change to using malloc, but this is faster
#define GROUP_SIZE 3

#define MAX_REPEATED_CHARS 60

uint8_t findRepetedCharacters(char* str1, char* str2, char* repeaterCharacters);
bool isCharInArray(char* array, int size, char c);
char findIdentifierCharacter(char lines[][MAX_LINE_LENGTH], int groupSize);

int main() {

    FILE* fp;
    char lines[GROUP_SIZE][MAX_LINE_LENGTH];
    char repeatedLetters[MAX_REPEATED_LETTERS];
    int points = 0;

    char identifierChar = 0;

    memset(repeatedLetters, 0, MAX_REPEATED_LETTERS);
    memset(lines, 0, GROUP_SIZE * MAX_LINE_LENGTH);

    fp = fopen("sample.txt", "r");
    // I will asume I only get letters

    // Always 3 lines
    while (fgets(lines[0], sizeof(lines[0]), fp) != NULL) {
        lines[0][strlen(lines[0]) - 1] = '\0'; // Special treatment for first line

        // Always 3 lines
        for (int i = 1; i < GROUP_SIZE; i++) { // Always 3 lines
            fgets(lines[i], sizeof(lines[i]), fp);
            lines[i][strlen(lines[i]) - 1] = '\0';
        }

        identifierChar = findIdentifierCharacter(lines, GROUP_SIZE);

        printf("identifierChar: %c\n", identifierChar);

        if (identifierChar >= 'a') {
            points += identifierChar - 'a' + 1;
        }
        else {
            points += identifierChar - 'A' + 27;
        }
    }

    // printf("repeatedLetters[%d]: %c\n", i, repeatedLetters[i]);
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

char findIdentifierCharacter(char lines[][MAX_LINE_LENGTH], int groupSize) {

    char commonChar = 0;
    char* repeatedLetters = malloc(MAX_REPEATED_LETTERS * sizeof(char) * (groupSize - 1));
    char commonChars[MAX_REPEATED_LETTERS];

    memset(commonChars, 0, MAX_REPEATED_LETTERS);
    memset(repeatedLetters, 0, MAX_REPEATED_LETTERS * (groupSize - 1));

    if (repeatedLetters == NULL) {
        printf("Error! memory not allocated.");
        exit(-1);
    }

    for (int i = 0; i < groupSize; i++) {
        if (i + 1 < groupSize) {
            findRepetedCharacters(lines[i], lines[i + 1],
                                  repeatedLetters + MAX_REPEATED_LETTERS * i);
        }
    }

    for (int i = 0; i < groupSize - 2; i++) {
        int num =
            findRepetedCharacters(repeatedLetters + MAX_REPEATED_LETTERS * i,
                                  repeatedLetters + MAX_REPEATED_LETTERS * (i + 1), commonChars);
        memcpy(repeatedLetters + MAX_REPEATED_LETTERS * (i + 1), commonChars, MAX_REPEATED_LETTERS);
    }
    if (strlen(commonChars) == 1) {
        commonChar = commonChars[0];
    }
    else {
        printf("Error! commonChars is not 1 character long. with string: %s\n", commonChars);
    }

    free(repeatedLetters);

    return commonChar;
}
