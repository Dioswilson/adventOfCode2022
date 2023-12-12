#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 4

bool areAllDifferent(char* arr, int size);

int main() {

    int fd;
    char buff[BUFFER_SIZE];
    bool search = true;
    int offset = 0;

    memset(buff, 0, BUFFER_SIZE);

    fd = open("sample.txt", O_RDONLY);

    while (pread(fd, buff, BUFFER_SIZE, offset) > 0 && !areAllDifferent(buff, BUFFER_SIZE)) {
        offset++;
    }
    close(fd);

    printf("Start at: %d \n", offset + BUFFER_SIZE);

    return 0;
}

bool areAllDifferent(char* arr, int size) {

    for (int i = 0; i < size - 1; i++) {
        for (int j = i + 1; j < size; j++) {
            if (arr[i] == arr[j]) {
                return false;
            }
        }
    }
    return true;
}
