#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 100
#define INITIAL_MAP_SIZE 10
typedef enum {
    NORTH,
    EAST,
    SOUTH,
    WEST,
} Direction;

typedef struct {
    int x;
    int y;
    char height;
} MapEntry;

typedef struct {
    MapEntry* entries; // Dinamic array
    int size;
    int _allocated;
    int width;
    int height;
} Map;

void initMap(Map* map);

void addEntry(Map* map, MapEntry entry);

void printMap(Map* map);

bool arrayContains(MapEntry** array, int size, MapEntry* entry);

int searchShortestPath(Map* map, MapEntry* start, MapEntry* end);

int main() {
    FILE* fp;
    Map map;
    char line[MAX_LINE_LENGTH];
    int lineLength = 0;
    MapEntry entry;
    int height = 0;
    MapEntry start;
    MapEntry end;
    int shortestPath = 0;

    initMap(&map);

    fp = fopen("sample.txt", "r");

    while (fgets(line, MAX_LINE_LENGTH, fp) != NULL) {
        lineLength = strlen(line);
        if (line[lineLength - 1] == '\n') {
            line[lineLength - 1] = '\0';
            lineLength--;
        }

        map.width = lineLength; // It will ocerride if not all are the same
        map.height++;
        for (int i = 0; i < lineLength; i++) {
            entry.x = i;
            entry.y = map.height - 1;
            if (line[i] == 'S') {
                entry.height = 'a';
                start = entry;
            }
            else if (line[i] == 'E') {
                entry.height = 'z';
                end = entry;
            }
            else {
                entry.height = line[i];
            }

            // printf("%c", height);
            addEntry(&map, entry);
        }
    }
    // printMap(&map);

    shortestPath = searchShortestPath(&map, &start, &end);

    printf("Shortest path: %d\n", shortestPath);

    fclose(fp);
}

void initMap(Map* map) {
    map->entries = malloc(sizeof(MapEntry) * INITIAL_MAP_SIZE);
    map->_allocated = INITIAL_MAP_SIZE;

    map->size = 0;
    map->width = 0;
    map->height = 0;
}

void addEntry(Map* map, MapEntry entry) {
    if (map->size == map->_allocated) {
        map->_allocated *= 2;
        map->entries = realloc(map->entries, sizeof(MapEntry) * map->_allocated);
    }

    map->entries[map->size] = entry;
    map->size++;
}

void printMap(Map* map) {
    int index = 0;
    for (int i = 0; i < map->height; i++) {
        for (int j = 0; j < map->width; j++) {
            index = i * map->width + j;
            if (index < map->size) {
                printf("%c", map->entries[index].height);
            }
        }
        printf("\n");
    }
}

bool arrayContains(MapEntry** array, int size, MapEntry* entry) {
    for (int i = 0; i < size; i++) {
        if (array[i]->x == entry->x && array[i]->y == entry->y) {
            return true;
        }
    }
    return false;
}

int searchShortestPath(Map* map, MapEntry* start, MapEntry* end) {
    int pathDepth = 0;
    int shortestPath = 0;
    int row = start->y;
    int col = start->x;
    MapEntry* current = start;
    MapEntry* visited[map->size]; // Note: Probably better to use a boolean array and use col+row
    MapEntry* queue[map->size];
    MapEntry* tempEntry = 0;

    int visitedIndex = 0;
    int queueIndex = 0;
    int currentLayerSize = 1;
    int nextLayerSize = 0;

    memset(visited, 0, sizeof(visited));
    memset(queue, 0, sizeof(queue));

    while (current != NULL) {
        // Add to visited
        visited[visitedIndex++] = current;

        // LOGIC ON CURRENT DIR
        if (current->x == end->x && current->y == end->y) {
            shortestPath = pathDepth;
            // printf("FOUND\n\n");
            break;
        }

        // add neighbors to queue
        for (Direction direction = NORTH; direction <= WEST; direction++) {
            switch (direction) {
                case NORTH:
                    if (current->y > 0) {
                        row = current->y - 1;
                    }
                    col = current->x;
                    break;
                case EAST:
                    if (current->x < map->width - 1) {
                        col = current->x + 1;
                    }
                    row = current->y;
                    break;
                case SOUTH:
                    if (current->y < map->height - 1) {
                        row = current->y + 1;
                    }
                    col = current->x;
                    break;
                case WEST:
                    if (current->x > 0) {
                        col = current->x - 1;
                    }
                    row = current->y;
                    break;
            }

            tempEntry = &map->entries[row * map->width + col];

            if (!arrayContains(visited, visitedIndex, tempEntry) &&
                !arrayContains(queue, queueIndex, tempEntry) &&
                tempEntry->height - 1 <= current->height) {
                queue[queueIndex++] = tempEntry;
                nextLayerSize++;
            }
        }

        currentLayerSize--;

        if (currentLayerSize == 0) {
            currentLayerSize = nextLayerSize;
            nextLayerSize = 0;
            pathDepth++;
        }

        current = queue[0];
        memmove(queue, queue + 1, sizeof(MapEntry*) * (queueIndex));
        queueIndex--;
    }

    return pathDepth;
}
