# Typing Speed Tester (C Project)

A terminal-based Typing Speed Tester built in **C**, demonstrating DSA concepts through:
- Stack (typing & backspace)
- Arrays (WPM samples)
- Strings (paragraph loading)
- Doubly Linked List (last 10 sessions)
- File Handling (permanent session saving).

The program runs in Ubuntu terminal using `termios` for real-time key capture and prints colored feedback for correct/incorrect characters.

## Project Structure

Typing-Speed-Tester/
│
├── header.c              # Main driver file (includes all modules)
├── stack.c               # Stack implementation for typing/backspace handling
├── linkedlist.c          # Doubly Linked List for recent session history
├── graph.c               # GNUPlot graph generation functions
├── filehandling.c        # Session storage and retrieval using files
├── paragraph.txt         # Typing paragraphs/source text
├── sessions.txt          # Permanent session records
├── README.md             # Project documentation
└── output/               # Generated graph images and outputs

## Features
- Real-time typing with color output (green = correct, red = incorrect)
- Calculates WPM, Accuracy, and total time
- Stores all sessions permanently in text files
- Maintains last 10 sessions in a Doubly Linked List
- Generates:
  - WPM vs Session graph (GNUPlot)
  - Accuracy vs Session graph (GNUPlot)

## Menu Options (Inside Program)
1. Start New Typing Session  
2. View Last 10 Sessions (including current)  
3. View Previous Session  
4. Exit Program  
5. Generate WPM Graph (GNUPlot)  
6. Generate Accuracy Graph (GNUPlot)  

## How to Compile
(Only **header.c** needs to be compiled; it includes all other C modules)
gcc header.c -o header -lm
## How to Run (Ubuntu)
./header

## Requirements
- Ubuntu / WSL
- GCC Compiler
- GNUPlot (`sudo apt install gnuplot`)

## Future Improvements

1. Add multiple difficulty levels (Easy, Medium, Hard)
2. Include multiplayer typing race mode
3. Add timer-based custom tests (30s, 60s, 120s)
4. Improve UI using ncurses for a better terminal interface
5. Add support for random paragraph generation
6. Export session history as CSV or PDF reports
7. Track highest WPM and personal best accuracy
8. Add user login system for multiple profiles
9. Generate advanced analytics graphs and statistics
10. Support cross-platform compatibility beyond Ubuntu/WSL

## Summary
A complete C-based Typing Speed Tester that applies core DSA concepts using Stack, Arrays, Strings, Doubly Linked List, file handling, real-time keystroke capture, and graph visualization.
