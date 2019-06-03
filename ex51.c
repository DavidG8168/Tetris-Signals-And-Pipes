// =================================== Included Libraries =============================================================
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <termios.h>
// =================================== Function Declarations ==========================================================
char getch();
void PrintError();
int CheckValidKey(char key);
// =================================== Main ===========================================================================
// ====================================================================================================================
// The main function.
int main() {
    // Create the pipe.
    int Pipe[2];
    // Initialize the pipe.
    pipe(Pipe);
    // Create an integer to be the process ID.
    int pid;
    // Fork the process.
    pid = fork();
    // Handle fork() failure.
    if (pid < 0) {
        // Print error message.
        PrintError();
    }
    // Child process.
    if (pid == 0) {
        // Force stdin to be the read side of the pipe.
        dup2(Pipe[0], 0);
        // Call exe to run draw.out after compiling ex52.c .
        execlp("./draw.out", "./draw.out", NULL);
        // If the exe command fails we will reach this code.
        // Print error.
        PrintError();
    }
    // Main Process.
    char key;
    while (1) {
        // Get the char from the shell.
        key = getch();
        // Do nothing and continue if the input char does not have an action.
        if (!CheckValidKey(key)) {
            continue;
        }
        // Write the game key to the pipe.
        // Print the error message if writing failed.
        if(write(Pipe[1], &key, 1) < 0) {
            PrintError();
        }
        // Send a signal to ex52.
        // Keep sending SIGUSR2.
        kill(pid, SIGUSR2);
        // If the char is 'q' then stop the program.
        if (key == 'q') {
            break;
        }
    }
    return 0;
}
// =================================== Function Implementation  =======================================================
// ====================================================================================================================
// getch() function supplied by instructor.
// Used to read characters from the shell.
char getch() {
    char buf = 0;
    struct termios old = {0};
    if (tcgetattr(0, &old) < 0)
        perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if (tcsetattr(0, TCSANOW, &old) < 0)
        perror("tcsetattr ICANON");
    if (read(0, &buf, 1) < 0)
        perror ("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if (tcsetattr(0, TCSADRAIN, &old) < 0)
        perror ("tcsetattr ~ICANON");
    return (buf);
}
// ====================================================================================================================
// Check if the char input is a valid character, returns 1 if yes, otherwise return 0.
int CheckValidKey(char key) {
    // Checking the keys.
    // a - Left, d - Right, s - Down, w - Turn shape, q - Quit.
    if (key == 'a' || key == 'd' || key == 's' || key == 'w' || key == 'q') {
        return 1;
    }
    return 0;
}
// ====================================================================================================================
// Prints error message to standard error file descriptor.
void PrintError() {
    // The number of the STD_ERROR file descriptor.
    int stdError = 2;
    // Store the error message in a constant string.
    const char* error;
    error = "Error in system call\n";
    // Write it to the descriptor using the write system call.
    write(stdError, error, strlen(error));
}
// ====================================================================================================================
