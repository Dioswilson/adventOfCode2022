#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFF_SIZE 8
#define PODIUM_SIZE 3

int findMin(int* arr, int size);

int main() {

    FILE* fp;
    char buff[BUFF_SIZE];
    int calories = 0;
    int caloriesSum = 0;
    int totalCalories = 0;
    int maxCalories[PODIUM_SIZE];
    int maxSum = 0;

    char* fgetsRet = NULL;

    int pos = 0;
    int minPos = 0;
    memset(maxCalories, 0, sizeof(maxCalories));
    memset(buff, 0, sizeof(buff));

    fp = fopen("sample01.txt", "r");
    // fp = fopen("example01.txt", "r");

    do {
        fgetsRet = fgets(buff, BUFF_SIZE, fp);
        calories = atoi(buff);
        caloriesSum = (caloriesSum + calories) * (bool)calories;

        if (caloriesSum && fgetsRet != 0) {
            totalCalories = caloriesSum;
        }
        else {
            if (maxCalories[minPos] < totalCalories) {
                maxCalories[minPos] = totalCalories;
                minPos = findMin(maxCalories, PODIUM_SIZE);
            }

            printf("Total Calories: %d \n", totalCalories);
        }
    } while (fgetsRet != NULL);

    for (int j = 0; j < PODIUM_SIZE; j++) {
        printf("MAX[%d]: %d\n", j, maxCalories[j]);
        maxSum += maxCalories[j];
    }

    printf("MaxCalories: %d \n", maxSum);

    return EXIT_SUCCESS;
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
