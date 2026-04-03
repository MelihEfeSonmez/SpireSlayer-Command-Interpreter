#include "state.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LINE_LENGTH 1024



// ---FUNCTIONS---

// Function: remove new line character
static void chomp_newline(char *line) {
    size_t len = strlen(line);

    if (len > 0 && line[len - 1] == '\n') {
        line[len - 1] = '\0';
    }
}

// Function: parse and execute the command line
static int execute_line(GameState *state, const char *line) {
    (void)state;
    (void)line;

    /*
     * Implement the Spring 2026 "spireslayer" grammar here.
     *
     * A typical approach is:
     * 1. Parse the whole line.
     * 2. Reject malformed input by returning 0.
     * 3. Apply semantic checks and print the required response.
     * 4. Return 1 after printing any non-INVALID response.
     */
    return 0;
}

// Function: main 
int main(void) {
    char line[MAX_LINE_LENGTH + 2];
    GameState state;

    init_game_state(&state);

    // loop which continues until reaching "Exit" command line
    while (1) {
        printf(">> ");
        fflush(stdout);

        // check whether there is any command line
        if (fgets(line, sizeof(line), stdin) == NULL) {
            break;
        }

        chomp_newline(line); // remove new line character

        // check "Exit" command line
        if (strcmp(line, "Exit") == 0) {
            break;
        }

        // try to parse and execute the command line 
        // if fails, print "INVALID"
        if (!execute_line(&state, line)) {
            printf("INVALID\n");
        }
    }

    return 0;
}
