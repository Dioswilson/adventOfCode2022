#include "stack.h"
#include <stdlib.h>
#include <stdio.h>


/**Initializes a stack*/
void initStack(Stack* stack) {
    stack->data = malloc(sizeof(char) * 5);
    stack->size = 0;
    stack->allocated = 5;
}

/**Adds an element to the stack**/
void push(Stack* stack, char data) {
    if (stack->size == stack->allocated) {
        stack->data = realloc(stack->data, stack->allocated * 2);
        stack->allocated *= 2;
    }
    stack->data[stack->size++] = data;
}

/**Removes an element from the stack
 * Returns -1 if the stack is empty*/
char pop(Stack* stack) {
    if (stack->size == 0) {
        return -1;
    }
    stack->size--;
    return stack->data[stack->size];
}

/**Returns the element at the given position
 * Returns -1 if the stack is empty*/
char getAtPosition(Stack* stack, int position) {
    if (stack->size == 0) {
        return -1;
    }
    return stack->data[position];
}

/**Returns the last element of the stack
 * Returns -1 if the stack is empty*/
char getLast(Stack* stack) {
    if (stack->size == 0) {
        return -1;
    }
    return stack->data[stack->size - 1];
}

/**Shrinks the stack to the current size*/
void shrinkToSize(Stack* stack) {
    stack->data = realloc(stack->data, stack->size);
    stack->allocated = stack->size;
}

/**Frees the stack*/
void freeStack(Stack* stack) {
    free(stack->data);
    stack->data = NULL;
    stack->size = 0;
    stack->allocated = 0;
}

//**Prints the stack*/
void printStack(Stack* stack) {
    if (stack->size == 0) {
        // printf("Empty stack\n");
        return;
    }
    printf("Stack with %d elements: \n", stack->size);
    for (int i = 0; i < stack->size; i++) {
        printf("[%d]%c\n", i, stack->data[i]);
    }
    printf("------------");
    printf("\n");
}
