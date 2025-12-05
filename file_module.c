// file_module.c
// Reads words from a text file sequentially (no random selection).

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORDS 500
#define WORD_LEN 25

// Reads all words from the file into 'words' array.
// 'count' stores how many words were read.
void load_words_from_file(int level, char words[MAX_WORDS][WORD_LEN]) {
    FILE *f;
    switch (level) 
    {
    case 1:
        f = fopen("easy.txt", "r");
        break;
    
    case 2:
        f = fopen("medium.txt", "r");
        break;

    case 3:
        f = fopen("hard.txt", "r");
        break;

    default:
        break;
    }

    int count = 0;
    while (count < MAX_WORDS && fscanf(f, "%s", words[count]) == 1) {
        count++;
    }
    fclose(f);
}

//Generate random value for startIndex
void GenerateStartIndex(int *StartIndex) {
    static int visited[50] = {0};
    do {
        *StartIndex = rand()%50;
    } while(visited[*StartIndex] == 1);
    visited[*StartIndex] = 1;
    *StartIndex *= 10;
}

// Copies 10 sequential words starting from 'start_index' into 'selected'.
void get_next_10_words(char selected[10][WORD_LEN], char words[MAX_WORDS][WORD_LEN], int start_index) {
    for (int i = 0; i < 10; i++) {
        int idx = start_index + i;
        strcpy(selected[i], words[idx]);
    }
}

void create_line(char *line, char selected[10][25]) {
    line[0] = '\0';   // IMPORTANT: initialize line

    for (int i = 0; i < 10; i++) {
        strcat(line, selected[i]);

        if (i < 9)
            strcat(line, " ");
    }
}
   
