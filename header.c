// main.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "file_module.c"
#include "stack_module.c"
#include "terminal_module.c"
#include "doublyll_module.c"


time_t globalStart;

int main()
{
    char words[MAX_WORDS][WORD_LEN];
    char selected[10][WORD_LEN];

    load_sessions_from_files(); //stores acc,wpm and timestamps in a doubly linked list
    
    printf("\n--------------Welcome to TYPING TEST!------------------\n\n");
    int choice; //To keep repeating(trying again).
    do
    {

        int timeLimit;
        printf("Enter typing duration (30 or 60 seconds): ");
        scanf("%d", &timeLimit);

        printf("\nSelect level:\n1:Easy 2:Medium 3:Hard\n");
        int level;
        scanf("%d", &level);
        getchar(); // remove newline

        load_words_from_file(level, words);

        globalStart = time(NULL); // Store the starting time of the session
        int startIndex;
        // To store accuracy and WPM after each typed line.
        float acc = 0.0f;
        int wpm = 0;

        printf("\n--- Typing Test Started (%d seconds) ---\n", timeLimit);
        int totalCorrect = 0; //Number of chars typed correctly by the user.
        int totalTyped = 0; // Number of characters typed in current session.
        srand(time(NULL));  // This line helps generate random values for StartIndex different each time.

        // This loop prints new line after one line is finished
        while (difftime(time(NULL), globalStart) < timeLimit)
        {

            GenerateStartIndex(&startIndex);
            get_next_10_words(selected, words, startIndex);

            // Create a continuous line from selected array
            char line[WORD_LEN * 10];
            create_line(line, selected);

            // Print the line to be typed.
            printf("\nType this:\n%s\n", line);

            // Reserve one blank lines for typed text, and for Accuracy/WPM (initial)
            printf("\n");    // line typed by user (will be overwritten)
            printf("\n");    // accuracy and WPM (will be overwritten)
            moveCursorUp(2); // move cursor two lines up (for typed line + stats line)

            CharStack stack; //To store the characters typed by user.
            initStack(&stack);
            int curr_typed_line = 0; //No of characters typed in current line.
            
            //This loop takes character entered by user and analyzes it.
            while (1)
            {
                if (difftime(time(NULL), globalStart) >= timeLimit) // Time limit reached
                    break;
                int ch = getchLinux(); //This takes user input without need to press enter and doesn't print it in the terminal.

                if (ch == '\n' || ch == '\r' || curr_typed_line > strlen(line) - 1){ //Enter is pressed or line has ended
                    if(!isEmpty(&stack)){
                        char typed_temp[MAX_CHARS];
                        toString(&stack, typed_temp);
                        update(line,typed_temp, &totalCorrect); //This function updates totalCorrect after each line ends.
                    }
                    break;
                }else if (ch == 127 || ch == 8)
                { // delete or backspace
                    if (!isEmpty(&stack))
                    {
                        pop(&stack);
                        totalTyped--;
                        curr_typed_line--;
                    }
                }
                else if (ch >= 32 && ch <= 126)
                {
                    push(&stack, (char)ch);
                    totalTyped++;
                    curr_typed_line++;
                }

                char typed[MAX_CHARS];
                toString(&stack, typed);
                acc = calculateAccuracy(line, typed, totalCorrect, totalTyped);
                wpm = computeWPM(totalTyped, difftime(time(NULL), globalStart));

                clearLine(); //Erases current line to write the line again from start.
                moveCursorLeft(curr_typed_line + 1);
                printColoredTyped(line, typed);
                printf("\n");
                clearLine(); //Erases previous stats to write latest stats
                printf("Accuracy: %.2f%% | WPM: %d\n", acc, wpm);
                fflush(stdout);
                moveCursorUp(2); // move cursor two lines up (for typed line + stats line)
                moveCursorRight(curr_typed_line);
            }

            printf("\n\nLine done!\n");
           
        }

        printf("\n--- Time’s Up! ---\n");


        // add one session
        add_session((double) wpm, (double) acc);

        // for session history and graph menu
        do
        {
            printf("\n========= MENU =========\n");
            printf("1. Try Again\n");
            printf("2. View session history\n");
            printf("3. View previous session\n");
            printf("4. View latest session\n");
            printf("5. Plot WPM graph\n");
            printf("6. Plot Accuracy graph\n");
            printf("7. Exit\n");
            printf("\nEnter choice: ");
            scanf("%d", &choice);

            switch (choice)
            {
            case 1:
                break;
            case 2:
                displayAllSessions();
                break;

            case 3:
                displayPrevSession();
                break;

            case 4:
                displayNextSession();
                break;

            case 5:
                plotGraph();
                break;
            
            case 6:
                plotAccuracyGraph();
                break;

            case 7:
                printf("Exiting menu...\n");
                break;

            default:
                printf("Invalid choice! Try again.\n");
            }

        } while (choice != 7 && choice != 1);
    } while (choice == 1);

    save_text_files();
    return 0;
}
