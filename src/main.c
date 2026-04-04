#include "state.h"
#include "parser.h"
#include "sentences.h"
#include "queries.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


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
    int spaces_before[MAX_TOKENS];
    int token_count = tokenize_line(line, tokens, spaces_before);    

    if (token_count == 0) { // empty line
        return 0;
    }
    
    // ---INPUT LINES---
    // I) sentence—a state-mutating command, 
    if (strcmp(tokens[0], "Ironclad") == 0) {
        
        if (token_count < 3) { // no input line smaller than 3 tokens
            return 0;
        }

        // a) Ironclad gains <n> gold
        if (token_count == 4 && strcmp(tokens[1], "gains") == 0 && strcmp(tokens[3], "gold") == 0) {
            if (is_valid_positive_integer(tokens[2])) {
                int amount = atoi(tokens[2]);
                
                execute_gain_gold(state, amount);

                return 1;
            }
        }

        // b) Ironclad gains card <C>
        if (token_count >= 4 && strcmp(tokens[1], "gains") == 0 && strcmp(tokens[2], "card") == 0) {
            char card_name[MAX_LINE_LENGTH];
            extract_name(tokens, 3, token_count - 1, card_name);

            execute_gain_card(state, card_name);
            
            return 1;
        }

        // c) Ironclad gains relic <R>
        if (token_count >= 4 && strcmp(tokens[1], "gains") == 0 && strcmp(tokens[2], "relic") == 0) {
            char relic_name[MAX_LINE_LENGTH];
            extract_name(tokens, 3, token_count - 1, relic_name);

            execute_gain_relic(state, relic_name);
            
            return 1;
        }

        // d) Ironclad gains potion <P>
        if (token_count >= 4 && strcmp(tokens[1], "gains") == 0 && strcmp(tokens[2], "potion") == 0) {
            char potion_name[MAX_LINE_LENGTH];
            extract_name(tokens, 3, token_count - 1, potion_name);
            
            execute_gain_potion(state, potion_name);
            
            return 1;
        }        

        // e) Ironclad buys card <C> for <n> gold
        if (token_count >= 7 && strcmp(tokens[1], "buys") == 0 && strcmp(tokens[2], "card") == 0 && 
            strcmp(tokens[token_count - 3], "for") == 0 && strcmp(tokens[token_count - 1], "gold") == 0) {
            
            if (is_valid_positive_integer(tokens[token_count - 2])) {
                int price = atoi(tokens[token_count - 2]);
                char card_name[MAX_LINE_LENGTH];
                extract_name(tokens, 3, token_count - 4, card_name);

                execute_buy_card(state, card_name, price);

                return 1;
            }
        }
   
        // f) Ironclad buys relic <R> for <n> gold
        if (token_count >= 7 && strcmp(tokens[1], "buys") == 0 && strcmp(tokens[2], "relic") == 0 && 
            strcmp(tokens[token_count - 3], "for") == 0 && strcmp(tokens[token_count - 1], "gold") == 0) {
            
            if (is_valid_positive_integer(tokens[token_count - 2])) {
                int price = atoi(tokens[token_count - 2]);
                char relic_name[MAX_LINE_LENGTH];
                extract_name(tokens, 3, token_count - 4, relic_name);

                execute_buy_relic(state, relic_name, price);

                return 1;
            }
        }

        // g) Ironclad buys potion <P> for <n> gold
        if (token_count >= 7 && strcmp(tokens[1], "buys") == 0 && strcmp(tokens[2], "potion") == 0 && 
            strcmp(tokens[token_count - 3], "for") == 0 && strcmp(tokens[token_count - 1], "gold") == 0) {
            
            if (is_valid_positive_integer(tokens[token_count - 2])) {
                int price = atoi(tokens[token_count - 2]);
                char potion_name[MAX_LINE_LENGTH];
                extract_name(tokens, 3, token_count - 4, potion_name);

                execute_buy_potion(state, potion_name, price);

                return 1;
            }
        }

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
    // At least 2 tokens and ends with '?'
    if (token_count >= 2 && strcmp(tokens[token_count - 1], "?") == 0) {

        // a) Total gold ?
        if (token_count == 3 && strcmp(tokens[0], "Total") == 0 && strcmp(tokens[1], "gold") == 0) {
            execute_gold_query(state);
            return 1;
        }

        // b) Floor ?
        if (token_count == 2 && strcmp(tokens[0], "Floor") == 0) {
            execute_floor_query(state);
            return 1;
        }

        // c) Where ?
        if (token_count == 2 && strcmp(tokens[0], "Where") == 0) {
            execute_where_query(state);
            return 1;
        }

        // d) Total card <card> ?

        // e) Total upgraded card <card> ?

        // f) Deck ?

        // g) Relics ?
        
        // h) Potions ?

        // i) What is effective against <enemy> ? ?

        // j) Defeated <enemy> ?

        // k) Health ?
        if (token_count == 2 && strcmp(tokens[0], "Health") == 0) {
            execute_health_query(state);
            return 1;
        }

        // l) Deck size ?
        if (token_count == 3 && strcmp(tokens[0], "Deck") == 0 && strcmp(tokens[1], "size") == 0) {
            execute_deck_size_query(state);
            return 1;
        }
        
        // m) Exhausts ?



    }
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
