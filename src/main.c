#include "state.h"
#include "parser.h"

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
    char tokens[MAX_TOKENS][MAX_LINE_LENGTH];
    int token_count = tokenize_line(line, tokens);    
    
    if (token_count == 0) { // empty line
        return 0;
    }
    
    // ---INPUT LINES---
    // I) sentence—a state-mutating command, 
    if (strcmp(tokens[0], "Ironclad") == 0) {
        if (token_count < 3) {
            return 0;
        }

        // a) Ironclad gains <n> gold

        // b) Ironclad gains card <C>

        // c) Ironclad gains relic <R>

        // d) Ironclad gains potion <P>

        // e) Ironclad buys card <C> for <n> gold
        
        // f) Ironclad buys relic <R> for <n> gold

        // g) Ironclad buys potion <P> for <n> gold

        // h) Ironclad removes card <C>

        // i) Ironclad removes upgraded card <C>
        
        // j) Ironclad upgrades card <C>
        
        // k) Ironclad enters <room> room
        
        // l) Ironclad learns ... effective ... (3 function calls as card, relic, and potion)
        
        // m) Ironclad fights <E>
        
        // n) Ironclad heals <n> hp
        
        // o) Ironclad takes <n> damage
        
        // p) Ironclad discards potion <P>
        
        // r) Ironclad sells card <C> for <n> gold
        
        // s) Ironclad sells upgraded card <C> for <n> gold
        
        // t) Ironclad sells potion <P> for <n> gold
        
        // u) Ironclad marks card <C> as exhaust
        
        // v) Ironclad fights <E> for <n> gold
        
        // y) Ironclad gains <n> max hp
    
    }

    // II) question—a read-only query

        // a) Total gold ?

        // b) Floor ?

        // c) Where ?

        // d) Total card <card> ?

        // e) Total upgraded card <card> ?

        // f) Deck ?

        // g) Relics ?
        
        // h) Potions ?

        // i) What is effective against <enemy> ? ?

        // j) Defeated <enemy> ?

        // k) Health ?

        // l) Deck size ?
        
        // m) Exhausts ?




    // IT WILL BE REMOVED ***********************
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
    // UNTIL HERE ***********************

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
