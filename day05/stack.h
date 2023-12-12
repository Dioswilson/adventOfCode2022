#ifndef STACK_H
#define STACK_H

typedef struct {
    char* data;
    int size;
    int allocated;
} Stack;

void initStack(Stack* stack);

void push(Stack* stack, char data);

char pop(Stack* stack);

char getAtPosition(Stack* stack, int position);

char getLast(Stack* stack);

void shrinkToSize(Stack* stack);

void freeStack(Stack* stack);

void printStack(Stack* stack);

#endif // STACK_H
