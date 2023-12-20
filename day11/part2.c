#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ITEMS 200
#define MAX_MONKEYS 10
#define MAX_LINE_LENGTH 100
#define OPERATOR_INDEX 23

#define CYCLES_TO_RUN 10000
// #define CYCLES_TO_RUN 1000


#define MONKEYS_TO_FOLLOW 2

typedef enum {
    MULT = '*',
    ADD = '+',
    SQARE = '^',
} OperationType;

typedef struct {
    OperationType type;
    int value;
} Operation;

typedef struct {
    int divisor;
    int numberIfTrue;
    int numberIfFalse;
} Test;

typedef struct {
    uint64_t items[MAX_ITEMS];
    int itemCount;
    Operation operation;
    Test test;
    int inspectedItemsCount;
} Monkey;

int initMonkeysFromFile(char* fileName, Monkey* monkeys);

void printMonkeys(Monkey* monkey);

int findMin(int* arr, int size);

int getMonkeysMCM(Monkey* monkeys, int monkeyIndex);

int main() {
    Monkey monkeys[MAX_MONKEYS];
    int monkeyIndex = 0;
    uint64_t item = 0;
    Monkey tempMonkey;
    int nextMonkeyIndex = 0;
    int monkeyInspections[MONKEYS_TO_FOLLOW];
    int minIndex = 0;
    uint64_t monkeyBusiness = 1;
    uint64_t superModulus = 1;//Hate you

    memset(monkeys, 0, sizeof(monkeys));

    monkeyIndex = initMonkeysFromFile("sample.txt", monkeys);

    superModulus = getMonkeysMCM(monkeys, monkeyIndex);
    

    for (int i1 = 0; i1 < CYCLES_TO_RUN; i1++) {
        for (int i = 0; i <= monkeyIndex; i++) {
            //			for (int j = 0; j < MAX_ITEMS; j++) {
            while (monkeys[i].itemCount > 0) {
                if (monkeys[i].items[0] == 0) {
                    break;
                }
                switch (monkeys[i].operation.type) {
                    case MULT:
                        monkeys[i].items[0] *= monkeys[i].operation.value;
                        break;
                    case ADD:
                        monkeys[i].items[0] += monkeys[i].operation.value;
                        break;
                    case SQARE:
                        monkeys[i].items[0] = monkeys[i].items[0] * monkeys[i].items[0];
                        break;
                    default:
                        printf("Invalid operation type\n");
                        break;
                }
                // gets bored
                // monkeys[i].items[0] /= 3;
                monkeys[i].items[0] %= superModulus;

                // test
                if ((monkeys[i].items[0] % monkeys[i].test.divisor) == 0) {
                    nextMonkeyIndex = monkeys[i].test.numberIfTrue;
                }
                else {
                    nextMonkeyIndex = monkeys[i].test.numberIfFalse;
                }
                tempMonkey = monkeys[nextMonkeyIndex];
                monkeys[nextMonkeyIndex].items[monkeys[nextMonkeyIndex].itemCount++] =
                    monkeys[i].items[0];

                memmove(monkeys[i].items, monkeys[i].items + 1,
                        sizeof(monkeys[i].items[0]) * monkeys[i].itemCount--);

                monkeys[i].inspectedItemsCount++;
            }
        }
    }

    for (int i = 0; i <= monkeyIndex; i++) {
        minIndex = findMin(monkeyInspections, MONKEYS_TO_FOLLOW);

        if (monkeys[i].inspectedItemsCount > monkeyInspections[minIndex]) {
            monkeyInspections[minIndex] = monkeys[i].inspectedItemsCount;
        }
        printf("Monkey %d: %d\n", i, monkeys[i].inspectedItemsCount);
    }

    for (int i = 0; i < MONKEYS_TO_FOLLOW; i++) {
        monkeyBusiness *= monkeyInspections[i];
    }
    printf("Monkey business: %lu\n", monkeyBusiness);

    return 0;
}

