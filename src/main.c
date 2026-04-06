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

            return 0;
        }

        // b) Ironclad gains card <C>
        if (token_count >= 4 && strcmp(tokens[1], "gains") == 0 && strcmp(tokens[2], "card") == 0) {
            
            if (is_valid_name(tokens, spaces_before, 3, token_count - 1)) {
                char card_name[MAX_LINE_LENGTH];
                extract_name(tokens, 3, token_count - 1, card_name);

                execute_gain_card(state, card_name);
                
                return 1;
            }

            return 0;  
        }

        // c) Ironclad gains relic <R>
        if (token_count >= 4 && strcmp(tokens[1], "gains") == 0 && strcmp(tokens[2], "relic") == 0) {
            
            if (is_valid_name(tokens, spaces_before, 3, token_count - 1)) {
                char relic_name[MAX_LINE_LENGTH];
                extract_name(tokens, 3, token_count - 1, relic_name);

                execute_gain_relic(state, relic_name);
                
                return 1;
            }
            
            return 0;
        }

        // d) Ironclad gains potion <P>
        if (token_count >= 4 && strcmp(tokens[1], "gains") == 0 && strcmp(tokens[2], "potion") == 0) {
            
            if (is_valid_name(tokens, spaces_before, 3, token_count - 1)) {
                char potion_name[MAX_LINE_LENGTH];
                extract_name(tokens, 3, token_count - 1, potion_name);
                
                execute_gain_potion(state, potion_name);
                
                return 1;
            }
            
            return 0;    
        }        

        // e) Ironclad buys card <C> for <n> gold
        if (token_count >= 7 && strcmp(tokens[1], "buys") == 0 && strcmp(tokens[2], "card") == 0 && 
            strcmp(tokens[token_count - 3], "for") == 0 && strcmp(tokens[token_count - 1], "gold") == 0) {
            
            if (is_valid_positive_integer(tokens[token_count - 2]) && is_valid_name(tokens, spaces_before, 3, token_count - 4)) {
                int price = atoi(tokens[token_count - 2]);
                char card_name[MAX_LINE_LENGTH];
                extract_name(tokens, 3, token_count - 4, card_name);

                execute_buy_card(state, card_name, price);

                return 1;
            }

            return 0;
        }
   
        // f) Ironclad buys relic <R> for <n> gold
        if (token_count >= 7 && strcmp(tokens[1], "buys") == 0 && strcmp(tokens[2], "relic") == 0 && 
            strcmp(tokens[token_count - 3], "for") == 0 && strcmp(tokens[token_count - 1], "gold") == 0) {
            
            if (is_valid_positive_integer(tokens[token_count - 2]) && is_valid_name(tokens, spaces_before, 3, token_count - 4)) {
                int price = atoi(tokens[token_count - 2]);
                char relic_name[MAX_LINE_LENGTH];
                extract_name(tokens, 3, token_count - 4, relic_name);

                execute_buy_relic(state, relic_name, price);

                return 1;
            }

            return 0;
        }

        // g) Ironclad buys potion <P> for <n> gold
        if (token_count >= 7 && strcmp(tokens[1], "buys") == 0 && strcmp(tokens[2], "potion") == 0 && 
            strcmp(tokens[token_count - 3], "for") == 0 && strcmp(tokens[token_count - 1], "gold") == 0) {
            
            if (is_valid_positive_integer(tokens[token_count - 2]) && is_valid_name(tokens, spaces_before, 3, token_count - 4)) {
                int price = atoi(tokens[token_count - 2]);
                char potion_name[MAX_LINE_LENGTH];
                extract_name(tokens, 3, token_count - 4, potion_name);

                execute_buy_potion(state, potion_name, price);

                return 1;
            }

            return 0;
        }

        // h) Ironclad removes card <C>
        if (token_count >= 4 && strcmp(tokens[1], "removes") == 0 && strcmp(tokens[2], "card") == 0) {
            
            if (is_valid_name(tokens, spaces_before, 3, token_count - 1)) {
                char card_name[MAX_LINE_LENGTH];
                extract_name(tokens, 3, token_count - 1, card_name);

                execute_remove_card(state, card_name);

                return 1;
            }
            
            return 0;
        }

        // i) Ironclad removes upgraded card <C>
        if (token_count >= 5 && strcmp(tokens[1], "removes") == 0 && strcmp(tokens[2], "upgraded") == 0 &&
            strcmp(tokens[3], "card") == 0) {
            
            if (is_valid_name(tokens, spaces_before, 4, token_count - 1)) {    
                char card_name[MAX_LINE_LENGTH];
                extract_name(tokens, 4, token_count - 1, card_name);

                execute_remove_upgraded_card(state, card_name);

                return 1;
            }
            
            return 0;
        }

        // j) Ironclad upgrades card <C>
        if (token_count >= 4 && strcmp(tokens[1], "upgrades") == 0 && strcmp(tokens[2], "card") == 0) {
                
            if (is_valid_name(tokens, spaces_before, 3, token_count - 1)) {
                char card_name[MAX_LINE_LENGTH];
                extract_name(tokens, 3, token_count - 1, card_name);

                execute_upgrade_card(state, card_name);

                return 1;
            }
                
            return 0;
        }

        // k) Ironclad enters <room> room
        if (token_count >= 4 && strcmp(tokens[1], "enters") == 0 && strcmp(tokens[3], "room") == 0) {
              
            const char *room_types[] = {"Monster", "Elite", "Rest", "Shop", "Treasure", "Event", "Boss"};
            int num_room_types = sizeof(room_types) / sizeof(room_types[0]);
            int is_valid_room = 0;

            for (int i = 0; i < num_room_types; i++) {
                if (strcmp(tokens[2], room_types[i]) == 0) {
                    is_valid_room = 1;
                    break;
                }
            }

            if (is_valid_room) {
                char room_name[MAX_LINE_LENGTH];
                extract_name(tokens, 2, token_count - 2, room_name);

                execute_enter_room(state, room_name);

                return 1;
            }
            
            return 0;
        }

        // l) Ironclad learns ... effective ... (3 function calls as card, relic, and potion)
        if (token_count >= 8 && strcmp(tokens[1], "learns") == 0) {
            int is_index = -1;

            for (int i = 3; i < token_count - 3; i++) {
                if(strcmp(tokens[i], "is") == 0 && strcmp(tokens[i + 1], "effective") == 0 && strcmp(tokens[i + 2], "against") == 0) {
                    is_index = i;
                    break;
                }
            }

            if (is_index != -1) { // if "... is effective against" exists
                    
                if (is_valid_name(tokens, spaces_before, 3, is_index - 1) && is_valid_name(tokens, spaces_before, is_index + 3, token_count - 1)) {
                    char entity_name[MAX_LINE_LENGTH];
                    char enemy_name[MAX_LINE_LENGTH];

                    extract_name(tokens, 3, is_index - 1, entity_name); // extract name of card, relic, or potion
                    extract_name(tokens, is_index + 3, token_count - 1, enemy_name);    

                    // 3 function calls as card, relic, and potion
                    if (strcmp(tokens[2], "card") == 0) {
                        execute_learn_card_effective(state, entity_name, enemy_name);
                        return 1;
                    } else if (strcmp(tokens[2], "relic") == 0) {
                        execute_learn_relic_effective(state, entity_name, enemy_name);
                        return 1;
                    } else if (strcmp(tokens[2], "potion") == 0) {
                        execute_learn_potion_effective(state, entity_name, enemy_name);
                        return 1;
                    }
                }

                return 0;
            }
        }
        
        // v) Ironclad fights <E> for <n> gold
        if (token_count >= 6 && strcmp(tokens[1], "fights") == 0 && strcmp(tokens[token_count - 3], "for") == 0 && 
            strcmp(tokens[token_count - 1], "gold") == 0) {
            
            if (is_valid_positive_integer(tokens[token_count - 2]) && is_valid_name(tokens, spaces_before, 2, token_count - 4)) {
                int bounty = atoi(tokens[token_count - 2]);
                char enemy_name[MAX_LINE_LENGTH];
                extract_name(tokens, 2, token_count - 4, enemy_name);

                execute_fight_enemy_bounty(state, enemy_name, bounty);

                return 1;
            }

            return 0;
        }  

        // m) Ironclad fights <E>
        if (token_count >= 3 && strcmp(tokens[1], "fights") == 0) {

            // check whether it is "...fights <E> for <n> gold" line
            if ( !(token_count >= 6 && strcmp(tokens[token_count - 1], "gold") == 0 && strcmp(tokens[token_count - 3], "for") == 0) ) {
                
                if (is_valid_name(tokens, spaces_before, 2, token_count - 1)) {
                    char enemy_name[MAX_LINE_LENGTH];
                    extract_name(tokens, 2, token_count - 1, enemy_name);

                    execute_fight_enemy(state, enemy_name);

                    return 1;
                }
                
                return 0;
            }
        }

        // n) Ironclad heals <n> hp
        if (token_count == 4 && strcmp(tokens[1], "heals") == 0 && strcmp(tokens[3], "hp") == 0) {
            
            if (is_valid_positive_integer(tokens[2])) {
                int amount = atoi(tokens[2]);
                execute_heal(state, amount);
                return 1;
            }

            return 0;
        }

        // o) Ironclad takes <n> damage
        if (token_count == 4 && strcmp(tokens[1], "takes") == 0 && strcmp(tokens[3], "damage") == 0) {
            
            if (is_valid_positive_integer(tokens[2])) {
                int amount = atoi(tokens[2]);
                execute_take_damage(state, amount);
                return 1;
            }

            return 0;
        }

        // p) Ironclad discards potion <P>
        if (token_count >= 4 && strcmp(tokens[1], "discards") == 0 && strcmp(tokens[2], "potion") == 0) {

            if (is_valid_name(tokens, spaces_before, 3, token_count - 1)) {
                char potion_name[MAX_LINE_LENGTH];
                extract_name(tokens, 3, token_count - 1, potion_name);
        
                execute_discard_potion(state, potion_name);

                return 1;
            }
            
            return 0;
        }

        // r) Ironclad sells card <C> for <n> gold
        if (token_count >= 7 && strcmp(tokens[1], "sells") == 0 && strcmp(tokens[2], "card") == 0 && 
            strcmp(tokens[token_count - 3], "for") == 0 && strcmp(tokens[token_count - 1], "gold") == 0) {
            
            if (is_valid_positive_integer(tokens[token_count - 2]) && is_valid_name(tokens, spaces_before, 3, token_count - 4)) {
                int price = atoi(tokens[token_count - 2]);
                char card_name[MAX_LINE_LENGTH];
                extract_name(tokens, 3, token_count - 4, card_name);

                execute_sell_card(state, card_name, price);

                return 1;
            }

            return 0;
        }

        // s) Ironclad sells upgraded card <C> for <n> gold
        if (token_count >= 8 && strcmp(tokens[1], "sells") == 0 && strcmp(tokens[2], "upgraded") == 0 && strcmp(tokens[3], "card") == 0 &&
            strcmp(tokens[token_count - 3], "for") == 0 && strcmp(tokens[token_count - 1], "gold") == 0) {
            
            if (is_valid_positive_integer(tokens[token_count - 2]) && is_valid_name(tokens, spaces_before, 4, token_count - 4)) {
                int price = atoi(tokens[token_count - 2]);
                char card_name[MAX_LINE_LENGTH];
                extract_name(tokens, 4, token_count - 4, card_name);

                execute_sell_upgraded_card(state, card_name, price);

                return 1;
            }

            return 0;
        }

        // t) Ironclad sells potion <P> for <n> gold
        if (token_count >= 7 && strcmp(tokens[1], "sells") == 0 && strcmp(tokens[2], "potion") == 0 &&
            strcmp(tokens[token_count - 3], "for") == 0 && strcmp(tokens[token_count - 1], "gold") == 0) {
            
            if (is_valid_positive_integer(tokens[token_count - 2]) && is_valid_name(tokens, spaces_before, 3, token_count - 4)) {
                int price = atoi(tokens[token_count - 2]);
                char potion_name[MAX_LINE_LENGTH];
                extract_name(tokens, 3, token_count - 4, potion_name);

                execute_sell_potion(state, potion_name, price);

                return 1;
            }

            return 0;
        }        

        // u) Ironclad marks card <C> as exhaust
        if (token_count >= 6 && strcmp(tokens[1], "marks") == 0 && strcmp(tokens[2], "card") == 0 &&
            strcmp(tokens[token_count - 2], "as") == 0 && strcmp(tokens[token_count - 1], "exhaust") == 0) {

            if (is_valid_name(tokens, spaces_before, 3, token_count - 3)) {
                char card_name[MAX_LINE_LENGTH];
                extract_name(tokens, 3, token_count - 3, card_name);

                execute_mark_exhaust(state, card_name);

                return 1;
            }
            
            return 0;
        }       

        // y) Ironclad gains <n> max hp
        if (token_count == 5 && strcmp(tokens[1], "gains") == 0 && strcmp(tokens[3], "max") == 0 && 
            strcmp(tokens[4], "hp") == 0) {
            
            if (is_valid_positive_integer(tokens[2])) {
                int amount = atoi(tokens[2]);

                execute_gain_max_hp(state, amount);

                return 1;
            }

            return 0;
        } 

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
        if (token_count >= 4 && strcmp(tokens[0], "Total") == 0 && strcmp(tokens[1], "card") == 0) {
            
            // Card name starts at index 2 and ends before ?
            if (is_valid_name(tokens, spaces_before, 2, token_count - 2)) {
                
                char card_name[MAX_LINE_LENGTH];
                extract_name(tokens, 2, token_count - 2, card_name);
                
                execute_total_card_query(state, card_name);
                return 1;
            }
        }

        // e) Total upgraded card <card> ?
        if (token_count >= 5 && strcmp(tokens[0], "Total") == 0 && strcmp(tokens[1], "upgraded") == 0 && strcmp(tokens[2], "card") == 0) {
            
            // Card name starts at index 3 and ends before ?
            if (is_valid_name(tokens, spaces_before, 3, token_count - 2)) {
                
                char card_name[MAX_LINE_LENGTH];
                extract_name(tokens, 3, token_count - 2, card_name);
                
                execute_total_upgraded_card_query(state, card_name);
                return 1;
            }
        }

        // f) Deck ?
        if (token_count == 2 && strcmp(tokens[0], "Deck") == 0) {
            execute_deck_query(state);
            return 1;
        }

        // g) Relics ?
        if (token_count == 2 && strcmp(tokens[0], "Relics") == 0) {
            execute_relics_query(state);
            return 1;
        }
        
        // h) Potions ?
        if (token_count == 2 && strcmp(tokens[0], "Potions") == 0) {
            execute_potions_query(state);
            return 1;
        }

        // i) What is effective against <enemy> ?
        if (token_count >= 6 && strcmp(tokens[0], "What") == 0 && strcmp(tokens[1], "is") == 0 && 
            strcmp(tokens[2], "effective") == 0 && strcmp(tokens[3], "against") == 0) {
            
            // Enemy name starts at index 4 and ends before the '?'
            if (is_valid_name(tokens, spaces_before, 4, token_count - 2)) {
                
                char enemy_name[MAX_LINE_LENGTH];
                extract_name(tokens, 4, token_count - 2, enemy_name);
                
                execute_effective_query(state, enemy_name);
                
                return 1;
            }
        }

        // j) Defeated <enemy> ?
        if (token_count >= 3 && strcmp(tokens[0], "Defeated") == 0) {
            
            if (is_valid_name(tokens, spaces_before, 1, token_count - 2)) {
                
                char enemy_name[MAX_LINE_LENGTH]; 
                extract_name(tokens, 1, token_count - 2, enemy_name);
                
                // Call function to print the result
                execute_defeated_query(state, enemy_name);
                
                return 1;
            }
        }

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
        if (token_count == 2 && strcmp(tokens[0], "Exhausts") == 0) {
            
            // Call function to print the total exhausted cards
            execute_exhausts_query(state);
            
            return 1;
        }



    }
    // IT WILL BE REMOVED ***********************
    //(void)state;
    //(void)line;

    /*
     * Implement the Spring 2026 "spireslayer" grammar here.
     *
     * A typical approach is:
     * 1. Parse the whole line.
     * 2. Reject malformed input by returning 0.
     * 3. Apply semantic checks and print the required response.
     * 4. Return 1 after printing any non-INVALID response.
     */
    //return 0;
    // UNTIL HERE ***********************

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
            printf("» INVALID\n");
        }
    }

    return 0;
}
