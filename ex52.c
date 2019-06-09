// =================================== Included Libraries =============================================================
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <signal.h>
#include <stdlib.h>
// =================================== Structures & Global Variables ==================================================
// Create a structure to represent the game.
typedef struct tetris_game_board {
    // Size of the grid.
    // 20 x 20 .
    char grid[20][20];
    // The Y-axis position of the shape.
    // Row.
    int row;
    // The X-axis position of the shape.
    // Column.
    int col;
    // Used to determine if the shape is horizontal or vertical position.
    // Horizontal - 0 .
    // - - -
    // Vertical - 1 .
    // -
    // -
    // -
    int position;
} Tetris;
// Global struct variable that will be used throughout the program.
Tetris tetris;
// ==================================== Function Declarations =========================================================
void InitializeGame(Tetris* tetris);
void CreateGrid(Tetris* tetris);
void MoveShape(int signum);
void CheckKeyAction(int signum);
void FlipPosition(Tetris* tetris);
void ShowGrid(Tetris* tetris);
void ClearShape(Tetris* tetris);
void PlaceShape(Tetris* tetris, int row, int col, int prevChanged);
// =================================== Main ===========================================================================
// ====================================================================================================================
// The main function.
int main() {
    // Start the game.
    InitializeGame(&tetris);
    // Print out the grid.
    ShowGrid(&tetris);
    // Define SIGALRM signal handler.
    // Signal moving the shape down.
    // Will be handled recursively.
    signal(SIGALRM, MoveShape);
    // Wait one second before the next signal.
    // The signal sent is SIGALRM.
    alarm(1);
    // Define SIGUSR2 signal handler.
    // Signal to check the key press.
    signal(SIGUSR2, CheckKeyAction);
    while (1) {
        // wait until the process gets a signal.
        // The signal will be SIGUSR2 from ex51.c .
        pause();
    }
}
// =================================== Function Implementation  =======================================================
// ====================================================================================================================
// Initialize the structure.
void InitializeGame(Tetris* tetris) {
    // Set all the members of the struct.
    // Start at a horizontal position.
    tetris->position = 0;
    // Starting grid location of shape.
    // Middle of top row.
    tetris->row = 0;
    tetris->col = 9;
    // Create the grid with '*' characters.
    CreateGrid(tetris);
    // Place the shape.
    PlaceShape(tetris, tetris->row, tetris->col, 0);

}
// ====================================================================================================================
// Handles the SIGALRM signal.
// Move the shape closer to the bottom every second.
void MoveShape(int signum) {
    // Re-define signal handler.
    signal(SIGALRM, MoveShape);
    // The signal will keep being sent, entering this function and moving the shape down.
    alarm(1);
    // Update the location of the shape on the grid.
    PlaceShape(&tetris, tetris.row + 1, tetris.col, 0);
    // Show the grid.
    ShowGrid(&tetris);
}
// ====================================================================================================================
// Handles the SIGUSR2 signal.
// Check the char key pressed and act as needed.
void CheckKeyAction(int signum) {
    // Re-define signal handler.
    signal(SIGUSR2, CheckKeyAction);
    // Get the key.
    char key = (char) getchar();
    // Move down.
    if (key == 's') {
        PlaceShape(&tetris, tetris.row + 1, tetris.col, 0);
    }
    // Move left.
    if (key == 'a') {
        PlaceShape(&tetris, tetris.row, tetris.col - 1, 0);
    }
    // Move right.
    if (key == 'd') {
        PlaceShape(&tetris, tetris.row, tetris.col + 1, 0);
    }
    // Flip the position of the shape.
    if (key == 'w') {
        FlipPosition(&tetris);
    }
    // Quit the program.
    if (key == 'q') {
        exit(1);
    }
    // Show the grid.
    ShowGrid(&tetris);
}
// ====================================================================================================================
// Flip the position of the shape.
void FlipPosition(Tetris* tetris) {
    // First delete the shape.
    ClearShape(tetris);
    // Save the previous location of the shape.
    int prevRow = tetris->row;
    int prevCol = tetris->col;
    // If horizontal change to vertical.
    if (tetris->position == 0) {
        // Change to vertical.
        tetris->position = 1;
        PlaceShape(tetris, prevRow - 1, prevCol + 1, 1);
    // If vertical change to horizontal.
    } else  {
        int x = prevRow + 1;
        int y = prevCol - 1;
        /* flip from vertical to horizontal will cause the shape to cross the right game border.*/
        if (y + 2 >= 20 - 1) {
            y = 20 - 4;
            /* flip from vertical to horizontal will cause the shape to cross the left game border.*/
        } else if (y < 1) {
            y = 1;
        }
        // Change to horizontal.
        tetris->position = 0;
        PlaceShape(tetris, x, y, 1);
    }
}
// ====================================================================================================================
// Initialize the grid by filling the grid multi-array with '*' characters.
void CreateGrid(Tetris* tetris) {
    int i, j;
    for (i = 0; i < 20 - 1; i++) {
        for (j = 0; j < 20; j++) {
            if (j == 0 || j == 20 - 1) {
                // Fill the borders with '*' .
                tetris->grid[i][j] = '*';
                continue;
            }
            // Fill the rest with ' ' .
            tetris->grid[i][j] = ' ';
        }
    }
    // Fill the bottom with '*' .
    for (j = 0; j < 20; j++) {
        tetris->grid[20 - 1][j] = '*';
    }
}
// ====================================================================================================================
// Prints the grid to the screen.
void ShowGrid(Tetris* tetris) {
    // Clear the screen.
    system("clear");
    int i,j;
    // Print each cell in pre-initialized grid array.
    for (i = 0; i < 20; i++) {
        for (j = 0; j < 20; j++) {
            printf("%c", tetris->grid[i][j]);
        }
        printf("\n");
    }
}
// ====================================================================================================================
// Deletes the  - - - shape from the grid.
void ClearShape(Tetris* tetris) {
    // If horizontal.
    if (tetris->position == 0) {
        // Delete the '-' chars by the X-axis.
        tetris->grid[tetris->row][tetris->col] = ' ';
        tetris->grid[tetris->row][tetris->col + 1] = ' ';
        tetris->grid[tetris->row][tetris->col + 2] = ' ';
    } else {
        // Vertical.
        // Delete the '-' chars by the Y-axis.
        tetris->grid[tetris->row][tetris->col] = ' ';
        tetris->grid[tetris->row + 1][tetris->col] = ' ';
        tetris->grid[tetris->row + 2][tetris->col] = ' ';
    }
}
// ====================================================================================================================
// Move the shape to create the falling animation.
void PlaceShape(Tetris* tetris, int row, int col, int prevChanged) {
    // Check if not crossing the allowed borders.
    // If horizontal.
    if (tetris->position == 0) {
        if (col <= 0)
            return;
        if (col + 2 >= 20 -1 )
            return;
    }
    // If vertical.
    if (tetris->position == 1 ) {
        if (col <= 0)
            return;
        if (col >= 20 - 1)
            return;
    }
    // Erase previous shape if we didn't flip it before.
    if (!prevChanged) {
        ClearShape(tetris);
    }
    // Set the new position of the shape.
    tetris->row = row;
    tetris->col = col;
    // "Re-Draw" the shape on the grid.
    // If horizontal.
    if (tetris->position == 0) {
        tetris->grid[row][col] = '-';
        tetris->grid[row][col + 1] = '-';
        tetris->grid[row][col + 2] = '-';
        // Once we hit the bottom, re-initialize the tetris game.
        if (row >= 20 - 1) {
            InitializeGame(tetris);
        }
    } else {
        // Vertical
        tetris->grid[row][col] = '-';
        tetris->grid[row + 1][col] = '-';
        tetris->grid[row + 2][col] = '-';
        // Once we hit the bottom, re-initialize the tetris game.
        if (row + 2 >= 20 - 1) {
            InitializeGame(tetris);
        }
    }
}
// ====================================================================================================================