int initMonkeysFromFile(char* fileName, Monkey* monkeys) {
    FILE* fp;
    char line[MAX_LINE_LENGTH];
    int monkeyIndex = 0;
    char* token;
    char* searchToken;
    int itemsIndex = 0;
    int tempNumber = 0;

    fp = fopen(fileName, "r");

    while (fgets(line, MAX_LINE_LENGTH, fp) != NULL) {
        if (strcmp(line, "\n") == 0) {
            itemsIndex = 0;
            monkeyIndex++;
            continue;
        }
        if (line[strlen(line) - 1] == '\n') {
            line[strlen(line) - 1] = '\0';
        }
        token = strtok(line, " ");
        tempNumber = 0;
        if (strcmp(token, "Monkey") == 0) {
            // Note: could use this as monkey ID, but using array index instead
        }
        else if (strcmp(token, "Starting") == 0) {
            memset(monkeys[monkeyIndex].items, 0, sizeof(monkeys[monkeyIndex].items));
            while (token != NULL) {
                if (atoi(token) != 0) {
                    monkeys[monkeyIndex].items[itemsIndex++] = atoi(token);
                    monkeys[monkeyIndex].itemCount++;
                }
                token = strtok(NULL, " ");
            }
        }
        else if (strcmp(token, "Operation:") == 0) {
            // Doing it this way is not too flexible, but it is easy to implement
            monkeys[monkeyIndex].operation.type = line[OPERATOR_INDEX];
            tempNumber = atoi(&line[OPERATOR_INDEX + 2]);
            if (tempNumber == 0) {
                if (strcmp(&line[OPERATOR_INDEX + 2], "old") == 0) {
                    monkeys[monkeyIndex].operation.type = SQARE;
                }
                else {
                    printf("Invalid operation value\n");
                }
            }
            monkeys[monkeyIndex].operation.value = tempNumber;
        }
        else if (strcmp(token, "Test:") == 0) {
            searchToken = token;
            while (searchToken != NULL && tempNumber == 0) {
                tempNumber = atoi(searchToken);
                searchToken = strtok(NULL, " ");
            }
            monkeys[monkeyIndex].test.divisor = tempNumber;
        }
        else if (strcmp(token, "If") == 0) {
            // Doing it this way is not too flexible, but it is easy to implement
            token = strtok(NULL, " ");
            searchToken = token;
            while (searchToken != NULL && tempNumber == 0) {
                tempNumber = atoi(searchToken);
                searchToken = strtok(NULL, " ");
            }

            if (strcmp(token, "true:") == 0) {
                monkeys[monkeyIndex].test.numberIfTrue = tempNumber;
            }
            else {
                monkeys[monkeyIndex].test.numberIfFalse = tempNumber;
            }
        }
    }
    return monkeyIndex;
}

void printMonkeys(Monkey* monkey) {
    printf("Starting items: ");
    for (int i = 0; i < MAX_ITEMS; i++) {
        if (monkey->items[i] != 0) {
            printf("%d, ", monkey->items[i]);
        }
    }
    printf("\nOperation: %c\n", monkey->operation.type);
    printf("Value: %d\n", monkey->operation.value);
    printf("Test: %d\n", monkey->test.divisor);
    printf("If true: %d\n", monkey->test.numberIfTrue);
    printf("If false: %d\n", monkey->test.numberIfFalse);
}

int findMin(int* arr, int size) {
    int min = arr[0];
    int minPos = 0;
    for (int i = 1; i < size; i++) {
        if (arr[i] < min) {
            min = arr[i];
            minPos = i;
        }
    }
    return minPos;
}

int getMonkeysMCM(Monkey* monkeys, int monkeyIndex) {
    int mcm = 1;

    for (int i = 0; i <= monkeyIndex; i++) {
        mcm *= monkeys[i].test.divisor; // Note: They are all primes, so this is enough
    }

    return mcm;
}
