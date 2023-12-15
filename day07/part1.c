#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 30
#define COMMAND_IDENTIFIER '$'
#define INIT_CHILDREN_COUNT 10
#define MAX_NODE_SIZE 250

typedef struct File {
    char* name;
    int size;
} File;

typedef struct Directory {
    char* name;
    struct Directory* parent;
    struct Directory** children; // ARRAY of directories
    int childrenCount;
    int childrenAlloc;
    File* files; // ARRAY of files
    int filesCount;
    int filesAlloc;
    int size;
} Directory;

void initFileSystemFromFile(Directory** root, const char* fileName);

void initDirectory(Directory* directory, const char* name, Directory* parent);

Directory* getDirectory(Directory* currentDir, char* name);

void initFile(File* file, char* name, int size);

void addFileToDirectory(Directory* directory, File* file);

bool arrayContains(Directory** arr, int size, Directory* dir);

int main(void) {
    Directory* root = NULL;
    Directory* tempDir = NULL;
    Directory* dirArr[MAX_NODE_SIZE];
    Directory* visitdDirs[MAX_NODE_SIZE];

    int arrIndex = 0;
    int visitedIndex = 0;

    int resultSize = 0;

    memset(dirArr, 0, sizeof(Directory*) * MAX_NODE_SIZE);
    memset(visitdDirs, 0, sizeof(Directory*) * MAX_NODE_SIZE);

    initFileSystemFromFile(&root, "sample.txt");

    tempDir = root;

    while (tempDir != NULL) {
        // printf("%s\n", tempDir->name);
        if (tempDir->size <= 100000) {
			// printf("Size: %d\n", tempDir->size);
            resultSize += tempDir->size;
        }
        visitdDirs[visitedIndex++] = tempDir;

        for (int i = 0; i < tempDir->childrenCount; i++) {
            if (!arrayContains(dirArr, arrIndex, tempDir->children[i]) &&
                !arrayContains(visitdDirs, visitedIndex, tempDir->children[i])) {
                dirArr[arrIndex++] = tempDir->children[i];
            }
        }

        tempDir = dirArr[arrIndex - 1];
        dirArr[--arrIndex] = NULL;
    }

    printf("Result size: %d\n", resultSize);

    //NOTE: Memory leak because I am lazy, might fix later

    return 0;
}

void initFileSystemFromFile(Directory** root, const char* fileName) {
    FILE* fp;
    char line[MAX_LINE_LENGTH];
    char* token;

    Directory* currentDirectory = NULL;
    Directory* tempDirectory = NULL;

    File* file;

    fp = fopen(fileName, "r");

    while (fgets(line, MAX_LINE_LENGTH, fp) != NULL) {
        if (line[strlen(line) - 1] == '\n') {
            line[strlen(line) - 1] = '\0';
        }
        token = strtok(line, " ");
        token = strtok(NULL, " ");

        if (atoi(line) != 0) { // FILE
            if (currentDirectory == NULL) {
                printf("No current directory\n");
                exit(-1);
            }
            file = malloc(sizeof(File)); // No check
            memset(file, 0, sizeof(File));

            initFile(file, token, atoi(line));

            addFileToDirectory(currentDirectory, file);

            tempDirectory = currentDirectory;
            while (tempDirectory != NULL) {
                tempDirectory->size += file->size;
                tempDirectory = tempDirectory->parent;
            }
        }
        else if (line[0] == COMMAND_IDENTIFIER) { // COMMAND
            if (strcmp(token, "cd") == 0) {
                token = strtok(NULL, " ");

                if (strcmp(token, "/") == 0) {
                    if (*root == NULL) {
                        *root = malloc(sizeof(Directory)); // No check
                        memset(*root, 0, sizeof(Directory));
                        initDirectory(*root, token, NULL);
                    }

                    currentDirectory = *root;
                }
                else if (strcmp(token, "..") == 0) {
                    // Maybe stop if root wasn't initialized

                    if (currentDirectory->parent != NULL) {
                        currentDirectory = currentDirectory->parent;
                    }
                }
                else if (token != NULL) {
                    tempDirectory = getDirectory(currentDirectory, token);

                    if (tempDirectory == NULL) {
                        tempDirectory = malloc(sizeof(Directory)); // No check
                        initDirectory(tempDirectory, token, currentDirectory);
                        currentDirectory->children[currentDirectory->childrenCount++] =
                            tempDirectory;
                        tempDirectory->parent = currentDirectory;
                    }
                    currentDirectory = tempDirectory;
                }
            }
            else if (strcmp(token, "ls") == 0) {
                // Nothing, just wait
            }
            else {
                printf("Invalid command\n");
            }
        }
        else { // DIRECTORY
               // Nothing
        }
        memset(line, 0, MAX_LINE_LENGTH);
    }

    fclose(fp);
}

void initDirectory(Directory* directory, const char* name, Directory* parent) {
    directory->name = malloc(sizeof(char) * (strlen(name) + 1));
    strcpy(directory->name, name);

    directory->parent = parent;

    directory->children = malloc(sizeof(Directory) * INIT_CHILDREN_COUNT);
    directory->childrenCount = 0;
    directory->childrenAlloc = INIT_CHILDREN_COUNT;

    memset(directory->children, 0, sizeof(Directory) * INIT_CHILDREN_COUNT);

    directory->files = malloc(sizeof(File) * INIT_CHILDREN_COUNT);
    directory->filesCount = 0;
    directory->filesAlloc = INIT_CHILDREN_COUNT;

	directory->size = 0;
}

Directory* getDirectory(Directory* currentDir, char* name) {
    int i;
    for (i = 0; i < currentDir->childrenCount; i++) {
        if (strcmp(currentDir->children[i]->name, name) == 0) {
            return currentDir->children[i];
        }
    }
    return NULL;
}

void initFile(File* file, char* name, int size) {
    file->name = malloc(sizeof(char) * (strlen(name) + 1));
    strcpy(file->name, name);

    file->size = size;
}

void addFileToDirectory(Directory* directory, File* file) { // SIZE NOT COUNTED
    if (directory->filesCount == directory->filesAlloc) {
        directory->filesAlloc *= 2;
        directory->files = realloc(directory->files, sizeof(File) * directory->filesAlloc);
    }

    directory->files[directory->filesCount++] = *file;
}

bool arrayContains(Directory** arr, int size, Directory* dir) {
    for (int i = 0; i < size; i++) {
        if (arr[i] == dir) {
            return true;
        }
    }
    return false;
}
