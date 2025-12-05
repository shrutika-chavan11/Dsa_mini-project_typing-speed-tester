// terminal_module.c
// Handles Linux terminal input (getch) and output formatting (colors, cursor control)

#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>

// --- Getch implementation for Linux ---
int getchLinux(void) {
    struct termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}

// --- Terminal formatting helpers ---
void clearLine() {
    printf("\033[2K");
}

void moveCursorUp(int n) {
    printf("\033[%dA", n);
}

void moveCursorRight(int n) {
    printf("\033[%dC", n);   // ANSI: move cursor n columns forward
}

void moveCursorLeft(int n) {
    printf("\033[%dD", n);   // ANSI: move cursor n columns backward
}

void resetColor() {
    printf("\033[0m");
}

void setGreen() {
    printf("\033[32m");
}

void setRed() {
    printf("\033[31m");
}

// Print typed string in color vs target string
void printColoredTyped(const char *target, const char *typed) {
    int tlen = strlen(target);
    int plen = strlen(typed);

    for (int i = 0; i < plen; i++) {
            if (typed[i] == target[i]) {
                setGreen();
                putchar(typed[i]);
            } else {
                setRed();
                putchar(typed[i]);
            }
        
        resetColor();
    }
}

