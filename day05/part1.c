#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stack.h"

#define MAX_LINE_LENGTH 50
#define MAX_STACK_QUANTITY 10

typedef struct {
    uint8_t id;
    char column;
} Identifier;


int setupIndentifiers(Identifier* identifiers, char* buf, FILE* fp, int* bufLen);

void loadStacks(Stack* stacks, Identifier* identifiers, int identierCount, char* buffer,
                int iterCount, FILE* fp);

void parseMoves(Stack* stacks, char* buff, FILE* fp);

int main() { 

    FILE* fp;
    char line[MAX_LINE_LENGTH];
    Stack stacks[MAX_STACK_QUANTITY];
    Identifier identifiers[MAX_STACK_QUANTITY];
    int graphHeight = 0;
    int traversedLines = 0;
    int lineLen = 0;
    char lastChar = 0;

    memset(identifiers, 0, sizeof(identifiers));

    for (int i = 0; i < sizeof(stacks) / sizeof(Stack); i++) {
        initStack(&stacks[i]);
    }

    fp = fopen("sample.txt", "r");


    graphHeight = setupIndentifiers(identifiers, line, fp, &lineLen);

    // Go back
    fseek(fp, -lineLen, SEEK_CUR);
    fseek(fp, -lineLen, SEEK_CUR);

    traversedLines = graphHeight - 1;

    loadStacks(stacks, identifiers, sizeof(identifiers) / sizeof(Identifier), line, traversedLines,
               fp);

    // Skip lines
    for (int i = 0; i < graphHeight + 1; i++) {
        fgets(line, MAX_LINE_LENGTH, fp);
    }

    parseMoves(stacks, line, fp);

    fclose(fp);

    for (int i = 0; i < sizeof(stacks) / sizeof(Stack); i++) {
        // printStack(&stacks[i]);
        lastChar = getLast(&stacks[i]);
        if (lastChar != -1) {
            printf("%c", lastChar);
        }
        freeStack(&stacks[i]);
    }
    printf("\n");

    return 0;
}



/**Sets up the identifiers and returns the graph height and last line's length*/
int setupIndentifiers(Identifier* identifiers, char* buf, FILE* fp, int* bufLen) {
    bool searchIdentifiers = true;
    char* token;
    int graphHeight = 0;

    while (searchIdentifiers) {
        fgets(buf, MAX_LINE_LENGTH, fp);
        *bufLen = strlen(buf);
        buf[*bufLen - 1] = '\0';
        graphHeight++;

        if (atoi(buf) != 0) {
            searchIdentifiers = false;
            token = strtok(buf, " ");
            int i = 0;
            while (token) {
                identifiers[i].id = atoi(token);
                identifiers[i].column = token - buf;
                token = strtok(NULL, " ");
                i++;
            }
            // fgetpos(fp, &graphEndPos);
            // rewind(fp);
        }
    }
    return graphHeight;
}

/**Loads stacks from the file with the corresponding identifiers*/
void loadStacks(Stack* stacks, Identifier* identifiers, int identifierCount, char* buffer,
                int iterCount, FILE* fp) {
    char* token;
    int bufferLen = 0;

    while (iterCount) {
        fgets(buffer, MAX_LINE_LENGTH, fp);
        bufferLen = strlen(buffer);

        fseek(fp, -bufferLen, SEEK_CUR);
        fseek(fp, -bufferLen, SEEK_CUR);

        buffer[bufferLen - 1] = '\0';
        iterCount--;

        // printf("buffer: %s\n", buffer);
        token = strtok(buffer, "[");

        // Load into the stacks
        while (token) {
            if (token[0] != ' ') {
                // printf("TOKEN:%s\n", token);
                for (int i = 0; i < identifierCount; i++) {
                    if (identifiers[i].column == (token - buffer)) {
                        // printf("%d | %c|\n", identifiers[i].id, token[0]);
                        // printStack(&(stacks[identifiers[i].id - 1]));
                        push(&(stacks[identifiers[i].id - 1]), token[0]);
                        break;
                    }
                }
            }
            token = strtok(NULL, "[");
        }
    }
}

void parseMoves(Stack* stacks, char* buff, FILE* fp) {
    char* token;
    int buffLen = 0;

    int source = 0;
    int destination = 0;
    int count = 0;

    while (fgets(buff, MAX_LINE_LENGTH, fp) != NULL) {
        buffLen = strlen(buff);
        buff[strlen(buff) - 1] = '\0';

        // printf("%s\n", buff);
        token = strtok(buff, "move ");
        count = atoi(token);

        token = strtok(NULL, "from ");
        source = atoi(token);

        token = strtok(NULL, "to ");
        destination = atoi(token);

        for (int i = 0; i < count; i++) {
            // printf("Moving %d from %d to %d\n", count, source, destination);
            char data = pop(&(stacks[source - 1]));
            push(&(stacks[destination - 1]), data);
        }
    }
}
