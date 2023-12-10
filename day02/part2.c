#include <stdbool.h>
#include <stdio.h>

#define MAX_LINE_LENGTH 5

#define WIN_POINTS 6
#define TIE_POINTS 3
#define LOSE_POINTS 0

typedef enum {
    ROCK = 'A',
    PAPER,
    SCISSORS,
} GameOptions;

typedef enum {
    ROCK_POINTS = 1,
    PAPER_POINTS,
    SCISSORS_POINTS,
} ElectionPoints;

typedef enum {
    LOSE = 'X',
    TIE,
    WIN,
} GameResults;

int main() {

    FILE* fp;
    char line[MAX_LINE_LENGTH];
    GameOptions enemyMove = 0;
    GameResults decision = 0;
    int totalPoints = 0;
    ElectionPoints playerPoints = 0;

    fp = fopen("sample.txt", "r"); // Wont check if it opened

    while (fgets(line, sizeof(line), fp) != NULL) {
        enemyMove = line[0];
        decision = line[2];

        playerPoints = enemyMove - (ROCK) + 1;

        switch (decision) {
            case WIN:
                totalPoints += WIN_POINTS;
                if (enemyMove != SCISSORS) {
                    playerPoints += 1;
                }
                else {
                    playerPoints = ROCK_POINTS;
                }
                break;
            case LOSE:
                totalPoints += LOSE_POINTS;
                if (enemyMove != ROCK) {
                    playerPoints -= 1;
                }
                else {
                    playerPoints = SCISSORS_POINTS;
                }
                break;
            case TIE:
                totalPoints += TIE_POINTS;

                break;
        }
        // printf("PlayerPoints: %d\n", playerPoints);

        totalPoints += playerPoints;
    }
    printf("Total points: %d\n", totalPoints);

    fclose(fp);

    return 0;
}
