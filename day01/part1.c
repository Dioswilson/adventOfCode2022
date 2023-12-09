#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFF_SIZE 8

int main() {

    FILE* fp;
    char buff[BUFF_SIZE];
    int calories = 0;
    int caloriesSum = 0;
    int totalCalories = 0;
    int maxCalories=0;

    char* fgetsRet = NULL;

    fp = fopen("sample01.txt", "r"); //Wont if it opened
    // fp = fopen("example01.txt", "r"); //Wont if it opened

    do {
        fgetsRet = fgets(buff, BUFF_SIZE, fp);
        calories = atoi(buff);
        caloriesSum = (caloriesSum + calories) * (bool)calories;

        if (caloriesSum && fgetsRet != 0) {
            totalCalories = caloriesSum;
        }
        else {

            if (maxCalories < totalCalories) {
                maxCalories = totalCalories;
            }

            printf("Total Calories: %d \n", totalCalories);
        }
    } while (fgetsRet != NULL);

    printf("MaxCalories: %d \n", maxCalories);

    return EXIT_SUCCESS;
}
