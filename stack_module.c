// stack_module.c
// Implements CharStack operations for typed characters (current line only)
// and accuracy calculation for the displayed line.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_CHARS 1000

// Stack structure to store typed characters
typedef struct
{
    char arr[MAX_CHARS];
    int top;
} CharStack;

// Initializes an empty CharStack
void initStack(CharStack *s)
{
    s->top = -1;
}

// Clears the CharStack (used when a new line of words is displayed)
void clear(CharStack *s)
{
    s->top = -1;
}

// Checks if CharStack is empty
int isEmpty(CharStack *s)
{
    return s->top == -1;
}

// Checks if CharStack is full
int isFull(CharStack *s)
{
    return s->top == MAX_CHARS - 1;
}

// Pushes a typed character onto the CharStack
void push(CharStack *s, char c)
{
    if (isFull(s))
    {
        printf("Stack Overflow! Cannot type more characters.\n");
        return;
    }

    s->top = s->top + 1;
    s->arr[s->top] = c;
}

// Pops the last typed character (used for backspace)
char pop(CharStack *s)
{
    if (isEmpty(s))
    {
        return '\0'; // nothing to delete
    }

    char removed = s->arr[s->top];
    s->top = s->top - 1;
    return removed;
}

// Converts CharStack content to a string (helper function)
void toString(CharStack *s, char *typedStr)
{
    int i;
    for (i = 0; i <= s->top; i++)
    {
        typedStr[i] = s->arr[i];
    }
    typedStr[i] = '\0'; // null-terminate string
}

// Compares typed text with original and calculates accuracy percentage
// Calculates accuracy between two strings directly
float calculateAccuracy(const char *original, const char *typed, int totalCorrect, int totalTyped)
{   int len = strlen(original);

    int min_len;
    if (len < strlen(typed))
    {
        min_len = len;
    }
    else
    {
        min_len = strlen(typed);
    }

    for (int i = 0; i < min_len; i++)
    {
        if (original[i] == typed[i])
            totalCorrect++;
    }

    if (len == 0)
        return 0.0f;

    return ((float)totalCorrect / totalTyped) * 100.0f;
}

int computeWPM(int chars, double seconds)
{
    if (seconds <= 0.0)
        return 0;
    return (int)(((chars / 5.0) / (seconds / 60.0)) + 0.5);
}

void update(const char *line,const char *typed, int *totalCorrect){
     int len = strlen(line);
    
    int min_len;
    if (len < strlen(typed))
    {
        min_len = len;
    }
    else
    {
        min_len = strlen(typed);
    }

    for (int i = 0; i < min_len; i++)
    {
        if (line[i] == typed[i])
            (*totalCorrect)++;
    }
}