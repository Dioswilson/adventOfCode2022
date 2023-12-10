#include <stdbool.h>
#include <stdio.h>

#define MAX_LINE_LENGTH 5

#define WIN_POINTS 6
#define TIE_POINTS 3
#define LOSE_POINTS 0

typedef enum {
    ROCK_ENEMY = 'A',
    PAPER_ENEMY,
    SCISSORS_ENEMY,

    ROCK_PLAYER = 'X',
    PAPER_PLAYER,
    SCISSORS_PLAYER,
} GameOptions;

int getOutcomePoints(GameOptions enemyMove, GameOptions playerMove);
int getElectionPoints(GameOptions playerMove);

int main() {
    FILE* fp;
    char line[MAX_LINE_LENGTH];
    GameOptions enemyMove = 0;
    GameOptions playerMove = 0;
    int totalPoints = 0;

    fp = fopen("sample.txt", "r"); // Wont check if it opened

    while (fgets(line, sizeof(line), fp) != NULL) {
        enemyMove = line[0];

        playerMove = line[2];
        totalPoints += getOutcomePoints(enemyMove, playerMove);
        totalPoints += getElectionPoints(playerMove);
    }
    printf("%d\n", totalPoints);

    fclose(fp);

    return 0;
}

int getOutcomePoints(GameOptions enemyMove, GameOptions playerMove) {
    GameOptions result = playerMove - enemyMove - (ROCK_PLAYER - ROCK_ENEMY);

    if (result == 1 || result == -2) {
        return WIN_POINTS;
    }
    else if (result == -1 || result == 2) {
        return LOSE_POINTS;
    }

    return TIE_POINTS;
}

int getElectionPoints(GameOptions playerMove) {
    int result = 0;

    switch (playerMove) {
        case ROCK_PLAYER:
            result = 1;
            break;
        case PAPER_PLAYER:
            result = 2;
            break;
        case SCISSORS_PLAYER:
            result = 3;
            break;
        default:
            // DO NOTHING
            break;
    }

    return result;
}
